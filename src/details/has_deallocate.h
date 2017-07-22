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

#ifndef GP_DETAILS_HAS_DEALLOCATE_H
#define GP_DETAILS_HAS_DEALLOCATE_H

#include <type_traits>

namespace gp
{
namespace details
{

//! Tests whether the type t_object has a method t_return deallocate().
template< typename t_object, typename t_return >
struct has_deallocate
{
    //! Tests the signature of the deallocate.
    template< typename t_signature >
    static std::true_type test( t_return ( t_signature::* )() )
    {
        return std::true_type();
    }

    //! Tests the existance of the deallocate method.
    template< typename t_exists >
    static decltype( test( &t_exists::deallocate ) )
    test( decltype( &t_exists::deallocate ), void* )
    {
        typedef decltype( test( &t_exists::deallocate ) ) return_type;
        return return_type();
    }

    //! Does not exist.
    template< typename t_nomethod >
    static std::false_type test( ... )
    {
        return std::false_type();
    }

    //
    typedef decltype( test< t_object >( 0, 0 ) ) result;

    static const bool value = result::value;

};


}

}

#endif // HAS_DEALLOCATE_H
