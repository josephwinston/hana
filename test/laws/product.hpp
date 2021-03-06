/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_PRODUCT_HPP
#define BOOST_HANA_TEST_LAWS_PRODUCT_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/product.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename P, typename = when<true>>
    struct TestProduct : TestProduct<P, laws> {
        using TestProduct<P, laws>::TestProduct;
    };

    template <typename P>
    struct TestProduct<P, laws> {
        static_assert(_models<Product, P>{}, "");

        template <typename Elements>
        TestProduct(Elements elements) {
            foreach2(elements, [](auto x, auto y) {
                BOOST_HANA_CHECK(hana::equal(
                    hana::first(hana::make<P>(x, y)),
                    x
                ));

                BOOST_HANA_CHECK(hana::equal(
                    hana::second(hana::make<P>(x, y)),
                    y
                ));
            });
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_PRODUCT_HPP
