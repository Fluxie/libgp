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

#include "datamodel.h"

#include <QJsonArray>
#include <QJsonObject>

const int DataModel::ControlRole = Qt::UserRole;
const int DataModel::ContentRole = Qt::UserRole + 1;

DataModel::DataModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // The model exposes a fixed number of items.
    return m_capturedData.size() + 1;
}

QHash< int, QByteArray > DataModel::roleNames() const
{
    QHash< int, QByteArray > roles;
    roles.insert( Qt::DisplayRole, QString( "title" ).toLatin1() );
    roles.insert( DataModel::ControlRole, QString( "control" ).toLatin1() );
    roles.insert( DataModel::ContentRole, QString( "content" ).toLatin1() );
    return roles;
}

QVariant DataModel::data( const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // Static
    switch( role )
    {
    case Qt::DisplayRole:
        return forDisplay( index );

    case DataModel::ControlRole :
        return forControl( index );

    case DataModel::ContentRole :
        return forContent( index );

    default:
        return QVariant();
    }
}

void DataModel::setCapturedData(
        const QList< QObject* >& capturedData
)
{
    // Inserting new rows?
    bool wasEmpty = m_capturedData.empty();
    if( wasEmpty )
        this->beginInsertRows( QModelIndex(), 0, 2 );

    m_capturedData.clear();
    for( QObject* packet : capturedData )
    {
        m_capturedData.append( new DataPacket( qobject_cast< DataPacket* >( packet ) ) );
    }

    // We updated the data.
    if( wasEmpty )
        this->endInsertRows();
    emit dataChanged( this->index( 0 ), this->index( m_capturedData.size() ) );
}

QVariant DataModel::forDisplay( const QModelIndex& index ) const
{
    if( index.row() == 0 )
        return QString( "Total Allocations" );
    else
        return QString( m_capturedData[ index.row() - 1 ]->title() );
}

QVariant DataModel::forControl( const QModelIndex& index ) const
{
    if( index.row() == 0 )
        return QString( "PieChart.qml" );
    else
        return QString( "Statistics.qml" );
}

QVariant DataModel::forContent( const QModelIndex& index ) const
{
    if( index.row() == 0 )
    {
        QJsonArray data;
        for( int i = 0; i < m_capturedData.size(); ++i )
        {
            QJsonObject slice;
            slice[ "label" ] = m_capturedData[ i ]->title();
            slice[ "value" ] = m_capturedData[ i ]->allocations();
            data.append( slice );
        }
        return data;
    }
    else
    {
        return QVariant::fromValue< DataPacket*>( m_capturedData[ index.row() - 1 ] );
    }

}
