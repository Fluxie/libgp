#ifndef GP_DEALLOCATION_GROUP_H
#define GP_DEALLOCATION_GROUP_H

#include "../configuration.h"
#include "../queued_item.h"

#include <memory>
#include <vector>

namespace  gp
{

namespace details
{

//! Represents a group of objects marked for deallocation.
class deallocation_group
{
private:

    //! The preferred number of items for each group.
    static const size_t preferred_maximum = 1000;

public:

    deallocation_group();

    //! Appends new items for deallocation. Returns true if the group becomes full.
    bool append(
            gp::configuration::epoch_t epoch,
            std::initializer_list< queued_item > items
    );

    //! Appends new items for deallocation. Returns true if the group becomes full.
    bool append(
            gp::configuration::epoch_t epoch,
            std::vector< queued_item >&& items
    );

    //! Gets the epoch of the youngest item in the group.
    gp::configuration::epoch_t epoch() const noexcept { return m_epoch; }

    //! Returns the number items queued for excution.
    size_t queued() const noexcept { return m_items.size(); }

    //! Deallocates all the items in this group.
    void dellocate()
    {
        // Deallocate.
        for( queued_item& qi : m_items )
            qi.m_deallocate( qi.m_object );
        m_items.clear();
    }

// Support moves.
public:

    deallocation_group(
            deallocation_group&&
    ) noexcept;

    deallocation_group& operator=(
            deallocation_group&&
    ) noexcept;

// Prevent copying.
public:

    //! Prevent copying.
    deallocation_group(
            const deallocation_group&
    ) = delete;

    //! Prevent regular assignment.
    deallocation_group& operator=(
            const deallocation_group&
    ) = delete;

private:

    gp::configuration::epoch_t m_epoch;  //!< The epoch of the youngest item in the group.
    std::vector< gp::queued_item > m_items;  //!< Items marked for dellocation.
};

}

}

#endif // GP_DEALLOCATION_GROUP_H
