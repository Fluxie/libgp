#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "datapacket.h"

class DataModel : public QAbstractListModel
{
    Q_OBJECT

    //! The role id of the lookup role.
    static const int ControlRole;
    static const int ContentRole;

public:
    explicit DataModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash< int, QByteArray > roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:

    //! Sets captured data.
    void setCapturedData(
            const QList< QObject* >& capturedData
    );

private:

    //! Gets value for dispaly role.
    QVariant forDisplay( const QModelIndex &index ) const;

    //! Gets value for control role.
    QVariant forControl( const QModelIndex &index ) const;

    //! Gets value for content role.
    QVariant forContent( const QModelIndex &index ) const;


private:

    QList<DataPacket*> m_capturedData;  //!< Captured data.
};

#endif // DATAMODEL_H
