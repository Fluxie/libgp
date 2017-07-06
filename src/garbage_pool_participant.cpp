#include "garbage_pool_participant.h"
#include "garbage_pool.h"

namespace gp
{


garbage_pool_participant::garbage_pool_participant(
        std::shared_ptr< garbage_pool > pool
) :
    m_pool( pool ),
    m_oldestLocalEpoch( 0 ),
    m_localEpoch( 0 )
{
    m_pool->register_participant( this );

    safe_zone();
}

garbage_pool_participant::garbage_pool_participant() :
    m_pool( garbage_pool::global() ),
    m_localEpoch( 0 )
{
    m_pool->register_participant( this );

    safe_zone();
}


garbage_pool_participant::~garbage_pool_participant()
{
    // Clean everything we can.
    clean();

    // Handover the rest.
    m_pool->unregister_participant( this );    
}


void garbage_pool_participant::safe_zone()
{
    m_localEpoch = m_pool->epoch();
}

//! Attempts to clean the local pool.
void garbage_pool_participant::clean(
        cleanup mode
)
{
    // Update the last active epoch before cleanup?
    if( mode == cleanup::after_update )
        m_pool->update_last_active();

    // Check the olders epoch quickly.
    garbage_pool::epoch_t lastActive = m_pool->last_active();
    if( m_oldestLocalEpoch > lastActive )
        return;

    // Deallocate.
    size_t deallocated = m_localPool.deallocate( lastActive );
    m_statistics.deallocations_completed( deallocated );
    m_oldestLocalEpoch = lastActive;
}


thread_local garbage_pool_participant garbage_pool_participant::s_threadsPool;

}

