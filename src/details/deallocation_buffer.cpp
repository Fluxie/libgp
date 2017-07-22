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

#include "deallocation_buffer.h"

#include "../configuration.h"

#include <cstring>
#include <cstdlib>
#include <vector>

namespace gp { namespace details {

deallocation_buffer::deallocation_buffer() :    
    m_currentGroup( nullptr )
{    
    m_currentGroup = ( gp::details::deallocation_group*  ) std::aligned_alloc( gp::configuration::cache_line_size, sizeof( gp::details::deallocation_group ) );
    if( m_currentGroup == nullptr )
        throw std::bad_alloc();
    new( m_currentGroup ) deallocation_group();
}

deallocation_buffer::~deallocation_buffer()
{
    free( m_currentGroup );
    for( deallocation_group* d : m_fullGroups )
        d->dellocate();
    freeGroups( m_fullGroups );
    freeGroups( m_freeGroups );
}

//! Appends new items for deallocation. Returns true if the group becomes full.
void deallocation_buffer::append(
        gp::configuration::epoch_t epoch,
        std::initializer_list< queued_item > items
)
{
    // Keep appending items until all the items have been added.
    std::initializer_list< queued_item >::iterator itr = items.begin();
    while( itr != items.end() )
    {
        // Append to current group.
        itr = itr + m_currentGroup->append( epoch, itr, items.end() );

        // Did we fill the group?
        if( itr != items.end() )
            rotate_group();

    }  // end whilebuffer_size
}

//! Appends new items for deallocation. Returns the number of appended
void deallocation_buffer::append(
        gp::configuration::epoch_t epoch,
        std::vector< queued_item >&& items
)
{
    // Keep appending items until all the items have been added.
    std::vector< queued_item >::iterator itr = items.begin();
    while( itr != items.end() )
    {
        // Append to current group.
        itr = itr + m_currentGroup->append( epoch, itr, items.end() );

        // Did we fill the group?
        if( itr != items.end() )
            rotate_group();

    }  // end while
}

size_t deallocation_buffer::deallocate(
        gp::configuration::epoch_t lastActive  //!< Epoch of the oldest active thread.
)
{
    // Clean all entries in the pool that are older than the last active epoch.
    size_t deallocations = 0;
    while( m_fullGroups.empty() == false )
    {
        groups_t::iterator groupToFree = m_fullGroups.begin();
        deallocation_group* group = (*groupToFree);
        if( group->epoch() <= lastActive )
        {
            deallocations += group->dellocate_full();
            m_freeGroups.splice( m_freeGroups.end(), m_fullGroups, groupToFree );
        }
        else
        {
            lastActive = group->epoch();
            break;
        }
    }

    // Try deallocating the current group?
    if( m_fullGroups.empty() && m_currentGroup->epoch() <= lastActive )
    {
        deallocations += m_currentGroup->dellocate();
    }

    return deallocations;
}

void deallocation_buffer::trim() noexcept
{
    freeGroups( m_freeGroups );
}

void deallocation_buffer::splice(
        deallocation_buffer* other
)
{
    // Move all groups that may have members to the target.
    m_fullGroups.splice( m_fullGroups.end(), other->m_fullGroups );
    m_freeGroups.splice( m_freeGroups.end(), other->m_freeGroups );
    append( other->m_currentGroup->epoch(), other->m_currentGroup->begin(), other->m_currentGroup->end() );

    other->m_fullGroups.clear();
    other->m_freeGroups.clear();
    other->m_currentGroup->reset( nullptr );
}

void deallocation_buffer::rotate_group()
{
    // The previous group is full.
    m_fullGroups.push_back( m_currentGroup );

    // Groups available for reuse?
    if( m_freeGroups.empty() )
    {
        // Prepare the new buffer.
        deallocation_group* newBuffer = ( gp::details::deallocation_group*  )
                    std::aligned_alloc( gp::configuration::cache_line_size, sizeof( gp::details::deallocation_group ) );
        if( newBuffer == nullptr )
            throw std::bad_alloc();
        new( newBuffer ) deallocation_group();

        // Replace the old buffer.
        m_currentGroup = newBuffer;
    }
    else
    {
        // We add groups to the back so take one from the back.
        m_currentGroup = m_freeGroups.back();
        m_freeGroups.pop_back();        
    }
    m_currentGroup->reset( m_fullGroups.back() );
}

//! Frees all groups in the given collection.
void deallocation_buffer::freeGroups(
        groups_t&  groupsToFree
)
{
    // Free the groups.
    for( deallocation_group* g : groupsToFree )
    {
        free( g );
    }
    groupsToFree.clear();
}

}
}
