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

#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include <iostream>
#include <thread>
#include <malloc.h>

#include "datamodel.h"
#include "datamonitor.h"
#include "datapacket.h"
#include "garbagepoolallocator.h"
#include "plainallocator.h"
#include "referencecounter.h"
#include "../../src/gp.h"

using namespace std::chrono_literals;

int main(int argc, char *argv[])
{
    gp::unregister_thread();

    bool gui = true;
//    int arenas = mallopt( M_ARENA_MAX, 1 );
//    if( arenas == 0 )
//        std::terminate();

    GarbagePoolAllocator gpa1( "qsbr-1" );
    PlainAllocator plain( "plain" );
    ReferenceCounter ref( "ref" );
    if( gui )
    {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
        QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
        QCoreApplication::setAttribute( Qt::AA_X11InitThreads );
    }

    QApplication app(argc, argv);

    // Prepare monitor for the GUI?
    DataMonitor monitor;
    DataModel model;
    if( gui )
    {
        // DataMonitor provides real-time access to the performance metrics.
        monitor.registerSource( &gpa1 );
        monitor.registerSource( &plain );
        monitor.registerSource( &ref );

        monitor.connect( &monitor, &DataMonitor::capturedDataChanged,
                           &model, &DataModel::setCapturedData );
    }

    // Start test.
    unsigned int threadsForTesting = ( std::thread::hardware_concurrency() - 2 ) / 3;
    unsigned int threadsForProfiling = 4;
    unsigned int oneThread = 1;

    unsigned int threads = threadsForTesting;
    gpa1.start( threads );
    plain.start( threads );
    ref.start( threads );

    // Display GUI?
    int result = 0;
    if( gui )
    {
        monitor.start();

        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty( "dataMonitor", &monitor );
        engine.rootContext()->setContextProperty( "dataModel", &model );
        qmlRegisterType< DataPacket >( "fluxie.gp.performance", 1, 0, "DataPacket" );
    //    qmlRegisterInterfaceType< DataPacket >( "fluxie.gp.performance", 1, 0, "DataPacket" );
        engine.load(QUrl(QLatin1String("qrc:/main.qml")));
        if( engine.rootObjects().isEmpty() )
            return -1;

        // Show GUI.
        result = app.exec();
    }
    else   
        std::this_thread::sleep_for( 15s );

    // Stop to monitor and the algorithm.
    if( gui )
        monitor.stop();
    gpa1.stop();
    plain.stop();
    ref.stop();

    // Ensure we will observe the statistics correctly in this thread.
    std::this_thread::sleep_for( 2s );

    // Print statistics.
    gp::statistics s = gp::get_statistics();
    std::cout << "Completed: " << s.completed_dellocations() << std::endl;

    // Return the result.
    return result;
}
