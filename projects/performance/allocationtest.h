#ifndef ALLOCATIONTEST_H
#define ALLOCATIONTEST_H

#include <array>
#include <atomic>
#include <string>
#include <thread>
#include <vector>

#include "idatasource.h"

//! A base class for the allocation tests.
template< typename t_allocated_object >
class AllocationTest : public IDataSource
{
public:

    AllocationTest(
            std::string title  //!< Title for the test.
    );

    //! Starts allocations.
    void start(
            unsigned int threads
    );

    //! Stops Allocations.
    void stop();

// IDataSource
public:

    //! Gets the title of the data source.
    virtual std::string title() const { return m_title; }

    //! Gets the time when the test was started.
    virtual IDataSource::clock_t::time_point started() const { return m_started; }

    //! Retrieves the number of allocations done so far.
    virtual double allocations() const;

    //! Custom data.
    virtual CustomData data() const = 0;

// The actual test methods.
protected:

    //! Called when the test round begins.
    virtual void beginTestRound() = 0;

    //! Allocates the objects.
    virtual void allocate(
            int count,  //!< Number of objects to allocate.
            std::vector< t_allocated_object >& allocations
    ) = 0;

    //! Deallocates the objects.
    virtual void deallocate(
            std::vector< t_allocated_object >& allocations
    ) = 0;

    //! Called when the test round ends.
    virtual void endTestRound() = 0;

    //! Called when the test ends.
    virtual void endTest() = 0;

private:

    //! Algorithm for the test.
    static void allocationAlgorithm(
            AllocationTest* thisParam,
            size_t index
    );

private:

    std::string m_title;
    IDataSource::clock_t::time_point m_started;  //!< The start time of the test.
    std::array< std::atomic< uint32_t >, 32 > m_counters;  //!< Allocation counter for each thread.

    std::atomic< bool > m_running;
    std::vector< std::thread > m_allocators;  //!< Threads doing the work.
};

template< typename t_allocated_object >
AllocationTest< t_allocated_object >::AllocationTest(
        std::string title
) :
    m_title( title ),
    m_running( false )
{
    // Reset counters.
    for( std::atomic< uint32_t >& c : m_counters )
        c.store( 0 );
}

//! Starts allocations.
template< typename t_allocated_object >
void AllocationTest< t_allocated_object >::start(
        unsigned int threads
)
{
    // Start.
    m_running = true;
    m_started = IDataSource::clock_t::now();

    for( unsigned int t = 0; t < threads; ++t )
        m_allocators.emplace_back( allocationAlgorithm, this, t );
}

//! Stops Allocations.
template< typename t_allocated_object >
void AllocationTest< t_allocated_object >::stop()
{
    // Request stop.
    m_running = false;

    // Wait for the threads to stop.
    for( std::thread& t : m_allocators )
        t.join();
}

template< typename t_allocated_object >
double AllocationTest< t_allocated_object >::allocations() const
{
    // Calculate a sum of the results.
    // It does not need to be accurate => memory_order_relaxed.
    double result = 0.0;
    for( auto& counter : m_counters )
    {
        result += counter.load( std::memory_order_relaxed );
    }
    return result;
}

template< typename t_allocated_object >
void AllocationTest< t_allocated_object >::allocationAlgorithm(
        AllocationTest* thisParam,
        size_t index
)
{
    std::vector< t_allocated_object> allocations;
    std::atomic< uint32_t >& counter = thisParam->m_counters[ index ];
    while( thisParam->m_running )
    {
        thisParam->beginTestRound();

        // Allocate 100 objects.
        int allocate = 10;
        thisParam->allocate( allocate , allocations );
        counter.fetch_add( allocate, std::memory_order_relaxed  );

        // Publish.
        {

        }

        // Deallocate
        thisParam->deallocate( allocations );
    }
    thisParam->endTest();

    // Ensure other threads will observe the statistics correctly.
    std::atomic_thread_fence( std::memory_order_seq_cst );
}

#endif // ALLOCATIONTEST_H
