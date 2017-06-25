#include "plainallocator.h"



PlainAllocator::PlainAllocator(
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
void PlainAllocator::start(
        unsigned int threads
)
{
    // Start.
    m_running = true;

    for( unsigned int t = 0; t < threads; ++t )
        m_allocators.emplace_back( allocationAlgorithm, this, t );
}


//! Stops Allocations.
void PlainAllocator::stop()
{
    // Request stop.
    m_running = false;

    // Wait for the threads to stop.
    for( std::thread& t : m_allocators )
        t.join();
}

double PlainAllocator::allocations() const
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

CustomData PlainAllocator::data() const
{
    // Collect custom data.
    return CustomData();
}

void PlainAllocator::allocationAlgorithm(
        PlainAllocator* thisParam,
        size_t index
)
{
    std::vector< std::vector< std::thread::id >* > allocations;
    while( thisParam->m_running )
    {

        // Allocate 100 objects.
        {
            for( int c = 0; c < 100; c++ )
                allocations.emplace_back( new std::vector< std::thread::id >( 5, std::this_thread::get_id() ) );
            thisParam->m_counters[ index ].fetch_add( 100, std::memory_order_relaxed );
        }

        // Publish.
        {

        }

        // Deallocate
        for( std::vector< std::thread::id >* a : allocations )
            delete a;
        allocations.clear();
    }
}
