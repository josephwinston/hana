/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_MONOID_HPP
#define BOOST_HANA_TEST_LAWS_MONOID_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/monoid.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename M, typename = when<true>>
    struct TestMonoid : TestMonoid<M, laws> {
        using TestMonoid<M, laws>::TestMonoid;
    };

    template <typename M>
    struct TestMonoid<M, laws> {
        static_assert(_models<Monoid, M>{}, "");

        template <typename Xs>
        TestMonoid(Xs xs) {
            foreach3(xs, [](auto a, auto b, auto c) {

                // left identity
                BOOST_HANA_CHECK(hana::equal(
                    hana::plus(zero<M>(), a),
                    a
                ));

                // right identity
                BOOST_HANA_CHECK(hana::equal(
                    hana::plus(a, zero<M>()),
                    a
                ));

                // associativity
                BOOST_HANA_CHECK(equal(
                    hana::plus(a, hana::plus(b, c)),
                    hana::plus(hana::plus(a, b), c)
                ));

                // operators
                only_when_(bool_<has_operator<M, decltype(plus)>{}>, [=](auto _) {
                    BOOST_HANA_CHECK(hana::equal(
                        hana::plus(a, b),
                        _(a) + _(b)
                    ));
                });

            });
        }
    };

    template <typename C>
    struct TestMonoid<C, when<_models<Constant, C>{}>>
        : TestMonoid<C, laws>
    {
        template <typename Xs>
        TestMonoid(Xs xs) : TestMonoid<C, laws>{xs} {
            foreach2(xs, [](auto x, auto y) {

                BOOST_HANA_CHECK(hana::equal(
                    hana::value(zero<C>()),
                    zero<typename C::value_type>()
                ));

                BOOST_HANA_CHECK(hana::equal(
                    hana::plus(hana::value(x), hana::value(y)),
                    hana::value(hana::plus(x, y))
                ));

            });
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_MONOID_HPP
