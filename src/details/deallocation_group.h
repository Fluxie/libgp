#ifndef GP_DEALLOCATION_GROUP_H
#define GP_DEALLOCATION_GROUP_H

#include "../configuration.h"
#include "../queued_item.h"
#include "deallocation_group_header.h"

#include <array>
#include <memory>


namespace  gp { namespace details
{

//! Represents a group of objects marked for deallocation.
class deallocation_group
{
private:

    //! Maximum size for the buffer.
    static constexpr size_t buffer_size = ( gp::configuration::deallocation_group_size - sizeof( deallocation_group_header ) );

    //! Maximum number of items stored in one group.
    static constexpr size_t maximum_item_count = buffer_size / sizeof( queued_item );

    //! Buffer type for storing the items queued for dellocation.
    union buffer {
        unsigned char bufferSize[ buffer_size ];  //!< Defines the size of the buffer.
        gp::queued_item items[ maximum_item_count ];

        //! The number of items in the buffer is tracked separately and we don't need to initialize the buffer to zero.
        buffer() {}
    };

public:

    //! Initializes a new empty dellocation group.
    deallocation_group();

    //! Gets pointer to the beginning of objects marked for deallocation.
    gp::queued_item* begin() noexcept { return m_buffer.items; }

    //! Gets pointer to the end of the objects marked for deallocation.
    gp::queued_item* end() noexcept { return m_buffer.items + m_header.size(); }

    //! Returns the epoch of the group.
    gp::configuration::epoch_t epoch() const noexcept { return m_header.epoch(); }

    //! Resets the group to zero size.
    void reset(
            deallocation_group* next  //!< Group deallocated after this group.
    ) noexcept
    {
        m_header.reset( next );
    }

    //! Appends new items for deallocation. Returns the number of appended objects.
    size_t append(
            gp::configuration::epoch_t epoch,
            gp::details::deallocation_group_header& header,
            std::initializer_list< queued_item > items
    );

    //! Appends new items for deallocation. Returns the number of appended objects.
    size_t append(
            gp::configuration::epoch_t epoch,
            gp::details::deallocation_group_header& header,
            std::vector< queued_item >&& items
    );

    //! Appends items to the group.
    template< typename t_iterator >
    size_t append(
            gp::configuration::epoch_t epoch,
            t_iterator begin,
            t_iterator end
    )
    {
        // Collect.
        m_header.set_epoch( epoch );
        t_iterator last = begin + std::min( maximum_item_count - m_header.size(), ( size_t ) std::distance( begin, end ) );
        for( t_iterator c = begin;c != last; ++c )
        {
            (*this)[ m_header.next() ] = (*c);
        }

        // Return the number of appended items.
        size_t count = last - begin;
        return count;
    }

    //! Deallocates all the items in this group. Returns the number of deallocated items.
    size_t dellocate() noexcept
    {
        // Deallocate.
        for( int i = 0; i <= m_header.slot(); ++i )
        {
            gp::queued_item& qi = m_buffer.items[ i ];
            qi.m_deallocate( qi.m_object );
        }
        size_t count = m_header.size();
        reset( nullptr );
        return count;
    }

    //! Deallocates all the items in this group. Assumes the group is full.
    size_t dellocate_full() noexcept
    {
        // Deallocate the whole buffer.
        for( size_t st = 0; st < maximum_item_count ; ++st )
        {
            gp::queued_item& qi = m_buffer.items[ st ];
            qi.m_deallocate( qi.m_object );
        }

        // When a full group is deallocated the dellocation algorithm needs to
        // check if the next group can be deallocated as well.
        // This involves reading the groups header.
        // We can speed up this operation by ensuring it is available when needed.
        m_header.prefetch_next();

        return gp::configuration::deallocation_group_size ;        
    }

// Prevent moves.
public:

    deallocation_group(
            deallocation_group&&
    ) noexcept = delete;

    deallocation_group& operator=(
            deallocation_group&&
    ) noexcept = delete;

// Prevent copying.
public:

    //! Prevent copying.
    deallocation_group(
            const deallocation_group&
    ) = delete;

    //! Prevent regular assignment.
    deallocation_group& operator=(
            const deallocation_group&
    ) = delete;       

private:

    //! Returns a referencto to an item in the buffer.
    queued_item& operator[](
            int index
    ) noexcept
    {
        return m_buffer.items[ index ];
    }

private:

    deallocation_group_header m_header;  //!< Information about the group including the number of queued items in the buffer.
    buffer m_buffer;  //!< Items marked for dellocation.
};

}

}

#endif // GP_DEALLOCATION_GROUP_H
