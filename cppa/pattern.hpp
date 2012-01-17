/******************************************************************************\
 *           ___        __                                                    *
 *          /\_ \    __/\ \                                                   *
 *          \//\ \  /\_\ \ \____    ___   _____   _____      __               *
 *            \ \ \ \/\ \ \ '__`\  /'___\/\ '__`\/\ '__`\  /'__`\             *
 *             \_\ \_\ \ \ \ \L\ \/\ \__/\ \ \L\ \ \ \L\ \/\ \L\.\_           *
 *             /\____\\ \_\ \_,__/\ \____\\ \ ,__/\ \ ,__/\ \__/.\_\          *
 *             \/____/ \/_/\/___/  \/____/ \ \ \/  \ \ \/  \/__/\/_/          *
 *                                          \ \_\   \ \_\                     *
 *                                           \/_/    \/_/                     *
 *                                                                            *
 * Copyright (C) 2011, 2012                                                   *
 * Dominik Charousset <dominik.charousset@haw-hamburg.de>                     *
 *                                                                            *
 * This file is part of libcppa.                                              *
 * libcppa is free software: you can redistribute it and/or modify it under   *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License                  *
 * or (at your option) any later version.                                     *
 *                                                                            *
 * libcppa is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
 * See the GNU Lesser General Public License for more details.                *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with libcppa. If not, see <http://www.gnu.org/licenses/>.            *
\******************************************************************************/


#ifndef LIBCPPA_PATTERN_HPP
#define LIBCPPA_PATTERN_HPP

#include <vector>
#include <cstddef>
#include <type_traits>

#include "cppa/anything.hpp"
#include "cppa/any_tuple.hpp"
#include "cppa/uniform_type_info.hpp"

#include "cppa/util/tbind.hpp"
#include "cppa/util/type_list.hpp"

#include "cppa/detail/tdata.hpp"
#include "cppa/detail/types_array.hpp"
#include "cppa/detail/pattern_details.hpp"

namespace cppa {

struct dont_initialize_pattern { };

template<typename... Types>
class pattern
{

    static_assert(sizeof...(Types) > 0, "empty pattern");

    pattern(pattern const&) = delete;
    pattern& operator=(pattern const&) = delete;

 public:

    pattern(dont_initialize_pattern const&) { }

    static constexpr size_t size = sizeof...(Types);

    typedef util::type_list<Types...> types;

    typedef typename util::lt_filter_not<types, util::tbind<std::is_same, anything>::type>::type
            filtered_types;

    typedef typename tuple_view_type_from_type_list<filtered_types>::type
            tuple_view_type;

    typedef typename tuple_view_type::mapping_vector mapping_vector;

    pattern()
    {
        for (size_t i = 0; i < size; ++i)
        {
            m_data_ptr[i] = nullptr;
        }
    }

    template<typename Arg0, typename... Args>
    pattern(Arg0 const& arg0, Args const&... args) : m_data(arg0, args...)
    {
        bool invalid_args[] = { detail::is_boxed<Arg0>::value,
                                detail::is_boxed<Args>::value... };
        for (size_t i = 0; i < sizeof...(Args) + 1; ++i)
        {
            m_data_ptr[i] = invalid_args[i] ? nullptr : m_data.at(i);
        }
        for (size_t i = sizeof...(Args) + 1; i < size; ++i)
        {
            m_data_ptr[i] = nullptr;
        }
    }

    bool operator()(cppa::any_tuple const& msg,
                    mapping_vector* mapping = nullptr) const
    {
        detail::pattern_iterator<decltype(m_utis)> arg0(size, m_data_ptr, m_utis);
        detail::tuple_iterator_arg<any_tuple,mapping_vector> arg1(msg, mapping);
        return detail::do_match(arg0, arg1);
    }

// private:

    detail::tdata<Types...> m_data;
    //cppa::uniform_type_info const* m_utis[size];
    static detail::types_array<Types...> m_utis;
    void const* m_data_ptr[size];

};

template<typename... Types>
detail::types_array<Types...> pattern<Types...>::m_utis;

template<class ExtendedType, class BasicType>
ExtendedType* extend_pattern(BasicType* p)
{
    ExtendedType* et = new ExtendedType(dont_initialize_pattern());
    et->m_data = p->m_data;
    for (size_t i = 0; i < BasicType::size; ++i)
    {
        et->m_data_ptr[i] = (p->m_data_ptr[i]) ? et->m_data.at(i)
                                               : nullptr;
    }
    for (size_t i = BasicType::size; i < ExtendedType::size; ++i)
    {
        et->m_data_ptr[i] = nullptr;
    }
    return et;
}

template<typename TypeList>
struct pattern_from_type_list;

template<typename... Types>
struct pattern_from_type_list<util::type_list<Types...>>
{
    typedef pattern<Types...> type;
};

} // namespace cppa

#endif // PATTERN_HPP
