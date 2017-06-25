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
