#ifndef PLAINALLOCATOR_H
#define PLAINALLOCATOR_H

#include <array>
#include <atomic>
#include <string>
#include <thread>
#include <vector>

#include "allocationtest.h"

class PlainAllocator : public AllocationTest< std::vector< std::thread::id >* >
{
public:

    PlainAllocator(
            const std::string& title
    );

// IDataSource
public:

    //! Custom data.
    virtual CustomData data() const;

protected:

    //! Called when the test round begins.
    virtual void beginTestRound();

    //! Allocates the objects.
    virtual void allocate(
            int count,  //!< Number of objects to allocate.
            std::vector< std::vector< std::thread::id >* >& allocations
    );

    //! Deallocates the objects.
    virtual void deallocate(
            std::vector< std::vector< std::thread::id >* >& allocations
    );

    //! Called when the test round ends.
    virtual void endTestRound();

    //! Called when the test ends.
    virtual void endTest();

};

#endif // PLAINALLOCATOR_H
