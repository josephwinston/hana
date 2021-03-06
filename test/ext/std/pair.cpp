/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/pair.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/product.hpp>
#include <test/cnumeric.hpp>
#include <test/injection.hpp>

#include <utility>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::std::Pair> = make<Tuple>(
        type<Product>
    );

    template <>
    auto objects<ext::std::Pair> = make<Tuple>(
          ::std::make_pair(cnumeric<int, 0>, cnumeric<int, 0>)
        , ::std::make_pair(cnumeric<int, 0>, cnumeric<int, 1>)
        , ::std::make_pair(cnumeric<int, 1>, cnumeric<int, 0>)
        , ::std::make_pair(cnumeric<int, 1>, cnumeric<int, 1>)
    );
}}}


int main() {
    test::check_datatype<ext::std::Pair>();

    using test::x;

    // Product
    {
        // first
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                first(std::make_pair(x<0>, x<1>)),
                x<0>
            ));
        }

        // second
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                second(std::make_pair(x<0>, x<1>)),
                x<1>
            ));
        }

        // make
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<ext::std::Pair>(x<0>, x<1>),
                std::make_pair(x<0>, x<1>)
            ));
        }
    }
}
