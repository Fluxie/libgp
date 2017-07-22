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

#include "datamonitor.h"
#include "idatasource.h"

#include <chrono>


DataMonitor::DataMonitor( QObject *parent ) : QObject( parent )
{
    // Start timer for reading data.
    m_monitorer = new QTimer();
    //m_monitorer->moveToThread( &m_monitorThread );
    connect( m_monitorer, &QTimer::timeout, this, &DataMonitor::readData );
}
//! List of available sources.
QStringList DataMonitor::sources() const
{
    // Collect the sources.
    QStringList sourcesTitles;
    for( IDataSource* source : m_sources )
    {
        sourcesTitles.append( QString( source->title().c_str() ) );
    }
    return sourcesTitles;
}

QList<QObject*> DataMonitor::data() const
{
    return m_capturedData;
}

void DataMonitor::start()
{
    m_monitorThread.start();
    m_monitorer->start( 500 );
}

void DataMonitor::stop()
{
    m_monitorer->stop();
    m_monitorThread.quit();
    m_monitorThread.wait();
}

void DataMonitor::readData()
{
    // Collect previous data.
    QList< QObject* > previousCapture;
    previousCapture.swap( m_capturedData );

    // Capture data from the sources.
    IDataSource::clock_t::duration current = IDataSource::clock_t::now().time_since_epoch();
    int64_t currentInMilliseconds = (int64_t) ( current.count() / ( ( double ) IDataSource::clock_t::duration::period::den / ( double ) std::chrono::milliseconds::period::den ) );
    for( IDataSource* source : m_sources )
    {
        // Capture.
        DataPacket* packet = new DataPacket();
        packet->setTitle( source->title().c_str() );
        IDataSource::clock_t::duration started = source->started().time_since_epoch();
        int64_t startedInMilliseconds = (int64_t) ( started.count() / ( ( double ) IDataSource::clock_t::duration::period::den / ( double ) std::chrono::milliseconds::period::den ) );
        packet->setStarted( startedInMilliseconds );
        packet->setCurrent( currentInMilliseconds );
        packet->setAllocations( source->allocations() );
        packet->setCustomData( source->data() );
        m_capturedData.push_back( packet );
    }

    // Deallocate previous captures.
    for( QObject* previous : previousCapture )
        previous->deleteLater();

    // We have changed the data.
    emit dataChanged();
    emit capturedDataChanged( m_capturedData );
}
