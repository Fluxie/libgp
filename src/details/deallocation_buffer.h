#ifndef DEALLOCATION_BUFFER_H
#define DEALLOCATION_BUFFER_H

#include "deallocation_group.h"
#include "../configuration.h"

#include <list>

namespace gp { namespace details
{


//! Circular buffer for managing the deallocations.
class alignas( gp::configuration::cache_line_size ) deallocation_buffer
{
private:

    //! Definition for a group buffer.
    typedef std::list< deallocation_group* > groups_t;

public:
    deallocation_buffer();

    ~deallocation_buffer();

    //! Appends new items for deallocation.
    void append(
            gp::configuration::epoch_t epoch,
            std::initializer_list< queued_item > items
    );

    //! Appends new items for deallocation.
    void append(
            gp::configuration::epoch_t epoch,
            std::vector< queued_item >&& items
    );

    //! Deallocates all groups in the buffer. Returns the number deallocations that took place.
    size_t deallocate(
            gp::configuration::epoch_t lastActive  //!< Epoch of the oldest active thread.
    );

private:

    //! Allocates a new group and moves the current group to the full group list.
    void rotate_group();

    //! Frees all groups in the given collection.
    static void freeGroups(
            groups_t&  groupsToFree
    );

private:

    deallocation_group* m_currentGroup;  //!< Array of groups waiting for deallocation.
    groups_t m_fullGroups;  //!< A collection of full groups.
    groups_t m_freeGroups;  //!< Groups that have been allocated earlier whih are now free.
};

}
}

#endif // DEALLOCATION_BUFFER_H
