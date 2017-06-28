#ifndef REFERENCECOUNTER_H
#define REFERENCECOUNTER_H


#include "allocationtest.h"

class ReferenceCounter: public AllocationTest< std::shared_ptr< std::vector< std::thread::id > > >
{
public:

    ReferenceCounter(
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
            std::vector< std::shared_ptr< std::vector< std::thread::id > > >& allocations
    );

    //! Deallocates the objects.
    virtual void deallocate(
            std::vector< std::shared_ptr< std::vector< std::thread::id > > >& allocations
    );

    //! Called when the test round ends.
    virtual void endTestRound();

    //! Called when the test ends.
    virtual void endTest();

};

#endif // REFERENCECOUNTER_H
