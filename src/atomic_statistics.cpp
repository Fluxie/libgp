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

#include "atomic_statistics.h"
#include "statistics.h"

namespace gp
{

atomic_statistics::atomic_statistics() :
    m_queuedDeallocations( 0 ),
    m_completedDeallocations( 0 )
{

}

void atomic_statistics::reset(
        const statistics& s
)
{
    m_queuedDeallocations.store( s.queued_dellocations(), std::memory_order_relaxed  );
    m_completedDeallocations.store( s.completed_dellocations(), std::memory_order_relaxed  );
}

void atomic_statistics::fetch_add(
        const statistics& s
)
{
    m_queuedDeallocations.fetch_add( s.queued_dellocations(), std::memory_order_relaxed  );
    m_completedDeallocations.fetch_add( s.completed_dellocations(), std::memory_order_relaxed  );
}

}
