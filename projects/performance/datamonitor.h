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

#ifndef DATAMONITOR_H
#define DATAMONITOR_H

#include <QList>
#include <QObject>
#include <QObjectList>
#include <QStringList>
#include <QTimer>
#include <QThread>

#include "datapacket.h"

class IDataSource;

class DataMonitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QStringList sources READ sources NOTIFY sourcesChanged )
    Q_PROPERTY( QList<QObject*> data READ data NOTIFY dataChanged )

public:

    explicit DataMonitor(QObject *parent = nullptr);

    //! Captures the current state.
    Q_INVOKABLE QList<QObject*> data() const;

    //! List of available sources.
    QStringList sources() const;

    //! Registers new source, the monitor must not be running.
    void registerSource(
        IDataSource* source
    )
    { m_sources.push_back( source ); }

    //! Starts to monitor the data.
    void start();

    //! Stops monitoring.
    void stop();

signals:

    void sourcesChanged();

    void dataChanged();

    void capturedDataChanged(
            const QList< QObject* >& capturedData
    );

public slots:    


private slots:

    //! Reads data from available data sources.
    void readData();

private:

    QList<QObject*> m_capturedData;  //!< Captured data.

    QThread m_monitorThread;  //!< Thread for the timer.
    QTimer* m_monitorer;  //!< Timer for updating allocations to the monitor.
    std::vector< IDataSource* > m_sources;
};

#endif // DATAMONITOR_H
