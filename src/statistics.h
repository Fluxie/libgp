#ifndef STATISTICS_H
#define STATISTICS_H

#include "configuration.h"

namespace gp
{

class atomic_statistics;

class statistics
{
public:

    //! Initializes statistics from the internal statistics type.
    statistics(
            const atomic_statistics& as
    );

    gp::configuration::statistics queued_dellocations() const { return m_queuedDeallocations; }

    gp::configuration::statistics completed_dellocations() const { return m_completedDeallocations; }

private:

    gp::configuration::statistics m_queuedDeallocations;
    gp::configuration::statistics m_completedDeallocations;
};

}

#endif // STATISTICS_H
