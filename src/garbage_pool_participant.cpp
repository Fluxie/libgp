#include "garbage_pool_participant.h"
#include "garbage_pool.h"

namespace gp
{


garbage_pool_participant::garbage_pool_participant(
        std::shared_ptr< garbage_pool > pool
) :
    m_pool( pool ),
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
size_t garbage_pool_participant::clean(
        cleanup mode
)
{
    // Update the last active epoch before cleanup?
    if( mode == cleanup::after_update )
        m_pool->update_last_active();

    // Clean all entries in the pool that are older than the last active epoch.
    // TODO: The groups in the pool are naturally ordered, no need to loop through them all.
    garbage_pool::epoch_t lastActive = m_pool->last_active();
    for( pool_t::iterator group = m_localPool.begin(); group != m_localPool.end();  )
    {
        if( group->m_epoch <= lastActive )
        {
            m_statistics.deallocations_completed( group->m_items.size() );
            group->dellocate();
            group = m_localPool.erase( group );
        }
        else
            break;
    }
    return m_localPool.size();
}


thread_local garbage_pool_participant garbage_pool_participant::s_threadsPool;

}

