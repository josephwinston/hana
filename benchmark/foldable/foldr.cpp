/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/fwd/foldable.hpp>

#include "benchmark.hpp"

<%= setup %>

template <int i> struct x { };


int main() {
    struct state { };
    auto f = [](auto&& x, auto&& s) -> decltype(auto) {
        return boost::hana::detail::std::forward<decltype(x)>(x);
    };
    auto foldable = <%= foldable %>;

    boost::hana::benchmark::measure([=] {
        boost::hana::foldr(foldable, state{}, f);
    });
}
