#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include "../../src/gp.h"

#include <string>
#include <tuple>
#include <variant>
#include <vector>

//! Represents custom data associated with specific allocation
class CustomData
{
public:

    //! Varint type holding the possible values.
    typedef std::variant< int64_t, int32_t > t_value;

    //! Named value.
    typedef std::tuple< std::string, t_value > t_named_value;

    //! Collection of named values.
    typedef std::vector< t_named_value > t_values;

public:

    CustomData();

    //! Appends the specified value.
    template< typename t_value >
    void append(
            std::string name,
            const t_value& value
    )
    { m_values.emplace_back( name, value ); }

    //! Beginning of the values.
    t_values::const_iterator begin() const { return m_values.begin(); }

    //! End of the values.
    t_values::const_iterator end() const { return m_values.end(); }

private:

    t_values m_values;  //!< The custom values.
};

#endif // CUSTOMDATA_H
