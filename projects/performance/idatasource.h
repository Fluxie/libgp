/*
* libgp - A library implementing Quiescent State Based Reclamanation
* Copyright (C) 2017  Juha Lepola
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
