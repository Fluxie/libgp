#ifndef GP_DETAILS_DEALLOCATION_GROUP_HEADER_H
#define GP_DETAILS_DEALLOCATION_GROUP_HEADER_H

#include "../configuration.h"

#include <cassert>
#include <xmmintrin.h>

namespace  gp { namespace details
{

class deallocation_group;

//! Header for the deallocation group.
class deallocation_group_header
{
public:

    //! Initializes the group to empty state.
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

    //! Clears the items and sets the next group to be deallocated.
    void reset(
            deallocation_group* next
    ) noexcept
    {
        m_next = next;
        m_currentSlot = -1;
    }

    //! Prefetches the next group to cache.
    inline void prefetch_next()
    {
        _mm_prefetch( m_next, _MM_HINT_T1 );
    }

private:

    gp::configuration::epoch_t m_epoch;  //!< The epoch of the youngest item in the group.
    deallocation_group* m_next;  //!< Next group for deallocation.
    int m_currentSlot;  //!< The next free slot.
};

}
}

#endif // GP_DETAILS_DEALLOCATION_GROUP_HEADER_H
