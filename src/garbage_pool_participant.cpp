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

#include "garbage_pool_participant.h"
#include "garbage_pool.h"

namespace gp
{


garbage_pool_participant::garbage_pool_participant(
        std::shared_ptr< garbage_pool > pool
) :
    m_pool( pool ),
    m_oldestLocalEpoch( 0 ),
    m_localEpoch( 0 )
{
    m_pool->register_participant( this );

    safe_zone();
}

garbage_pool_participant::garbage_pool_participant() :
    m_pool( garbage_pool::global() ),
    m_localEpoch( 0 )
{
    m_pool->register_participant( this );

    safe_zone();
}


garbage_pool_participant::~garbage_pool_participant()
{
    // Clean everything we can.
    clean();

    // Handover the rest.
    m_pool->unregister_participant( this );    
}


void garbage_pool_participant::safe_zone()
{
    m_localEpoch = m_pool->epoch();
}

bool garbage_pool_participant::clean(
        cleanup mode
)
{
    // Update the last active epoch before cleanup?
    if( mode == cleanup::after_update )
        m_pool->update_last_active();

    // Check the olders epoch quickly.
    garbage_pool::epoch_t lastActive = m_pool->last_active();
    if( m_oldestLocalEpoch > lastActive )
        return m_statistics.all_deallocated();

    // Deallocate.
    size_t deallocated = m_localPool.deallocate( lastActive );
    m_statistics.deallocations_completed( deallocated );
    m_oldestLocalEpoch = lastActive;

    return m_statistics.all_deallocated();
}


thread_local garbage_pool_participant garbage_pool_participant::s_threadsPool;

}

