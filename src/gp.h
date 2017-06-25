#ifndef GP_H
#define GP_H

#include "garbage_pool_participant.h"
#include "statistics.h"

namespace gp
{

    //! Gets the pool associated with the current thread.
    inline garbage_pool_participant& pool() { return garbage_pool_participant::current(); }

    //! Unregisters the current thread from the garbage pool.
    inline void unregister_thread() { garbage_pool::global()->unregister_participant( &pool() ); }

    //! Registers the current thread for the garbage pool.
    inline void register_thread() { garbage_pool::global()->register_participant( &pool() ); }

    //! The thread has reached a safe-zone.
    inline void safe_zone() { pool().safe_zone(); }

    //! Gets statistics of the current thread.
    inline statistics get_statistics() { return garbage_pool::global()->get_statistics(); }
}

#endif // GP_H
