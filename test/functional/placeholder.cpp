/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional/placeholder.hpp>

#include <boost/hana/assert.hpp>

#include <type_traits>
#include <utility>
using namespace boost::hana;


struct _extra { virtual ~_extra() { } };
_extra extra{};

constexpr struct { } invalid{};

template <typename ...> using bool_t = bool;
constexpr bool valid_call(...) { return false; }
template <typename F, typename ...Args>
constexpr auto valid_call(F&& f, Args&& ...args)
    -> bool_t<decltype(std::forward<F>(f)(std::forward<Args>(args)...))>
{ return true; }

#define BOOST_HANA_TEST_BINARY_OP(op, x, y)                                 \
    BOOST_HANA_CONSTEXPR_CHECK((_ op _)(x, y) == (x op y));                 \
    BOOST_HANA_RUNTIME_CHECK((_ op _)(x, y, extra) == (x op y));            \
    BOOST_HANA_RUNTIME_CHECK((_ op _)(x, y, extra, extra) == (x op y));     \
    static_assert(!valid_call(_ op _), "");                                 \
    static_assert(!valid_call(_ op _, invalid), "");                        \
    static_assert(!valid_call(_ op _, invalid, invalid), "");               \
                                                                            \
    BOOST_HANA_CONSTEXPR_CHECK((_ op y)(x) == (x op y));                    \
    BOOST_HANA_RUNTIME_CHECK((_ op y)(x, extra) == (x op y));               \
    BOOST_HANA_RUNTIME_CHECK((_ op y)(x, extra, extra) == (x op y));        \
    static_assert(!valid_call(_ op y), "");                                 \
    static_assert(!valid_call(_ op y, invalid), "");                        \
                                                                            \
    BOOST_HANA_CONSTEXPR_CHECK((x op _)(y) == (x op y));                    \
    BOOST_HANA_RUNTIME_CHECK((x op _)(y, extra) == (x op y));               \
    BOOST_HANA_RUNTIME_CHECK((x op _)(y, extra, extra) == (x op y));        \
    static_assert(!valid_call(x op _), "");                                 \
    static_assert(!valid_call(x op _, invalid), "");                        \
    static_assert(!valid_call(x op _, invalid), "");                        \
/**/

#define BOOST_HANA_TEST_UNARY_OP(op, x)                                     \
    BOOST_HANA_CONSTEXPR_CHECK((op _)(x) == (op x));                        \
    BOOST_HANA_RUNTIME_CHECK((op _)(x, extra) == (op x));                   \
    BOOST_HANA_RUNTIME_CHECK((op _)(x, extra, extra) == (op x));            \
    static_assert(!valid_call(op _), "");                                   \
    static_assert(!valid_call(op _, invalid), "");                          \
/**/

struct _incr {
    template <typename X>
    constexpr auto operator()(X x) const -> decltype(x+1)
    { return x+1; }
};
constexpr _incr incr{};

int main() {
    // Arithmetic
    BOOST_HANA_TEST_UNARY_OP(+, 1)
    BOOST_HANA_TEST_UNARY_OP(-, 1)
    BOOST_HANA_TEST_BINARY_OP(+, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(-, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(*, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(/, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(%, 6, 3)

    // Bitwise
    BOOST_HANA_TEST_UNARY_OP(~, 5)
    BOOST_HANA_TEST_BINARY_OP(&, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(|, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(^, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(<<, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(>>, 6, 3)

    // Comparison
    BOOST_HANA_TEST_BINARY_OP(==, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(!=, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(<, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(<=, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(>, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(>=, 6, 3)

    // Logical
    BOOST_HANA_TEST_BINARY_OP(||, true, false)
    BOOST_HANA_TEST_BINARY_OP(&&, true, true)
    BOOST_HANA_TEST_UNARY_OP(!, true)

    // Member access
    constexpr int i = 4;
    constexpr int array[] = {0, 1, 2};
    BOOST_HANA_TEST_UNARY_OP(*, &i)

    BOOST_HANA_CONSTEXPR_CHECK(_[0](array) == array[0]);
    BOOST_HANA_RUNTIME_CHECK(_[0](array, extra) == array[0]);
    BOOST_HANA_RUNTIME_CHECK(_[0](array, extra, extra) == array[0]);
    BOOST_HANA_CONSTEXPR_CHECK(_[1](array) == array[1]);
    BOOST_HANA_CONSTEXPR_CHECK(_[1](array) == array[1]);
    BOOST_HANA_CONSTEXPR_CHECK(_[2](array) == array[2]);
    static_assert(!valid_call(_[invalid]), "");
    static_assert(!valid_call(_[invalid], array), "");
    static_assert(!valid_call(_[invalid], invalid), "");
    static_assert(!valid_call(_[0], invalid), "");

    // Call operator
    BOOST_HANA_CONSTEXPR_CHECK(_(1)(incr) == incr(1));
    BOOST_HANA_RUNTIME_CHECK(_(1)(incr, extra) == incr(1));
    BOOST_HANA_RUNTIME_CHECK(_(1)(incr, extra, extra) == incr(1));
    BOOST_HANA_CONSTEXPR_CHECK(_(2)(incr) == incr(2));
    BOOST_HANA_CONSTEXPR_CHECK(_(3)(incr) == incr(3));
    static_assert(!valid_call(_(invalid)), "");
    static_assert(!valid_call(_(invalid), incr), "");
    static_assert(!valid_call(_(invalid), invalid), "");
    static_assert(!valid_call(_(1), invalid), "");
}
