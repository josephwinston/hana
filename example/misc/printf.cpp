/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/demux.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <cstdio>
using namespace boost::hana;


constexpr auto formats = make_map(
    make_pair(type<int>, string<'%', 'd'>),
    make_pair(type<float>, string<'%', 'f'>),
    make_pair(type<char const*>, string<'%', 's'>)
);

struct concat_strings {
    template <char ...s1, char ...s2>
    constexpr auto operator()(_string<s1...>, _string<s2...>) const
    { return string<s1..., s2...>; }
};

template <typename ...Tokens>
constexpr auto format(Tokens ...tokens_) {
    auto tokens = make_tuple(tokens_...);

    // If you don't care about constexpr-ness of `format`, you
    // can use this lambda instead of `demux(...)(...)`:
    //
    // [](auto token) {
    //     return from_just(find(formats, decltype_(token)));
    // }
    auto format_string_tokens = adjust_if(tokens,
        compose(not_, is_a<String>),
        demux(from_just)(compose(partial(find, formats), decltype_))
    );

    auto format_string = foldl(format_string_tokens, string<>, concat_strings{});
    auto variables = filter(tokens, compose(not_, is_a<String>));
    return prepend(format_string, variables);
}


template <char ...s>
char const string_literal[] = {s..., '\0'};

template <char ...s>
auto to_string_literal(_string<s...> const&)
{ return string_literal<s...>; }

int main() {
    int a = 1;
    float b = 1.3;
    char const* c = "abcdef";

    auto args = format(
          BOOST_HANA_STRING("first="), a
        , BOOST_HANA_STRING(" second="), b
        , BOOST_HANA_STRING(" third="), c
    );

    unpack(args, [](auto fmt, auto ...args) {
        std::printf(to_string_literal(fmt), args...);
    });
}
