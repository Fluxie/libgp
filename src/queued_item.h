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

#ifndef GP_QUEUED_ITEM_H
#define GP_QUEUED_ITEM_H

#include <type_traits>
#include <xmmintrin.h>

#include "details/has_deallocate.h"

namespace gp
{

//! Represents item queued for dellocation.
struct queued_item
{
    //! Function for deallocation an object of the specified type.
    template< typename t_item >
    static constexpr void deallocate_object( void* d )
    {
        static_cast< t_item* >( d )->deallocate();
    }

    //! Function for deleting an object of the specified type.
    template< typename t_item >
    static constexpr void delete_object( void* d )
    {
        delete static_cast< t_item* >( d );
    }

    //! Queues an item with deallocate method.
    template< typename t_item >
    static auto create(
            t_item* item,
            typename std::enable_if< gp::details::has_deallocate< t_item, void >::value >::type* = 0
    )
    {
        return queued_item( item, &queued_item::deallocate_object< t_item > );
    }

    //! Queues an item without deallocate method using regular delete.
    template< typename t_item, typename = std::enable_if_t< gp::details::has_deallocate< t_item, void >::value == false > >
    static auto create(
            t_item* item
    )
    {
        return queued_item( item, &queued_item::delete_object< t_item > );
    }

    queued_item() :
        m_object( nullptr ),
        m_deallocate( nullptr )
    {}

    constexpr queued_item(
            void* object,  //!< Object which is queued for dellocation
            void ( *deallocate )( void* )  //!< Function which dellocates the object.
    ) :
        m_object( object ),
        m_deallocate( deallocate )
    {
    }

    void* m_object;  //!< Object which is queued for dellocation
    void ( *m_deallocate )( void* );  //!< Function which dellocates the object.   
};


}

#endif // QUEUED_ITEM_H
