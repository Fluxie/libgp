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

#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>

#include "customdata.h"

class DataPacket : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title WRITE setTitle NOTIFY titleChanged )
    Q_PROPERTY( double started READ started WRITE setStarted NOTIFY startedChanged )
    Q_PROPERTY( double current READ current WRITE setCurrent NOTIFY currentChanged )
    Q_PROPERTY( double allocations READ allocations WRITE setAllocations NOTIFY allocationsChanged )
    Q_PROPERTY( QVariantMap customData READ customData NOTIFY customDataChanged )
public:

    explicit DataPacket(
            QObject *parent = nullptr
    );

    //! Initializes a copy of an existing packet.
    explicit DataPacket(
            const DataPacket* source,
            QObject *parent = nullptr
    );

    //! Initializes the
    void initialize();

    //! Gets the title.
    QString title() const { return m_title; }

    //! Timepoint in milliseconds when the testing started
    double started() const { return m_started; }

    //! The current time in milliseconds, only usefule whe
    double current() const { return m_current; }

    double allocations() const
    {
        return m_allocations;
    }

    QVariantMap customData() const { return m_customData; }

    //! Sets the custom data.
    void setCustomData(
            const CustomData& data
    );

signals:

    void titleChanged();
    void startedChanged();
        void currentChanged();
    void allocationsChanged();
    void customDataChanged();

public slots:

    //! Sets the title.
    void setTitle(
            const QString& title
    )
    {
        if( m_title != title )
        {
            m_title = title;
            emit titleChanged();
        }
    }

    void setStarted(
            const double& started
    )
    {
        if( m_started != started )
        {
            m_started = started;
            emit startedChanged();
        }
    }

    void setCurrent(
            const double& current
    )
    {
        if( m_current != current )
        {
            m_current = current;
            emit currentChanged();
        }
    }

    void setAllocations(
            double value
    )
    {
        // Changed?
        if( value != m_allocations )
        {
            m_allocations = value;
            emit allocationsChanged();
        }
    }

// Support move.
public:

    DataPacket(
            DataPacket&& source
    );

    DataPacket& operator=(
            DataPacket&& source
    );

private:

    //! Converts std::varinat to QVarinat.
    template< typename t_std_variant >
    QVariant ToQtType(
            const t_std_variant& variant
    )
    {
        return std::visit( []( auto&& arg ) -> QVariant { return QVariant::fromValue( (double) arg ); }, variant );
    }

//    template< typename t_std_variant, size_t pos >
//    QVariant ToQtType(
//            const t_std_variant& variant
//    )
//    {
//        if( auto pval = std::get_if< pos >( variant ) )
//            return QVariant
//        else

//    }

private:

    QString m_title;  //!< The title of the data source that produced this packet.
    double m_started;  //!< When the data source was started.
    double m_current;  //!< The current time.
    double m_allocations;
    QVariantMap m_customData;
};

#endif // DATAPACKET_H
