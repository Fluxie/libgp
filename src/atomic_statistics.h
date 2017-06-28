#ifndef ATOMIC_STATISTICS_H
#define ATOMIC_STATISTICS_H

#include "configuration.h"

#include <atomic>

namespace gp { class statistics; }

namespace gp
{

//! Collects statistics of a pool
class atomic_statistics
{
public:

    atomic_statistics();

    //! Reports queued deallocations.
    void deallocations_queued(
            gp::configuration::statistics deallocations
    )
    {
        // Add, we do not care about accuracy here only speed.
        m_queuedDeallocations.fetch_add( deallocations, std::memory_order_relaxed );
    }

    //! Reports completed deallocations.
    void deallocations_completed(
            gp::configuration::statistics deallocations
    )
    {
        // Add, we do not care about accuracy here only speed.
        m_completedDeallocations.fetch_add( deallocations, std::memory_order_relaxed );
    }

    //! Resets the statistics from the given set of other statistics.
    void reset(
            const statistics& s
    );

    //! Updates the statistics from the given statistics collection.
    void fetch_add(
            const statistics& s
    );

private:

    friend class statistics;

    std::atomic< gp::configuration::statistics > m_queuedDeallocations;
    std::atomic< gp::configuration::statistics > m_completedDeallocations;
};

}

#endif // ATOMIC_STATISTICS_H
