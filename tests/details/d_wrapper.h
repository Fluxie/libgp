#ifndef GP_TESTS_DETAILS_D_WRAPPER_H
#define GP_TESTS_DETAILS_D_WRAPPER_H

#include <utility>

namespace gp
{
namespace tests
{
namespace details
{

template< typename t_object >
class d_wrapper
{
public:

    template< typename... t_args >
    d_wrapper(
            t_args... args
    )
    {
        m_wrapped = new t_object( std::forward< t_args >( args )... );
    }

    //! Accesses the wrapped object.
    t_object* operator->() { return m_wrapped; }

    //! Deallocates this object.
    void deallocate() { delete this; }

private:

    //! Force deallocation with the method.
    ~d_wrapper()
    {
        delete m_wrapped;
    }

    t_object* m_wrapped;
};

}
}
}

#endif // GP_TESTS_DETAILS_D_WRAPPER_H
