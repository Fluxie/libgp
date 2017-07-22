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

#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include "../../src/gp.h"

#include <string>
#include <tuple>
#include <variant>
#include <vector>

//! Represents custom data associated with specific allocation
class CustomData
{
public:

    //! Varint type holding the possible values.
    typedef std::variant< int64_t, int32_t > t_value;

    //! Named value.
    typedef std::tuple< std::string, t_value > t_named_value;

    //! Collection of named values.
    typedef std::vector< t_named_value > t_values;

public:

    CustomData();

    //! Appends the specified value.
    template< typename t_value >
    void append(
            std::string name,
            const t_value& value
    )
    { m_values.emplace_back( name, value ); }

    //! Beginning of the values.
    t_values::const_iterator begin() const { return m_values.begin(); }

    //! End of the values.
    t_values::const_iterator end() const { return m_values.end(); }

private:

    t_values m_values;  //!< The custom values.
};

#endif // CUSTOMDATA_H
