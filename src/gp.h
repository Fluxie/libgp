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

#ifndef GP_H
#define GP_H

#include "garbage_pool_participant.h"
#include "statistics.h"

namespace gp
{

    //! Gets the pool associated with the current thread.
    inline garbage_pool_participant& pool() { return garbage_pool_participant::current(); }

    //! Unregisters the current thread from the garbage pool.
    inline void unregister_thread() { garbage_pool::global()->unregister_participant( &pool() ); }

    //! Registers the current thread for the garbage pool.
    inline void register_thread() { garbage_pool::global()->register_participant( &pool() ); }

    //! The thread has reached a safe-zone and is not holding any references to dynamically allocated items.
    inline void safe_zone() { pool().safe_zone(); }

    //! Gets statistics of the current thread.
    inline statistics get_statistics() { return garbage_pool::global()->get_statistics(); }
}

#endif // GP_H
