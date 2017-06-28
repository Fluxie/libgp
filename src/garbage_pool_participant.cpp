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
size_t garbage_pool_participant::clean(
        cleanup mode
)
{
    // Update the last active epoch before cleanup?
    if( mode == cleanup::after_update )
        m_pool->update_last_active();

    // Check the olders epoch quickly.
    garbage_pool::epoch_t lastActive = m_pool->last_active();
    if( m_oldestLocalEpoch > lastActive )
        return m_localPool.size();

    // Clean all entries in the pool that are older than the last active epoch.
    // TODO: The groups in the pool are naturally ordered, no need to loop through them all.    
    for( pool_t::iterator itr = m_localPool.begin(); itr != m_localPool.end();  )
    {
        group_t& group = *itr;
        if( group.epoch() <= lastActive )
        {
            m_statistics.deallocations_completed( group.queued() );
            group.dellocate();
            m_cachedGroups.push_back( std::move( group ) );
            itr = m_localPool.erase( itr );
        }
        else
        {
            m_oldestLocalEpoch = group.epoch();
            break;
        }
    }
    return m_localPool.size();
}


thread_local garbage_pool_participant garbage_pool_participant::s_threadsPool;

}

