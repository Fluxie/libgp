#ifndef IDATASOURCE_H
#define IDATASOURCE_H

#include <QDateTime>

#include <chrono>

#include "customdata.h"

//! Interface that descripes a
class IDataSource
{
public:

    //! The type of the clock the data sources use.
    typedef std::chrono::steady_clock clock_t;

public:

    IDataSource() {}

    virtual ~IDataSource() {}

    //! Gets the title of the data source.
    virtual std::string title() const = 0;

    //! Gets the time when the test was started.
    virtual clock_t::time_point started() const = 0;

    //! Retrieves the number of allocations done so far.
    virtual double allocations() const = 0;    

    //! Custom data.
    virtual CustomData data() const = 0;

};

#endif // IDATASOURCE_H
