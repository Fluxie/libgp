#include "deallocation_group.h"

namespace gp
{
namespace details
{

deallocation_group::deallocation_group() :
    m_epoch( 0 )
{
    m_items.reserve( deallocation_group::preferred_maximum );
}

//! Appends new items for deallocation.
bool deallocation_group::append(
        gp::configuration::epoch_t epoch,
        std::initializer_list< queued_item > items
)
{
    // Collect.
    m_epoch = epoch;
    for( queued_item qi : items )
        m_items.push_back( std::move( qi ) );

    // Are we full?
    bool full = m_items.size() >= deallocation_group::preferred_maximum - 10;
    return full;
}

//! Appends new items for deallocation. Returns true if the group becomes full.
bool deallocation_group::append(
        gp::configuration::epoch_t epoch,
        std::vector< queued_item >&& items
)
{
    // Collect.
    m_epoch = epoch;
    for( queued_item& qi : items )
        m_items.push_back( std::move( qi ) );

    // Are we full?
    bool full = m_items.size() >= deallocation_group::preferred_maximum;
    return full;
}

deallocation_group::deallocation_group(
        deallocation_group&& other
)  noexcept :
    m_epoch( other.m_epoch ),
    m_items( std::move( other.m_items ) )
{

}

deallocation_group& deallocation_group::operator=(
        deallocation_group&& other
)  noexcept
{
    if( this != &other )
    {
        m_epoch = other.m_epoch;
        m_items = std::move( other.m_items );
    }
    return *this;
}

}
}
