/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
#include <type_traits>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // IntegralConstant's API (like std::integral_constant)
    //////////////////////////////////////////////////////////////////////////
    {
        // operator()
        static_assert(size_t<0>() == 0, "");
        static_assert(size_t<1>() == 1, "");
        static_assert(int_<-3>() == -3, "");

        // decltype(operator())
        BOOST_HANA_CONSTANT_CHECK(decltype_(size_t<0>()) == type<std::size_t>);
        BOOST_HANA_CONSTANT_CHECK(decltype_(int_<-3>()) == type<int>);

        // conversions
        constexpr std::size_t a = size_t<0>, b = size_t<1>;
        static_assert(a == 0 && b == 1, "");

        constexpr int c = int_<0>, d = int_<-3>;
        static_assert(c == 0 && d == -3, "");

        // nested ::value
        static_assert(decltype(int_<1>)::value == 1, "");

        // nested ::type
        static_assert(std::is_same<
            decltype(int_<1>)::type,
            std::remove_cv_t<decltype(int_<1>)>
        >{}, "");

        // nested ::value_type
        static_assert(std::is_same<decltype(int_<1>)::value_type, int>{}, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // Make sure we can inherit _integral_constant and retain the same
    // data type.
    //////////////////////////////////////////////////////////////////////////
    {
        struct derived : _integral_constant<int, 10> { };
        static_assert(std::is_same<datatype_t<derived>, IntegralConstant<int>>{}, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // Extensions to std::integral_constant
    //////////////////////////////////////////////////////////////////////////
    {
        // times member function (the other ones are tested in the examples)
        {
            int counter = 0;
            int_<3>.times([&] { ++counter; });
            BOOST_HANA_RUNTIME_CHECK(counter == 3);
        }

        // Arithmetic operators
        {
            BOOST_HANA_CONSTANT_CHECK(+int_<1> == int_<1>);
            BOOST_HANA_CONSTANT_CHECK(-int_<1> == int_<-1>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> + int_<2> == int_<3>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> - int_<2> == int_<-1>);
            BOOST_HANA_CONSTANT_CHECK(int_<3> * int_<2> == int_<6>);
            BOOST_HANA_CONSTANT_CHECK(int_<6> / int_<3> == int_<2>);
            BOOST_HANA_CONSTANT_CHECK(int_<6> % int_<4> == int_<2>);
            BOOST_HANA_CONSTANT_CHECK(~int_<6> == int_<~6>);
            BOOST_HANA_CONSTANT_CHECK((int_<6> & int_<3>) == int_<6 & 3>);
            BOOST_HANA_CONSTANT_CHECK((int_<4> | int_<2>) == int_<4 | 2>);
            BOOST_HANA_CONSTANT_CHECK((int_<6> ^ int_<3>) == int_<6 ^ 3>);
            BOOST_HANA_CONSTANT_CHECK((int_<6> << int_<3>) == int_<(6 << 3)>);
            BOOST_HANA_CONSTANT_CHECK((int_<6> >> int_<3>) == int_<(6 >> 3)>);
        }

        // Comparison operators
        {
            BOOST_HANA_CONSTANT_CHECK(int_<0> == int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> != int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<0> < int_<1>);
            BOOST_HANA_CONSTANT_CHECK(int_<0> <= int_<1>);
            BOOST_HANA_CONSTANT_CHECK(int_<0> <= int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> > int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> >= int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<0> >= int_<0>);
        }

        // Logical operators
        {
            BOOST_HANA_CONSTANT_CHECK(int_<3> || int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<3> && int_<1>);
            BOOST_HANA_CONSTANT_CHECK(!int_<0>);
            BOOST_HANA_CONSTANT_CHECK(!!int_<3>);
        }

        // Creation with user-defined literals
        {
            using namespace boost::hana::literals;

            BOOST_HANA_CONSTANT_CHECK(0_c == llong<0>);
            BOOST_HANA_CONSTANT_CHECK(1_c == llong<1>);
            BOOST_HANA_CONSTANT_CHECK(12_c == llong<12>);
            BOOST_HANA_CONSTANT_CHECK(123_c == llong<123>);
            BOOST_HANA_CONSTANT_CHECK(1234567_c == llong<1234567>);
            BOOST_HANA_CONSTANT_CHECK(-34_c == llong<-34>);

            BOOST_HANA_CONSTANT_CHECK(decltype_(-1234_c) == decltype_(llong<-1234>));
            BOOST_HANA_CONSTANT_CHECK(-12_c < 0_c);
        }
    }
}
