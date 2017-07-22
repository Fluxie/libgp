#ifndef STATISTICS_H
#define STATISTICS_H

#include "configuration.h"

namespace gp
{

class atomic_statistics;

//! A container for the statistics collected from the garbage pool.
class statistics
{
public:

    //! Initializes statistics from the internal statistics type.
    statistics(
            const atomic_statistics& as
    );

    //! Gets the number items queued for deallocation.
    gp::configuration::statistics_t queued_dellocations() const { return m_queuedDeallocations; }

    //! Gets the number of items deallocated.
    gp::configuration::statistics_t completed_dellocations() const { return m_completedDeallocations; }

private:

    gp::configuration::statistics_t m_queuedDeallocations;
    gp::configuration::statistics_t m_completedDeallocations;
};

}

#endif // STATISTICS_H
