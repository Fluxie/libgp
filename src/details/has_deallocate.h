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
