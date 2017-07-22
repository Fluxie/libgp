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

#ifndef MULTIPLE_PARTICIPANTS_H
#define MULTIPLE_PARTICIPANTS_H

#include <QtTest>

namespace gp
{
namespace tests
{

class multiple_participants : public QObject
{
    Q_OBJECT
public:
    explicit multiple_participants(QObject *parent = nullptr);

signals:

private slots:

    //! Verifies that multiple participants can deallocate objects.
    void deallocationSucceedsFromMultipleThreadsSucceeds();

    //! Verifies that deallocating objects after they have been shared between threads succeeeds.
    void deallocationAfterExchangingObjectsBetweenThreadsSucceeds();

};

}
}
#endif // MULTIPLE_PARTICIPANTS_H
