#include "../include/Interval.h"

#define CATCH_CONFIG_MAIN	
#include "catch.hpp"

TEST_CASE( "Empty", "[identities]" ) {
    REQUIRE( MAKE_INTERVAL(0,0).Empty );
    REQUIRE( !MAKE_INTERVAL(0,0.001).Empty );
}

TEST_CASE( "Reverse", "[identities]" ) {
    REQUIRE( !MAKE_INTERVAL(0, 0).Reverse );
    REQUIRE(  MAKE_INTERVAL(1, 0).Reverse );
    REQUIRE( !MAKE_INTERVAL(0, 1).Reverse );
}

TEST_CASE( "Size", "[identities]" ) {
    constexpr auto v1 = 0., v2 = 8.;
    REQUIRE( floatCompareEqual(MAKE_INTERVAL(v1, v2).Length, v2-v1));
}

TEST_CASE( "Same Interval Assignment", "[identities]" ) {
    constexpr auto v1 = 0, v2 = 10, v3 = 3, v4 = 8;
    auto i1 = MAKE_INTERVAL(v1, v2, v3);
    auto i2 = MAKE_INTERVAL(v1, v2, v4);
    i1 = i2;
    REQUIRE( i1.value == v4 );
}

TEST_CASE( "Different Interval Assignment", "[identities]" ) {
    constexpr auto v1 = 0., v2 = 10., v3 = 3.;
    auto i1 = MAKE_INTERVAL(v1, v3, v3/2);
    auto i2 = MAKE_INTERVAL(v1, v2, v2/2);
    i1 = i2;
    auto value = (v2/2)*((v3-v1)/(v2-v1));
    REQUIRE( floatCompareEqual(i1.value, value));
}

TEST_CASE( "Value Assignment", "[identities]" ) {
    constexpr auto v1 = 0, v2 = 10, v3 = 3;
    auto i1 = MAKE_INTERVAL(v1, v2);
    i1 = v3;
    REQUIRE( i1.value == v3 );
}


TEST_CASE( "Equality", "[identities]" ) {
    REQUIRE( (MAKE_INTERVAL(0,0) == MAKE_INTERVAL(0,0)) );
    REQUIRE( (MAKE_INTERVAL(0,1) == MAKE_INTERVAL(0,1)) );

    REQUIRE( (MAKE_INTERVAL(0,0.001) != MAKE_INTERVAL(0,0)) );
    REQUIRE( (MAKE_INTERVAL(0,0.001) != MAKE_INTERVAL(0,1)) );

    REQUIRE( !(MAKE_INTERVAL(0,0) == MAKE_INTERVAL(0, .0001)) );
    REQUIRE( !(MAKE_INTERVAL(0,1) == MAKE_INTERVAL(0, .0001)) );

    REQUIRE( !(MAKE_INTERVAL(0,0) != MAKE_INTERVAL(0,0)) );
    REQUIRE( !(MAKE_INTERVAL(0,1) != MAKE_INTERVAL(0,1)) );
}

TEST_CASE( "Greater than", "[comparisons]" ) {
    REQUIRE( (MAKE_INTERVAL(0.,1.,.9) > MAKE_INTERVAL(0.,1.,.5)) );

    REQUIRE( !(MAKE_INTERVAL(0,0) > MAKE_INTERVAL(0,0)) );
    REQUIRE( !(MAKE_INTERVAL(0,1,0) > MAKE_INTERVAL(0,1,1)) );
}

TEST_CASE( "Less than", "[comparisons]" ) {
	REQUIRE( (MAKE_INTERVAL(0.,1.,.5) < MAKE_INTERVAL(0.,1.,.9)) );

    REQUIRE( !(MAKE_INTERVAL(0,0) < MAKE_INTERVAL(0,0)) );
    REQUIRE( !(MAKE_INTERVAL(0,1,1) < MAKE_INTERVAL(0,1,0)) );
}

TEST_CASE( "Addition Assignment", "[assignment]" ) {
    auto i1 = MAKE_INTERVAL(0., 1., .5);
    REQUIRE( !floatCompareEqual(i1.value, i1.End));
    i1 += i1;
    REQUIRE( floatCompareEqual(i1.value, i1.End));
    i1 += i1;
    REQUIRE( floatCompareEqual(i1.value, i1.End));
}

TEST_CASE( "Subtraction assignment", "[assignment]" ) {
    auto i1 = MAKE_INTERVAL(0., 1., .5);
    REQUIRE( !floatCompareEqual(i1.value, i1.End));
    i1 -= i1;
    REQUIRE( floatCompareEqual(i1.value, 0.));
    i1 -= i1;
    REQUIRE( floatCompareEqual(i1.value, 0.));
}

TEST_CASE( "Multiplication assignment", "[assignment]" ) {
    auto i1 = MAKE_INTERVAL(0., 1., .5);
    REQUIRE( !floatCompareEqual(i1.value, i1.End));
    i1 *= i1;
    REQUIRE( i1.value == .25);
    i1 *= i1;
    REQUIRE( i1.value == .0625);
}

TEST_CASE( "Division assignment", "[assignment]" ) {
    auto i1 = MAKE_INTERVAL(0., 10. ,10.);
    REQUIRE( floatCompareEqual(i1.value, i1.End));
    i1 /= i1;
    constexpr auto var1 = i1.End/i1.End;
    REQUIRE( floatCompareEqual(i1.value, var1));
    i1 /= i1;
    REQUIRE( floatCompareEqual(i1.value, var1/var1));
}

TEST_CASE( "Addition by arithmetic value", "[arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = MAKE_INTERVAL(0, v1+v2, v1);
    i1 += v2;
    REQUIRE( i1.value == v1+v2);
}

TEST_CASE( "Subtraction by arithmetic value", "[arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = MAKE_INTERVAL(0, v1+v2, v2);
    i1 -= v1;
    REQUIRE( i1.value == v2-v1);
}

TEST_CASE( "Multiplication by arithmetic value", "[arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = MAKE_INTERVAL(0, v1*v2, v1);
    i1 *= v2;
    REQUIRE( i1.value == v1*v2);
}

TEST_CASE( "Division by arithmetic value", "[arithmetic]" ){
    constexpr auto v1 = 2., v2 = 3.;
    auto i1 = MAKE_INTERVAL(0., v2, v2);
    i1 /= v1;
    REQUIRE( floatCompareEqual(i1.value, v2/v1));
}

TEST_CASE( "Clamped", "[bounds]" ) {
    constexpr auto v1 = 2;
    auto i1 = MAKE_INTERVAL(0, v1, v1-(v1/2));
    i1 += v1;
    REQUIRE( !i1.Empty);
    REQUIRE( i1.value == i1.End);
}

TEST_CASE( "Wrap", "[bounds]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = MAKE_INTERVAL(0,v2, v2, IntervalWrapModes::Wrap);
    i1 += v1;
    REQUIRE( i1.value == v1%v2);
}
