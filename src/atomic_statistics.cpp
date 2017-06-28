#include "atomic_statistics.h"
#include "statistics.h"

namespace gp
{

atomic_statistics::atomic_statistics() :
    m_queuedDeallocations( 0 ),
    m_completedDeallocations( 0 )
{

}

void atomic_statistics::reset(
        const statistics& s
)
{
    m_queuedDeallocations.store( s.queued_dellocations(), std::memory_order_relaxed  );
    m_completedDeallocations.store( s.completed_dellocations(), std::memory_order_relaxed  );
}

void atomic_statistics::fetch_add(
        const statistics& s
)
{
    m_queuedDeallocations.fetch_add( s.queued_dellocations(), std::memory_order_relaxed  );
    m_completedDeallocations.fetch_add( s.completed_dellocations(), std::memory_order_relaxed  );
}

}
