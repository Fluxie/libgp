#include "referencecounter.h"

#include <memory>

ReferenceCounter::ReferenceCounter(
        const std::string& title
) :
    AllocationTest( title )
{
}


CustomData ReferenceCounter::data() const
{
    // Collect custom data.
    return CustomData();
}

//! Called when the test round begins.
void ReferenceCounter::beginTestRound()
{

}

//! Allocates the objects.
void ReferenceCounter::allocate(
        int count,  //!< Number of objects to allocate.
        std::vector< std::shared_ptr< std::vector< std::thread::id > > >& allocations
)
{
    for( int c = 0; c < count; c++ )
    {
        auto value = new std::vector< std::thread::id >( 5, std::this_thread::get_id() );
        allocations.emplace_back(
                    std::shared_ptr< std::vector< std::thread::id > > ( value ) );
    }
}

//! Deallocates the objects.
void ReferenceCounter::deallocate(
        std::vector< std::shared_ptr< std::vector< std::thread::id > > >& allocations
)
{
    allocations.clear();
}

//! Called when the test round ends.
void ReferenceCounter::endTestRound()
{

}

//! Called when the test ends.
void ReferenceCounter::endTest()
{

}

