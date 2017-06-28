#include "plainallocator.h"



PlainAllocator::PlainAllocator(
        const std::string& title
) :
    AllocationTest( title )
{
}

CustomData PlainAllocator::data() const
{
    // Collect custom data.
    return CustomData();
}

//! Called when the test round begins.
void PlainAllocator::beginTestRound()
{

}

//! Allocates the objects.
void PlainAllocator::allocate(
        int count,  //!< Number of objects to allocate.
        std::vector< std::vector< std::thread::id >* >& allocations
)
{
    for( int c = 0; c < count; c++ )
        allocations.emplace_back( new std::vector< std::thread::id >( 5, std::this_thread::get_id() ) );
}

//! Deallocates the objects.
void PlainAllocator::deallocate(
        std::vector< std::vector< std::thread::id >* >& allocations
)
{
    for( std::vector< std::thread::id >* a : allocations )
        delete a;
    allocations.clear();
}

//! Called when the test round ends.
void PlainAllocator::endTestRound()
{

}

//! Called when the test ends.
void PlainAllocator::endTest()
{

}
