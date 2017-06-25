#ifndef STMCPP_DETAIL_GARBAGE_POOL_H
#define STMCPP_DETAIL_GARBAGE_POOL_H

#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <tuple>
#include <unordered_set>
#include <vector>

#include "atomic_statistics.h"
#include "configuration.h"
#include "queued_item.h"
#include "statistics.h"

namespace gp
{

class garbage_pool_participant;

/*!
 * \brief The garbage_pool class
 * The objects are put to queue
 * The thread performing the cleanup moves the items from the queue to the actual pool.
 * The cleanup thread then proceeds to process the items in the pool.
 */
class garbage_pool : public std::enable_shared_from_this< garbage_pool >
{
public:

    //! Epoch data type.
    typedef gp::configuration::epoch_t epoch_t;

    //! Period for the cleanup routine.
    typedef std::chrono::duration< uint32_t, std::milli > cleanup_period_t;

private:

    // Groups of objects marked for deallocation.
    struct group
    {
        group(
                gp::configuration::epoch_t epoch,
                std::initializer_list< queued_item > items  //!< Items assigned for dellocation.
        ) :
            m_epoch( epoch ),
            m_items( items )
        {
        }

        group(
                gp::configuration::epoch_t epoch,
                std::vector< queued_item >&& items  //!< Items assigned for dellocation.
        ) :
            m_epoch( epoch )
        {
            m_items = std::move( items );
        }


        //! Deallocates all the items in this group.
        void dellocate()
        {
            // Deallocate.
            for( queued_item& qi : m_items )
                qi.m_deallocate( qi.m_object );
        }

        epoch_t m_epoch;  //!< Epoch when this group can be allocated.
        std::vector< gp::queued_item > m_items;  //!< Items assigned for dellocation.
    };

    typedef std::list< group > pool_t;

public:

    garbage_pool(
            cleanup_period_t cleanupPeriod
    );

    ~garbage_pool();

    //! Updates the epoch.
    epoch_t update_epoch() noexcept { return ++m_globalEpoch; }

    //! Gets the global epoch.
    epoch_t epoch() const noexcept { return m_globalEpoch; }

    //! Gets the last active epoch.
    epoch_t last_active() const noexcept { return m_lastActive; }

    statistics get_statistics() const noexcept { return gp::statistics( m_statistics ); }

    //! Registers a pariticpant to the pool.
    void register_participant(
            garbage_pool_participant* participant  //!< Registered participant.
    );

    //! Unregisters a pariticipant from the pool.
    void unregister_participant(
            garbage_pool_participant* participant  //!< Unregistered participant.
    );

    //! Accesses the global garbage pool.
    static std::shared_ptr< garbage_pool >& global();

// Functionality associated with the background task.
private:

    //! Initializes the global bool.
    static void initialize_global();

    //! Cleansup the global pool and updates the last active epoch.
    static void cleanup_routine(
            garbage_pool* thisPool
    );

    //! Updates epoch of the last active participant.
    void update_last_active();

    //! Dellocates all eglible items in the pool.
    void clean();

private:

    // Participants access pool's types.
    friend class garbage_pool_participant;

    std::atomic< epoch_t > m_globalEpoch;  //!< Current global epoch.
    std::atomic< epoch_t > m_lastActive;  //!< Epoch of the last active participant.

    std::mutex m_statisticsGuard;  //!< Protects access to the statistics.
    atomic_statistics m_statistics;  //!< Statistics collected from the participants.

    std::mutex m_poolGuard;
    pool_t m_pool;  //!< Objects marked for dellocation that were moved here from the local garbage pool participants.

    std::mutex m_registerGuard;  //!< Protects access to the register guard.
    std::unordered_set< garbage_pool_participant* > m_participants;  //!< Collection of threads registered for transaction framework.

    std::atomic< bool > m_continueCleanup;
    cleanup_period_t m_cleanupPeriod;
    std::thread m_cleaner;

    // Container for hosting the garbage pool.
    static std::once_flag s_poolInitialized;
    static std::shared_ptr< garbage_pool > s_globalPool;
};

}

#endif // GARBAGE_POOL_H
