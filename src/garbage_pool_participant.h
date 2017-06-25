#ifndef STMCPP_DETAIL_GARBAGE_POOL_PARTICIPANT_H
#define STMCPP_DETAIL_GARBAGE_POOL_PARTICIPANT_H

#include <atomic>
#include <deque>
#include <functional>
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <vector>

#include "garbage_pool.h"
#include "details/has_deallocate.h"
#include "statistics.h"
#include "atomic_statistics.h"

namespace gp
{

/*!
 * \brief The garbage_pool_participant class
 *
 *
 */
class garbage_pool_participant
{
    typedef std::deque< garbage_pool::group > pool_t;

public:

    //! Defines how the cleanup is performed.
    enum class cleanup
    {
        known,  //!< Cleans only objects immediately eglible for deallocation.
        after_update,  //!< Updates the last active epoch before attempting the cleanup. Forces synchronization with the cleanup thread.
    };

    //! Registers the participant
    garbage_pool_participant(
            std::shared_ptr< garbage_pool > pool
    );

    //! Unregisters the participant.
    ~garbage_pool_participant();

    //! Gets the current epoch.
    garbage_pool::epoch_t epoch() const noexcept { return m_localEpoch.load( std::memory_order_seq_cst ); }

    //! Updates the global epoch.
    garbage_pool::epoch_t update_epoch()
    { return m_pool->update_epoch(); }

    //! Notifies the garbage pool that the current thread cannot hold any references to the objects in the pool.
    void safe_zone();

    //! Deallocates the specified items using the default destructor.
    template< typename... t_item >
    void deallocate(
        t_item*... item
    )
    {
        // Convert to queued items.
       deallocate( { queued_item::create< t_item >( item )... } );
    }

    //! Deallocates the specified items using the default destructor.
    template< typename t_item >
    void deallocate(
        std::vector< t_item* >& items
    )
    {
        // Convert to queued items.
        std::vector< queued_item > queuedItems;
        queuedItems.reserve( items.size() );
        for( t_item* item :items )
            queuedItems.emplace_back( queued_item::create< t_item >( item ) );

        garbage_pool::epoch_t epoch = update_epoch();
        deallocate_items( epoch, std::move( queuedItems ) );
    }

    //! Marks the specified items for dellocation.
    void deallocate(
        std::initializer_list< queued_item > items
    )
    {
        // Delegate.
        garbage_pool::epoch_t epoch = update_epoch();
        deallocate_items( epoch, items );
    }

    //! Marks the specified items for dellocation.
    void deallocate_items(
        garbage_pool::epoch_t epoch,
        std::initializer_list< queued_item > items
    )
    {
        // Collect the items for dellocation.
        m_statistics.deallocations_queued( items.size() );
        m_localPool.emplace_back( epoch, items );        
    }

    //! Marks the specified items for dellocation.
    void deallocate_items(
        garbage_pool::epoch_t epoch,
        std::vector< queued_item >&& items
    )
    {
        // Collect the items for dellocation.
        m_statistics.deallocations_queued( items.size() );
        m_localPool.emplace_back( epoch, std::forward< std::vector< queued_item > >( items ) );        
    }

    //! Attempts to clean the local pool.
    size_t clean(
            cleanup mode = cleanup::known
    );

    //! Gets the statistics.
    gp::statistics statistics() const { return gp::statistics( m_statistics ); }

    //! Accesses the current pool.
    static garbage_pool_participant& current() { return s_threadsPool; }

private:

    //! Initializes the participant and associated the participant with the global pool.
    garbage_pool_participant();

    std::shared_ptr< garbage_pool > m_pool;  //!< Garbage pool this associated with the participant.

    std::atomic< garbage_pool::epoch_t > m_localEpoch;
    pool_t m_localPool;

    atomic_statistics m_statistics;  //!< Collects statistics about the usage.

    static thread_local garbage_pool_participant s_threadsPool;
};

}


#endif // GARBAGE_POOL_PARTICIPANT_H
