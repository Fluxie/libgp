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

