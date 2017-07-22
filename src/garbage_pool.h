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
#include "details/deallocation_buffer.h"
#include "details/deallocation_group.h"
#include "queued_item.h"
#include "statistics.h"

namespace gp
{

class garbage_pool_participant;

//! Main garbage pool that coordinates the efforts of the participants.
class garbage_pool : public std::enable_shared_from_this< garbage_pool >
{
public:

    //! Epoch data type.
    typedef gp::configuration::epoch_t epoch_t;

    //! Period for the cleanup routine.
    typedef std::chrono::duration< uint32_t, std::milli > cleanup_period_t;

public:

    //! Initializes a new garbage pool with the given cleanup period.
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

    //! Gets combined statistics from all participants. The combined statistics are update periodically.
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
    atomic_statistics m_retiredStatistics;  //!< Statistics collected from the retired participants.

    std::mutex m_poolGuard;
    gp::details::deallocation_buffer m_globalPool;  //!< Objects marked for dellocation that were moved here from the local garbage pool participants.

    std::mutex m_registerGuard;  //!< Protects access to the participant register.
    std::unordered_set< garbage_pool_participant* > m_participants;  //!< Collection of threads registered for transaction framework.

    std::atomic< bool > m_continueCleanup;  //!< True while the cleanup should running.
    cleanup_period_t m_cleanupPeriod;  //!< Time period for the cleanup routine.
    std::thread m_cleaner;  //!< A background thread responsible for cleaning objects of retired participants and updating the global epoch.

    // Container for hosting the garbage pool.
    static std::once_flag s_poolInitialized;
    static std::shared_ptr< garbage_pool > s_globalPool;
};

}

#endif // GARBAGE_POOL_H
