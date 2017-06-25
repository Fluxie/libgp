#ifndef IDATASOURCE_H
#define IDATASOURCE_H

#include "customdata.h"

//! Interface that descripes a
class IDataSource
{
public:

    IDataSource() {}

    virtual ~IDataSource() {}

    //! Gets the title of the data source.
    virtual std::string title() const = 0;

    //! Retrieves the number of allocations done so far.
    virtual double allocations() const = 0;

    //! Custom data.
    virtual CustomData data() const = 0;

};

#endif // IDATASOURCE_H
