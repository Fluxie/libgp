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
