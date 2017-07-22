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
