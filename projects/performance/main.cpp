#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

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
    unsigned int threads = std::thread::hardware_concurrency() / 3 - 1;
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

    // Return the result.
    return result;
}
