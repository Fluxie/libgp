#include "deallocation_buffer.h"

#include "../configuration.h"

#include <cstring>
#include <cstdlib>
#include <vector>

namespace gp { namespace details {

deallocation_buffer::deallocation_buffer() :    
    m_currentGroup( nullptr )
{
//    size_t bufferSize = gp::configuration::initial_deallocation_buffer_size * sizeof( gp::details::deallocation_group );
    size_t bufferSize = gp::configuration::initial_deallocation_buffer_size * sizeof( gp::details::deallocation_group );
    m_currentGroup = ( gp::details::deallocation_group*  ) std::aligned_alloc( gp::configuration::cache_line_size, bufferSize );
    if( m_currentGroup == nullptr )
        throw std::bad_alloc();
    new( m_currentGroup ) deallocation_group();
}

deallocation_buffer::~deallocation_buffer()
{
    if( m_currentGroup != nullptr )
        free( m_currentGroup );
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
        gp::configuration::epoch_t& lastActive  //!< Epoch of the oldest active thread.
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

void deallocation_buffer::rotate_group()
{
    // The previous group is full.
    m_fullGroups.push_back( m_currentGroup );

    // Groups available for reuse?
    if( m_freeGroups.empty() )
    {
        // Prepare the new buffer.
        deallocation_group* newBuffer = ( gp::details::deallocation_group*  )
                    std::aligned_alloc( gp::configuration::cache_line_size, gp::configuration::deallocation_group_size * sizeof( gp::details::deallocation_group ) );
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
        m_currentGroup->reset();
    }
}

//! Frees all groups in the given collection.
void deallocation_buffer::freeGroups(
        groups_t  groupsToFree
)
{
    // Free the groups.
    for( deallocation_group* g : groupsToFree )
    {
        g->dellocate();
        free( g );
    }
    groupsToFree.clear();
}

}
}
