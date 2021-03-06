/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_MONAD_PLUS_HPP
#define BOOST_HANA_TEST_LAWS_MONAD_PLUS_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/monad_plus.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename M, typename = when<true>>
    struct TestMonadPlus : TestMonadPlus<M, laws> {
        using TestMonadPlus<M, laws>::TestMonadPlus;
    };

    template <typename M>
    struct TestMonadPlus<M, laws> {
        static_assert(_models<MonadPlus, M>{}, "");

        template <typename Xs>
        TestMonadPlus(Xs xs) {
            auto f = hana::compose(lift<M>, test::_injection<0>{});

            foreach3(xs, [=](auto a, auto b, auto c) {

                // left identity
                BOOST_HANA_CHECK(hana::equal(
                    hana::concat(empty<M>(), a),
                    a
                ));

                // right identity
                BOOST_HANA_CHECK(hana::equal(
                    hana::concat(a, empty<M>()),
                    a
                ));

                // associativity
                BOOST_HANA_CHECK(hana::equal(
                    hana::concat(a, hana::concat(b, c)),
                    hana::concat(hana::concat(a, b), c)
                ));

                // absorption
                BOOST_HANA_CHECK(hana::equal(
                    hana::bind(empty<M>(), f),
                    empty<M>()
                ));

                BOOST_HANA_CHECK(hana::equal(
                    hana::bind(a, hana::always(empty<M>())),
                    empty<M>()
                ));

            });
        }
    };

    template <typename S>
    struct TestMonadPlus<S, when<_models<Sequence, S>{}>> : TestMonadPlus<S, laws> {
        template <int i>
        using eq = _constant<i>;

        template <typename Xs>
        TestMonadPlus(Xs xs) : TestMonadPlus<S, laws>{xs} {
            auto z = eq<999>{};
            constexpr auto list = make<S>;

            //////////////////////////////////////////////////////////////////
            // empty
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<S>(), list()
            ));

            //////////////////////////////////////////////////////////////////
            // concat
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(), list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(), list(eq<0>{})),
                list(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(), list(eq<0>{}, eq<1>{})),
                list(eq<0>{}, eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(eq<0>{}), list()),
                list(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(eq<0>{}), list(eq<1>{})),
                list(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(eq<0>{}), list(eq<1>{}, eq<2>{})),
                list(eq<0>{}, eq<1>{}, eq<2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(eq<0>{}, eq<1>{}), list()),
                list(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(eq<0>{}, eq<1>{}), list(eq<2>{})),
                list(eq<0>{}, eq<1>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(eq<0>{}, eq<1>{}), list(eq<2>{}, eq<3>{})),
                list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
            ));


            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(list(), list()),
                list()
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                concat(list(1), list()),
                list(1)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                concat(list(), list(1)),
                list(1)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                concat(list(1), list('2')),
                list(1, '2')
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                concat(list(1, '2'), list(3.3)),
                list(1, '2', 3.3)
            ));


            //////////////////////////////////////////////////////////////////
            // filter
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(), not_equal.to(z)),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(z), not_equal.to(z)),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(eq<1>{}), not_equal.to(z)),
                list(eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(eq<1>{}, eq<2>{}), not_equal.to(z)),
                list(eq<1>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(z, eq<2>{}), not_equal.to(z)),
                list(eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(eq<1>{}, z), not_equal.to(z)),
                list(eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(z, eq<2>{}, eq<3>{}), not_equal.to(z)),
                list(eq<2>{}, eq<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(eq<1>{}, z, eq<3>{}), not_equal.to(z)),
                list(eq<1>{}, eq<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(eq<1>{}, eq<2>{}, z), not_equal.to(z)),
                list(eq<1>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(eq<1>{}, z, z), not_equal.to(z)),
                list(eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(z, eq<2>{}, z), not_equal.to(z)),
                list(eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                filter(list(z, z, eq<3>{}), not_equal.to(z)),
                list(eq<3>{})
            ));

            //////////////////////////////////////////////////////////////////
            // prepend
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(eq<0>{}, list()),
                list(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(eq<0>{}, list(eq<1>{})),
                list(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(eq<0>{}, list(eq<1>{}, eq<2>{})),
                list(eq<0>{}, eq<1>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(eq<0>{}, list(eq<1>{}, eq<2>{}, eq<3>{})),
                list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
            ));


            BOOST_HANA_CONSTEXPR_CHECK(equal(
                prepend(1, list()), list(1)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                prepend(1, list('2')), list(1, '2')
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                prepend(1, list('2', 3.3)), list(1, '2', 3.3)
            ));

            //////////////////////////////////////////////////////////////////
            // append
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                append(list(), eq<0>{}),
                list(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                append(list(eq<0>{}), eq<1>{}),
                list(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                append(list(eq<0>{}, eq<1>{}), eq<2>{}),
                list(eq<0>{}, eq<1>{}, eq<2>{})
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                append(list(), 1), list(1)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                append(list(1), '2'), list(1, '2')
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                append(list(1, '2'), 3.3), list(1, '2', 3.3)
            ));


            //////////////////////////////////////////////////////////////////
            // cycle
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<0>, list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<1>, list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<2>, list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<3>, list()),
                list()
            ));


            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<0>, list(eq<0>{})),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<1>, list(eq<0>{})),
                list(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<2>, list(eq<0>{})),
                list(eq<0>{}, eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<3>, list(eq<0>{})),
                list(eq<0>{}, eq<0>{}, eq<0>{})
            ));


            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<0>, list(eq<0>{}, eq<1>{})),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<1>, list(eq<0>{}, eq<1>{})),
                list(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<2>, list(eq<0>{}, eq<1>{})),
                list(eq<0>{}, eq<1>{}, eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<3>, list(eq<0>{}, eq<1>{})),
                list(eq<0>{}, eq<1>{}, eq<0>{}, eq<1>{}, eq<0>{}, eq<1>{})
            ));


            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<0>, list(eq<0>{}, eq<1>{}, eq<2>{})),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<1>, list(eq<0>{}, eq<1>{}, eq<2>{})),
                list(eq<0>{}, eq<1>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<2>, list(eq<0>{}, eq<1>{}, eq<2>{})),
                list(eq<0>{}, eq<1>{}, eq<2>{}, eq<0>{}, eq<1>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cycle(size_t<3>, list(eq<0>{}, eq<1>{}, eq<2>{})),
                list(eq<0>{}, eq<1>{}, eq<2>{}, eq<0>{}, eq<1>{}, eq<2>{}, eq<0>{}, eq<1>{}, eq<2>{})
            ));


            //////////////////////////////////////////////////////////////////
            // repeat
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                repeat<S>(size_t<0>, eq<0>{}),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                repeat<S>(size_t<1>, eq<0>{}),
                list(eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                repeat<S>(size_t<2>, eq<0>{}),
                list(eq<0>{}, eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                repeat<S>(size_t<3>, eq<0>{}),
                list(eq<0>{}, eq<0>{}, eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                repeat<S>(size_t<4>, eq<0>{}),
                list(eq<0>{}, eq<0>{}, eq<0>{}, eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                repeat<S>(size_t<5>, eq<0>{}),
                list(eq<0>{}, eq<0>{}, eq<0>{}, eq<0>{}, eq<0>{})
            ));


            //////////////////////////////////////////////////////////////////
            // prefix
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                prefix(z, list()),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                prefix(z, list(eq<0>{})),
                list(z, eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                prefix(z, list(eq<0>{}, eq<1>{})),
                list(z, eq<0>{}, z, eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                prefix(z, list(eq<0>{}, eq<1>{}, eq<2>{})),
                list(z, eq<0>{}, z, eq<1>{}, z, eq<2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                prefix(z, list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})),
                list(z, eq<0>{}, z, eq<1>{}, z, eq<2>{}, z, eq<3>{})
            ));


            //////////////////////////////////////////////////////////////////
            // suffix
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                suffix(z, list()),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                suffix(z, list(eq<0>{})),
                list(eq<0>{}, z)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                suffix(z, list(eq<0>{}, eq<1>{})),
                list(eq<0>{}, z, eq<1>{}, z)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                suffix(z, list(eq<0>{}, eq<1>{}, eq<2>{})),
                list(eq<0>{}, z, eq<1>{}, z, eq<2>{}, z)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                suffix(z, list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})),
                list(eq<0>{}, z, eq<1>{}, z, eq<2>{}, z, eq<3>{}, z)
            ));
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_MONAD_PLUS_HPP
