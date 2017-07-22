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

#ifndef STATISTICS_H
#define STATISTICS_H

#include "configuration.h"

namespace gp
{

class atomic_statistics;

//! A container for the statistics collected from the garbage pool.
class statistics
{
public:

    //! Initializes statistics from the internal statistics type.
    statistics(
            const atomic_statistics& as
    );

    //! Gets the number items queued for deallocation.
    gp::configuration::statistics_t queued_dellocations() const { return m_queuedDeallocations; }

    //! Gets the number of items deallocated.
    gp::configuration::statistics_t completed_dellocations() const { return m_completedDeallocations; }

private:

    gp::configuration::statistics_t m_queuedDeallocations;
    gp::configuration::statistics_t m_completedDeallocations;
};

}

#endif // STATISTICS_H
