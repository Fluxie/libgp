#ifndef GARBAGEPOOLALLOCATOR_H
#define GARBAGEPOOLALLOCATOR_H

#include <array>
#include <atomic>
#include <string>
#include <thread>
#include <vector>

#include "idatasource.h"

//! Manages garbage pool allocation tests.
class GarbagePoolAllocator : public IDataSource
{
public:

    GarbagePoolAllocator(
            std::string title
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

    //! Retrieves the number of allocations done so far.
    virtual double allocations() const;

    //! Custom data.
    virtual CustomData data() const;

private:

    //! Algorithm for the test.
    static void allocationAlgorithm(
            GarbagePoolAllocator* thisParam,
            size_t index
    );

private:

    std::string m_title;
    std::array< std::atomic< uint32_t >, 32 > m_counters;  //!< Allocation counter for each thread.

    std::atomic< bool > m_running;
    std::vector< std::thread > m_allocators;  //!< Threads doing the work.
};

#endif // GARBAGEPOOLALLOCATOR_H
