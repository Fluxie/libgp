#include "garbage_pool.h"
#include "garbage_pool_participant.h"


namespace gp
{

garbage_pool::garbage_pool(
        cleanup_period_t cleanupPeriod
) :
    m_globalEpoch( 0 ),
    m_lastActive( 0 ),
    m_continueCleanup( true ),
    m_cleanupPeriod( cleanupPeriod ),
    m_cleaner( garbage_pool::cleanup_routine, this )
{
}

garbage_pool::~garbage_pool()
{
    m_continueCleanup = false;
    m_cleaner.join();
}


//! Dellocates all eglible items in the pool
void garbage_pool::clean()
{
    // Move all items from the queue to the cleanup pool.
    {
        std::unique_lock< std::mutex >( m_queueGuard );

        // m_pool.splice( m_pool.begin(), m_queue );
    }

    // Perform the actual cleanup.

}

//! Registers a pariticpant to the pool.
void garbage_pool::register_participant(
        garbage_pool_participant* participant  //!< Registered participant.
)
{
    std::unique_lock<std::mutex> lock( m_registerGuard );

    m_participants.insert( participant );
}

//! Unregisters a pariticipant from the pool.
void garbage_pool::unregister_participant(
        garbage_pool_participant* participant  //!< Unregistered participant.
)
{
    std::unique_lock<std::mutex> lock( m_registerGuard );

    m_participants.erase( participant );

    // Update statistics.
    statistics s = participant->statistics();
    m_retiredStatistics.fetch_add( s );
}

//! Accesses the global garbage pool.
std::shared_ptr< garbage_pool >& garbage_pool::global()
{
    // Initialize the pool if not initialized.
    std::call_once( s_poolInitialized, initialize_global );

    // Return.
    return s_globalPool;
}

void garbage_pool::update_last_active()
{
    // Start from the global epoch.
    epoch_t lastActive = m_globalEpoch;

    // Determine the oldest epoch of the participants.
    {
        std::unique_lock<std::mutex> lock( m_registerGuard );

        m_statistics.reset( m_retiredStatistics );
        for( auto& participant : m_participants )
        {
            epoch_t participantEpoch = participant->epoch();
            if( participantEpoch < lastActive )
                lastActive = participantEpoch;

            // Update statistics.
            statistics s = participant->statistics();
            m_statistics.fetch_add( s );
        }
    }

    // Update the epoch of the last active participant.
    m_lastActive = lastActive;
}

void garbage_pool::initialize_global()
{
    s_globalPool = std::make_shared< garbage_pool >( std::chrono::milliseconds( 500 ) );
}

//! Cleansup the global pool and updates the last active epoch.
void garbage_pool::cleanup_routine(
        garbage_pool* thisPool
)
{
    while( thisPool->m_continueCleanup )
    {
        // Update the last active participant.
        thisPool->update_last_active();

        // Wait.
        std::this_thread::sleep_for( thisPool->m_cleanupPeriod );
    }
}

std::once_flag garbage_pool::s_poolInitialized;
std::shared_ptr< garbage_pool > garbage_pool::s_globalPool;

}

