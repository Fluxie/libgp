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

#include "datapacket.h"

DataPacket::DataPacket( QObject *parent ) :
    QObject( parent ),
    m_started( 0 ),
    m_current( 0 ),
    m_allocations( 0 )
{

}

//! Initializes a copy of an existing packet.
DataPacket::DataPacket(
        const DataPacket* source,
        QObject* parent
) :
    QObject( parent ),
    m_title( source->m_title ),
    m_started( source->m_started ),
    m_current( source->m_current ),
    m_allocations( source->m_allocations ),
    m_customData( source->m_customData )
{

}

DataPacket::DataPacket(
        DataPacket&& source
) :
    QObject( source.parent() ),
    m_allocations( source.allocations() )
{

}

DataPacket& DataPacket::operator=(
        DataPacket&& source
)
{
    if( this != &source )
    {
        m_allocations = source.allocations();
    }
    return *this;
}

void DataPacket::setCustomData(
        const CustomData& data
)
{
    // Read the custom data into QVariantMap.
    m_customData.clear();
    for( const CustomData::t_named_value& value : data )
    {
        m_customData[ std::get< 0 >( value ).c_str() ] = ToQtType( std::get< 1 >( value ) );
    }
}
