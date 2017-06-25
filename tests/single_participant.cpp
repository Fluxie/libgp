#include "single_participant.h"

#include "../src/gp.h"

#include "details/d_wrapper.h"

using gp::tests::details::d_wrapper;

namespace gp
{
namespace tests
{

void single_participant::deallocatingOneObjectSucceeds()
{
    {
        // Create an object.
        d_wrapper< std::string >* hello = new d_wrapper< std::string >( "Hello" );

        // Mark object for deallocation.
        gp::pool().deallocate( hello );
    }

    // Checkpoint.
    gp::pool().safe_zone();

    // Clean.
    size_t remaining = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
    QVERIFY( remaining  == 0 );
}


void single_participant::deletingOneObjectSucceeds()
{
    {
        // Create an object.
        std::string* hello = new std::string( "Hello" );

        // Mark object for deallocation.
        gp::pool().deallocate( hello );
    }

    // Checkpoint.
    gp::pool().safe_zone();

    // Clean.
    size_t remaining = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
    QVERIFY( remaining  == 0 );
}

//! Verifies that deleting multiple objects succeeds.
void single_participant::deletingMultipleObjectsSucceeds()
{
    {
        // Create an object.
        std::string* hello1 = new std::string( "Hello" );
        std::string* hello2 = new std::string( "Hello" );

        // Mark object for deallocation.
        gp::pool().deallocate( hello1, hello2 );
    }

    // Checkpoint.
    gp::pool().safe_zone();

    // Clean.
    size_t remaining = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
    QVERIFY( remaining  == 0 );
}

//! Verifies that mixing deallocation types succeeds.
void single_participant::mixingDeallocationTypesSucceeds()
{
    {
        // Create an objects.
        d_wrapper< std::string >* wrappedHello = new d_wrapper< std::string >( "Hello" );
        std::string* hello = new std::string( "Hello" );

        // Mark object for deallocation.
        gp::pool().deallocate( wrappedHello, hello );
    }

    // Checkpoint.
    gp::pool().safe_zone();

    // Clean.
    size_t remaining = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
    QVERIFY( remaining  == 0 );
}

}
}


