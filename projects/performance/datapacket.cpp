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
