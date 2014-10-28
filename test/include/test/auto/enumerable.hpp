/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_ENUMERABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_ENUMERABLE_HPP

#include <boost/hana/enumerable.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename E>
    auto laws<Enumerable, E> = [] {
        //! @todo
        //! Enumerable should probably have the following laws:
        //! For all enumerable `x`,
        //! @code
        //!     succ(pred(x)) == x
        //!     pred(succ(x)) == x
        //! @endcode
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_ENUMERABLE_HPP