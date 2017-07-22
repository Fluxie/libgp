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

