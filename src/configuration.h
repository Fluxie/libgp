#ifndef GP_CONFIGURATION_H
#define GP_CONFIGURATION_H

#include <cstddef>
#include <cstdint>

namespace gp
{
namespace configuration
{

    //! Defines the variable for storing the epoch used track the safe zoners for each thread.
    typedef uint64_t epoch_t;

    //! Defines the type for storing statistics.
    typedef size_t statistics;

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
