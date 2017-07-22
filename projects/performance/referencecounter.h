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
