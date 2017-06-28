#include "garbagepoolallocator.h"

#include "../../src/gp.h"

#include <atomic>


GarbagePoolAllocator::GarbagePoolAllocator(
        const std::string& title
) :
    AllocationTest< std::vector< std::thread::id >* >( title )
{
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

//! Called when the test round begins.
void GarbagePoolAllocator::beginTestRound()
{
    // Try cleanup.
    gp::safe_zone();
    gp::pool().clean();
}

//! Allocates the objects.
void GarbagePoolAllocator::allocate(
        int count,  //!< Number of objects to allocate.
        std::vector< std::vector< std::thread::id >* >& allocations
)
{
    for( int c = 0; c < count; c++ )
        allocations.emplace_back( new std::vector< std::thread::id >( 5, std::this_thread::get_id() ) );
}

//! Deallocates the objects.
void GarbagePoolAllocator::deallocate(
        std::vector< std::vector< std::thread::id >* >& allocations
)
{
    for( auto a : allocations )
        gp::pool().deallocate( a );
    allocations.clear();
}

//! Called when the test round ends.
void GarbagePoolAllocator::endTestRound()
{

}

//! Called when the test ends.
void GarbagePoolAllocator::endTest()
{
    gp::pool().clean( gp::garbage_pool_participant::cleanup::after_update );    
}

