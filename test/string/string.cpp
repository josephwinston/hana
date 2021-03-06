/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>
#include <boost/hana/config.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // string and BOOST_HANA_STRING
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto const s1 = BOOST_HANA_STRING("abcd");
        constexpr auto s2 = string<'a', 'b', 'c', 'd'>;

        static_assert(std::is_same<
            decltype(s1), decltype(s2)
        >::value, "");
    }
}
