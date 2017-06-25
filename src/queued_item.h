#ifndef GP_QUEUED_ITEM_H
#define GP_QUEUED_ITEM_H

#include <type_traits>

#include "details/has_deallocate.h"

namespace gp
{

//! Represents item queued for dellocation.
struct queued_item
{

    //! Queues an item with deallocate method.
    template< typename t_item >
    static auto create(
            t_item* item,
            typename std::enable_if< gp::details::has_deallocate< t_item, void >::value >::type* = 0
    )
    {
        return queued_item( item, [] ( void* d ) -> void { static_cast< t_item* >( d )->deallocate(); } );
    }

    //! Queues an item with deallocate method.
    template< typename t_item, typename = std::enable_if_t< gp::details::has_deallocate< t_item, void >::value == false > >
    static auto create(
            t_item* item
    )
    {
        return queued_item( item, [] ( void* d ) -> void { delete static_cast< t_item* >( d ); } );
    }

//    //! Queues an item with deallocate method.
//    template< typename t_item >
//    static queued_item create(
//            t_item* item,
//            std::enable_if< gp::details::has_deallocate< t_item, void >::value >::type
//    )
//    {
//        if( gp::details::has_deallocate< t_item, void >::value )
//            return queued_item( item, [] ( void* d ) -> void { static_cast< t_item* >( d )->deallocate(); } );
//        else
//            return queued_item( item, [] ( void* d ) -> void { static_cast< t_item* >( d )->~t_item(); } );
//    }

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
