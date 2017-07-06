#include "deallocation_group.h"

#include <algorithm>
#include <vector>

namespace gp
{
namespace details
{

deallocation_group::deallocation_group()
{  
}

//! Appends new items for deallocation.
size_t deallocation_group::append(
        gp::configuration::epoch_t epoch,
        gp::details::deallocation_group_header& header,
        std::initializer_list< queued_item > items
)
{
    // Collect.
    header.set_epoch( epoch );
    std::initializer_list< queued_item >::iterator last = items.begin() + std::min( header.size(), items.size() );
    for( std::initializer_list< queued_item >::iterator c = items.begin();
            c != items.end(); ++c )
    {
        m_buffer.items[ header.next() ] = (*c);
    }

    // Return the number of appended items.
    return last - items.begin();
}

//! Appends new items for deallocation. Returns true if the group becomes full.
size_t deallocation_group::append(
        gp::configuration::epoch_t epoch,
        gp::details::deallocation_group_header& header,
        std::vector< queued_item >&& items
)
{
    // Collect.
    header.set_epoch( epoch );
    std::vector< queued_item >::iterator last = items.begin() + std::min( header.size(), items.size() );
    for( std::vector< queued_item >::iterator c = items.begin();
            c != items.end(); ++c )
    {
        m_buffer.items[ header.next() ] = std::move( *c );
    }

    // Return the number of appended items.
    return last - items.begin();
}

}
}
