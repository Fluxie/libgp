#include "statistics.h"
#include "atomic_statistics.h"

#include <atomic>

namespace gp
{

statistics::statistics(
        const atomic_statistics& as
) :
    m_queuedDeallocations( as.m_queuedDeallocations.load( std::memory_order_relaxed )),
    m_completedDeallocations( as.m_completedDeallocations.load( std::memory_order_relaxed ) )
{

}

}
