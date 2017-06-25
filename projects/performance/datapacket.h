#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>

#include "customdata.h"

class DataPacket : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title WRITE setTitle NOTIFY titleChanged )
    Q_PROPERTY( double allocations READ allocations WRITE setAllocations NOTIFY allocationsChanged )
    Q_PROPERTY( QVariantMap customData READ customData NOTIFY customDataChanged )
public:

    explicit DataPacket(QObject *parent = nullptr);

    //! Initializes a copy of an existing packet.
    explicit DataPacket(
            const DataPacket* source,
            QObject *parent = nullptr
    );

    //! Initializes the
    void initialize();

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

    //! Gets the title.
    QString title() const { return m_title; }

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
    void allocationsChanged();
    void customDataChanged();

public slots:

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

    QString m_title;
    double m_allocations;
    QVariantMap m_customData;
};

#endif // DATAPACKET_H
