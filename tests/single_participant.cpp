/*
* libgp - A library implementing Quiescent State Based Reclamanation
* Copyright (C) 2017  Juha Lepola
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    bool allDeallocated = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
    QVERIFY( allDeallocated );
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
    bool allDeallocated = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
    QVERIFY( allDeallocated );
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
    bool allDeallocated = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
    QVERIFY( allDeallocated );
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
    bool allDeallocated = gp::pool().clean( garbage_pool_participant::cleanup::after_update );
    QVERIFY( allDeallocated );
}

}
}


