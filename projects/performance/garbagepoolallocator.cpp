#include "garbagepoolallocator.h"

#include "../../src/gp.h"


GarbagePoolAllocator::GarbagePoolAllocator(
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
void GarbagePoolAllocator::start(
        unsigned int threads
)
{
    // Start.
    m_running = true;    

    for( unsigned int t = 0; t < threads; ++t )
        m_allocators.emplace_back( allocationAlgorithm, this, t );
}

//! Stops Allocations.
void GarbagePoolAllocator::stop()
{
    // Request stop.
    m_running = false;

    // Wait for the threads to stop.
    for( std::thread& t : m_allocators )
        t.join();
}

double GarbagePoolAllocator::allocations() const
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

CustomData GarbagePoolAllocator::data() const
{
    // Collect custom data.
    gp::statistics s = gp::get_statistics();
    CustomData data;
    data.append( "queued", s.queued_dellocations() );
    data.append( "completed", s.completed_dellocations() );
    return data;    // Reset counters.
}

void GarbagePoolAllocator::allocationAlgorithm(
        GarbagePoolAllocator* thisParam,
        size_t index
)
{
    std::vector< std::vector< std::thread::id >* > allocations;
    std::atomic< uint32_t >& counter = thisParam->m_counters[ index ];
    while( thisParam->m_running )
    {
        // Try cleanup.
        gp::safe_zone();
        gp::pool().clean();

        // Allocate 100 objects.

        {
            for( int c = 0; c < 100; c++ )
                allocations.emplace_back( new std::vector< std::thread::id >( 5, std::this_thread::get_id() ) );
            counter.fetch_add( 100, std::memory_order_relaxed  );
        }

        // Publish.
        {

        }

        // Deallocate
        gp::pool().deallocate( allocations );
        allocations.clear();
    }
    gp::pool().clean( gp::garbage_pool_participant::cleanup::after_update );
}
