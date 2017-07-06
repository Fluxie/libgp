#ifndef GP_DETAILS_DEALLOCATION_GROUP_HEADER_H
#define GP_DETAILS_DEALLOCATION_GROUP_HEADER_H

#include "../configuration.h"

#include <cassert>

namespace  gp { namespace details
{

//! Header for the deallocation group.
class deallocation_group_header
{
public:

    deallocation_group_header() noexcept :
        m_epoch( 0 ),
        m_currentSlot( -1 )
    {}

    //! Epoch.
    gp::configuration::epoch_t epoch() const noexcept { return m_epoch; }

    //! Epoch.
    void set_epoch(
            gp::configuration::epoch_t epoch
    ) noexcept { m_epoch = epoch; }

    //! Returns the number of stored items in the group.
    size_t size() const noexcept { return m_currentSlot + 1; }

    //! Current slot.
    int slot() const noexcept { return m_currentSlot; }

    //! Advances the current slot and returns the new slot.
    int next() noexcept { assert( m_currentSlot >= -1 ); return ++m_currentSlot; }

    //! Clears the items.
    void clear() noexcept { m_currentSlot = -1; }

private:

    gp::configuration::epoch_t m_epoch;  //!< The epoch of the youngest item in the group.
    int m_currentSlot;  //!< The next free slot.
};

}
}

#endif // GP_DETAILS_DEALLOCATION_GROUP_HEADER_H