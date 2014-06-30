/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "minimal.hpp"
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % int_<2> != int_<0>;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(filter(odd, ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(filter(odd, ilist<0>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(filter(odd, ilist<0, 1>) == ilist<1>);
    BOOST_HANA_STATIC_ASSERT(filter(odd, ilist<0, 1, 2>) == ilist<1>);
    BOOST_HANA_STATIC_ASSERT(filter(odd, ilist<0, 1, 2, 3>) == ilist<1, 3>);
}