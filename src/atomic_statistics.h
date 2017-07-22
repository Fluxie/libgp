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

#ifndef ATOMIC_STATISTICS_H
#define ATOMIC_STATISTICS_H

#include "configuration.h"

#include <atomic>

namespace gp { class statistics; }

namespace gp
{

//! Collects statistics about the allocations and deallocations
class atomic_statistics
{
public:

    atomic_statistics();

    //! Reports queued deallocations.
    void deallocations_queued(
            gp::configuration::statistics_t deallocations
    )
    {
        // Add, we do not care about accuracy here, only speed.
        m_queuedDeallocations.fetch_add( deallocations, std::memory_order_relaxed );
    }

    //! Reports completed deallocations. Returns true if the queued objects have been deallocated.
    void deallocations_completed(
            gp::configuration::statistics_t deallocations
    )
    {
        // Add, we do not care about accuracy here, only speed.
        m_completedDeallocations.fetch_add( deallocations, std::memory_order_relaxed );
    }

    //! Resets the statistics from the given set of other statistics.
    void reset(
            const statistics& s
    );

    //! Updates the statistics from the given statistics collection.
    void fetch_add(
            const statistics& s
    );

    //! Checks if all queued objects have been deallocated.
    bool all_deallocated() const noexcept
    {
        bool allDeallocated =
                ( m_queuedDeallocations.load( std::memory_order_relaxed ) - m_completedDeallocations.load( std::memory_order_relaxed ) ) == 0;
        return allDeallocated;
    }

private:

    friend class statistics;

    std::atomic< gp::configuration::statistics_t > m_queuedDeallocations;
    std::atomic< gp::configuration::statistics_t > m_completedDeallocations;
};

}

#endif // ATOMIC_STATISTICS_H
