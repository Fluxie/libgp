#include "referencecounter.h"

#include <memory>

ReferenceCounter::ReferenceCounter(
        std::string title
) :
    m_title( title ),
    m_running( false )
{
    // Reset counters.
    for( std::atomic< uint32_t >& c : m_counters )
        c.store( 0 );
}

//! Starts allocations.
void ReferenceCounter::start(
        unsigned int threads
)
{
    // Start.
    m_running = true;

    for( unsigned int t = 0; t < threads; ++t )
        m_allocators.emplace_back( allocationAlgorithm, this, t );
}


//! Stops Allocations.
void ReferenceCounter::stop()
{
    // Request stop.
    m_running = false;

    // Wait for the threads to stop.
    for( std::thread& t : m_allocators )
        t.join();
}

double ReferenceCounter::allocations() const
{
    // Calculate a sum of the results.
    // It does not need to be accurate => memory_order_relaxed.
    double result = 0.0;
    for( auto& counter : m_counters )
    {
        result += counter.load( std::memory_order_relaxed );
    }
    return result;
}

CustomData ReferenceCounter::data() const
{
    // Collect custom data.
    return CustomData();
}

void ReferenceCounter::allocationAlgorithm(
        ReferenceCounter* thisParam,
        size_t index
)
{
    std::vector< std::shared_ptr< std::vector< std::thread::id > >> allocations;
    while( thisParam->m_running )
    {

        // Allocate 100 objects.
        {
            for( int c = 0; c < 100; c++ )
            {
                auto value = new std::vector< std::thread::id >( 5, std::this_thread::get_id() );
                allocations.emplace_back(
                            std::shared_ptr< std::vector< std::thread::id > > ( value ) );
            }
            thisParam->m_counters[ index ].fetch_add( 100, std::memory_order_relaxed );
        }

        // Publish.
        {

        }

        // Deallocate
        allocations.clear();
    }
}
