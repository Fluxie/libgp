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

#ifndef GP_CONFIGURATION_H
#define GP_CONFIGURATION_H

#include <cstddef>
#include <cstdint>

namespace gp
{
namespace configuration
{

    //! The assumed size of a cache line in the processor.
    static const size_t cache_line_size = 64;

    //! TThe size of a single deallocation group in bytes.
    static const size_t deallocation_group_size = 4096;

    //! Defines the variable for storing the epoch used track the safe zoners for each thread.
    typedef int64_t epoch_t;

    //! Defines the type for storing statistics.
    typedef int64_t statistics_t;

    //! Defines how the garbage pools are managed.
    enum class hosting_method : std::size_t
    {
        invisible,  //!< The garbage pool is available
        manual,  //!< The garbage pool and the garbage pool participants must be creted manually.
    };

    //! Defines the hosting method.
    constexpr gp::configuration::hosting_method selected_container() { return hosting_method::manual; }
}
}

#endif // CONFIGURATION_H
