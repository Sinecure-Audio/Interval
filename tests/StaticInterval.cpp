
#include "../include/Interval.h"

#include <catch2/catch.hpp>

//===================
// Math Tests
//===================

TEST_CASE("integralPow 0", "[Maths]") {
    const auto result = detail::iPow(10ull, 0);
    REQUIRE(result == 1);
}

TEST_CASE("integralPow 1", "[Maths]") {
    const auto result = detail::iPow(10ull, 1);
    REQUIRE(result == 10);
}

TEST_CASE("integralPow", "[Maths]") {
    const auto result = detail::iPow(10ull, 2);
	REQUIRE(result == 100);
}

TEST_CASE("integralPow mant 1", "[Maths]") {
    const auto result = detail::iPow(1ull, 30);
    REQUIRE(result == 1);
}

TEST_CASE("integralPow big", "[Maths]") {
    const auto result = detail::iPow(10ull, 3);
    REQUIRE(result == 1000);
}

TEST_CASE("integralPow 2", "[Maths]") {
    const auto result = detail::iPow(2ull, 4);
    REQUIRE(result == 16);
}

TEST_CASE("abs", "[Maths]") {
    const auto neg1 = -1;
    constexpr auto result = detail::abs(neg1);
    REQUIRE(result == 1);
}

TEST_CASE("fmod with integer", "[Maths]") {
    constexpr auto result = detail::fmod(4, 3);
    REQUIRE(result == 1);
}

TEST_CASE("fmod with float", "[Maths]") {
    constexpr auto result = detail::fmod(4.5, 3);
    REQUIRE(result == 1.5);
}

TEST_CASE("round", "[Maths]") {
    constexpr auto result = detail::round(3.6);
    REQUIRE(result == 4);
}

//===================
// Float Constant Tests
//===================

TEST_CASE("Create as Template", "[Float Constant]") {
    constexpr auto result = FloatConstant<true, 0, 0, 0, int>{};
    REQUIRE(result.value == 0);
}

TEST_CASE("Create from Function", "[Float Constant]") {
    constexpr double input = 0.0;
    constexpr auto fcParamTuple = makeFloatConstantImpl(input);
    constexpr auto fc = FloatConstant<std::get<0>(fcParamTuple), std::get<1>(fcParamTuple), std::get<2>(fcParamTuple), std::get<3>(fcParamTuple), decltype(std::get<4>(fcParamTuple))>{};
    constexpr auto result = FloatConstant<true, 0, 0, 0, double>{};
    REQUIRE_THAT(result.value, Catch::WithinRel(fc.value));
    REQUIRE(std::is_same_v<decltype(input), decltype(fc.value)>);
}

TEST_CASE("Create inline from Function", "[Float Constant]") {
    constexpr auto fc = FloatConstant<std::get<0>(makeFloatConstantImpl(0.0)), std::get<1>(makeFloatConstantImpl(0.0)), std::get<2>(makeFloatConstantImpl(0.0)), std::get<3>(makeFloatConstantImpl(0.0)), decltype(std::get<4>(makeFloatConstantImpl(0.0)))>{};
    constexpr auto result = FloatConstant<true, 0, 0, 0, double>{};
    REQUIRE_THAT(result.value, Catch::WithinRel(fc.value));
    REQUIRE(std::is_same_v<decltype(result.value), decltype(fc.value)>);
}


TEST_CASE("Float Constant Decimal Accuracy", "[Float Constant]") {
    constexpr float result = 3.1f;
    constexpr auto fcParamTuple = makeFloatConstantImpl(result);
    constexpr auto fc = FloatConstant<std::get<0>(fcParamTuple), std::get<1>(fcParamTuple), std::get<2>(fcParamTuple), std::get<3>(fcParamTuple), decltype(std::get<4>(fcParamTuple))>{};
    REQUIRE_THAT(result, Catch::WithinRel(fc.value));
    REQUIRE(std::is_same_v<decltype(result), decltype(fc.value)>);
}

TEST_CASE("Float Constant Decimal Accuracy 2", "[Float Constant]") {
    constexpr double result = 3.1415928637892;
    constexpr auto fcParamTuple = makeFloatConstantImpl(result);
    constexpr auto fc = FloatConstant<std::get<0>(fcParamTuple), std::get<1>(fcParamTuple), std::get<2>(fcParamTuple), std::get<3>(fcParamTuple), decltype(std::get<4>(fcParamTuple))>{};
    REQUIRE_THAT(result, Catch::WithinRel(fc.value));
    // REQUIRE(std::is_same_v<decltype(result), decltype(fc.value)>);
}

TEST_CASE("Float Constant Decimal Accuracy 3", "[Float Constant]") {
    constexpr double result = 31415928.637892;
    // constexpr double result = 3.1415928637;
    constexpr auto fcParamTuple = makeFloatConstantImpl(result);
    constexpr auto fc = FloatConstant<std::get<0>(fcParamTuple), std::get<1>(fcParamTuple), std::get<2>(fcParamTuple), std::get<3>(fcParamTuple), decltype(std::get<4>(fcParamTuple))>{};
    REQUIRE_THAT(result, Catch::WithinRel(fc.value));
    REQUIRE(std::is_same_v<decltype(result), decltype(fc.value)>);
}

TEST_CASE("Float Constant Decimal Accuracy 4", "[Float Constant]") {
    constexpr auto result = 3.1415928637892L;
    // constexpr double result = 3.1415928637;
    constexpr auto fcParamTuple = makeFloatConstantImpl(result);
    constexpr auto fc = FloatConstant<std::get<0>(fcParamTuple), std::get<1>(fcParamTuple), std::get<2>(fcParamTuple), std::get<3>(fcParamTuple), decltype(std::get<4>(fcParamTuple))>{};
    REQUIRE_THAT(result, Catch::WithinRel(static_cast<double>(fc.value)));
    REQUIRE(std::is_same_v<decltype(result), decltype(fc.value)>);
}


// //===================
// // Interval Tests
// //===================

TEST_CASE( "Create from Float Constants", "[create]" ) {
    constexpr auto fc0 = FloatConstant<true, 0, 0, 0, double>{};
    constexpr auto fc1 = FloatConstant<true, 1, 0, 0, double>{};

    constexpr auto fcParamTuple0 = makeFloatConstantImpl(0.0);
    constexpr auto fcParamTuple1 = makeFloatConstantImpl(1.0);

    constexpr auto fc2 = FloatConstant<std::get<0>(fcParamTuple0), std::get<1>(fcParamTuple0), std::get<2>(fcParamTuple0), std::get<3>(fcParamTuple0), decltype(std::get<4>(fcParamTuple0))>{};
    constexpr auto fc3 = FloatConstant<std::get<0>(fcParamTuple1), std::get<1>(fcParamTuple1), std::get<2>(fcParamTuple1), std::get<3>(fcParamTuple1), decltype(std::get<4>(fcParamTuple1))>{};

    constexpr auto fc4 = FloatConstant<std::get<0>(makeFloatConstantImpl(0.0)), std::get<1>(makeFloatConstantImpl(0.0)), std::get<2>(makeFloatConstantImpl(0.0)), std::get<3>(makeFloatConstantImpl(0.0)),  decltype(std::get<4>(makeFloatConstantImpl(0.0)))>{};

    Interval<decltype(fc0), decltype(fc1)> interval(0);
    Interval<decltype(fc2), decltype(fc3)> interval2(0);
    Interval<decltype(fc4), decltype(fc1)> interval3(0);
    Interval<FloatConstant<true, 0, 0, 0, double>, FloatConstant<true, 1, 0, 0, double>> interval4(0);

    REQUIRE(interval == interval2);
    REQUIRE(interval == interval3);
    REQUIRE(interval == interval4);
}

TEST_CASE( "Create from 2 arg Macro", "[create]" ) {
    constexpr auto i = MAKE_INTERVAL(0, 1);
    (void) i;
}

TEST_CASE( "Create from 3 arg Macro", "[create]" ) {
    constexpr auto i = MAKE_INTERVAL(0,1,0);
    (void) i;
}

TEST_CASE( "Create from 4 arg Macro With Clamp", "[create]" ) {
   constexpr auto i = MAKE_INTERVAL(0.0,1.0,0.0, IntervalWrapModes::Clamp);
   (void) i;
}

TEST_CASE( "Create from 4 arg Macro With Wrap", "[create]" ) {
   constexpr auto i = MAKE_INTERVAL(0.0,1.0,0.0, IntervalWrapModes::Wrap);
   (void) i;
}

TEST_CASE( "Static Interval Empty", "[identities]" ) {
    REQUIRE( MAKE_INTERVAL(0,0).Empty );
}

TEST_CASE( "Static Interval Not Empty", "[identities]" ) {
    REQUIRE( !MAKE_INTERVAL(0,0.00001).Empty );
}

TEST_CASE( "Static Interval Reverse", "[identities]" ) {
    REQUIRE( !MAKE_INTERVAL(0, 0).Reverse );
    REQUIRE(  MAKE_INTERVAL(1, 0).Reverse );
    REQUIRE( !MAKE_INTERVAL(0, 1).Reverse );
}

TEST_CASE( "Static Interval Size", "[Static Interval Identities]" ) {
    constexpr auto v1 = 0., v2 = 8.;
    REQUIRE(MAKE_INTERVAL(v1, v2).Length == v2-v1);
}

TEST_CASE( "Static Interval Same Interval Assignment", "[identities]" ) {
    constexpr auto v1 = 0, v2 = 10, v3 = 3, v4 = 8;
    auto i1 = MAKE_INTERVAL(v1, v2, v3);
    auto i2 = MAKE_INTERVAL(v1, v2, v4);
    i1 = i2;
    REQUIRE( i1 == i2 );
    REQUIRE( i1.getValue() == v4 );
}

TEST_CASE( "Static Interval Same Interval Different Type Assignment", "[identities]" ) {
    constexpr auto v1 = 0, v2 = 10, v3 = 3, v4 = 8;
    auto i1 = MAKE_INTERVAL(v1, v2, v3);
    auto i2 = MAKE_INTERVAL(float(v1), float(v2), float(v4));
    i1 = i2;
    REQUIRE(i1.getValue() == v4);
}

TEST_CASE( "Static Interval Different Interval Assignment", "[identities]" ) {
    constexpr auto v1 = 0., v2 = 10., v3 = 3.;
    auto i1 = MAKE_INTERVAL(v1, v3, v3/2);
    auto i2 = MAKE_INTERVAL(v1, v2, v2/2);
    i1 = i2;
    auto value = (v2/2)*((v3-v1)/(v2-v1));
    REQUIRE(i1.getValue() == value);
}

TEST_CASE( "Static Interval Different Interval Assignment 2", "[identities]" ) {
    constexpr auto v1 = 0., v2 = 100., v3 = 1.0, v4 = .5;
    auto i1 = MAKE_INTERVAL(v1, v2);
    auto i2 = MAKE_INTERVAL(v1, v3, v4);
    i1 = i2;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(v4*v2/v3));
}

TEST_CASE( "Static Interval Value Assignment", "[identities]" ) {
    constexpr auto v1 = 0, v2 = 10, v3 = 3;
    auto i1 = MAKE_INTERVAL(v1, v2);
    i1 = v3;
    REQUIRE( i1.getValue() == v3 );
}

TEST_CASE( "Bounds Comparisons", "[Static Interval][identities]" ) {
    REQUIRE( MAKE_INTERVAL(0,0).hasSameBoundsAs(MAKE_INTERVAL(0,0)) );
    REQUIRE( MAKE_INTERVAL(0,1).hasSameBoundsAs(MAKE_INTERVAL(0,1)) );
    
    REQUIRE( !MAKE_INTERVAL(0,0).hasSameBoundsAs(MAKE_INTERVAL(0,1)) );
    REQUIRE( !MAKE_INTERVAL(1,2).hasSameBoundsAs(MAKE_INTERVAL(0,1)) );
}

TEST_CASE( "Interval Indentical", "[Static Interval][identities]" ) {
    REQUIRE( MAKE_INTERVAL(0,0).isIdenticalTo(MAKE_INTERVAL(0,0)) );
    REQUIRE( MAKE_INTERVAL(0,1).isIdenticalTo(MAKE_INTERVAL(0,1)) );

    REQUIRE( !MAKE_INTERVAL(0,0).isIdenticalTo(MAKE_INTERVAL(0,1)) );
    REQUIRE( !MAKE_INTERVAL(1,2).isIdenticalTo(MAKE_INTERVAL(0,1)) );
}

TEST_CASE( "Interval Equality", "[Static Interval][identities]" ) {
    REQUIRE( (MAKE_INTERVAL(0,0) == MAKE_INTERVAL(0,0)) );
    REQUIRE( (MAKE_INTERVAL(0,1) == MAKE_INTERVAL(0,1)) );
    REQUIRE( (MAKE_INTERVAL(0.0,1.0, .5) == MAKE_INTERVAL(0.0,1.0, .5)) );
    REQUIRE( !(MAKE_INTERVAL(0.0,1.0,.3) == MAKE_INTERVAL(0.0,1.0, .5)) );

    REQUIRE( (MAKE_INTERVAL(0.0,0.001) == MAKE_INTERVAL(0.0,0.0)) );
    REQUIRE( (MAKE_INTERVAL(0.0,0.001) == MAKE_INTERVAL(0.0,1.0)) );
 
    REQUIRE( (MAKE_INTERVAL(0.0,0.0) != MAKE_INTERVAL(1.0,2.0, 1.5)) );
    REQUIRE( (MAKE_INTERVAL(0.0,1.0) != MAKE_INTERVAL(1.0,2.0, 1.5)) );

    constexpr auto a = MAKE_INTERVAL(0.0, 2.0, 1.0);
    constexpr auto b = MAKE_INTERVAL(0.0, 1.0, 0.5);
    REQUIRE_THAT( (a - b).getValue(), Catch::WithinRel(0.0));
    REQUIRE(!a.isIdenticalTo(b));
}

TEST_CASE( "Static Interval Greater than", "[comparisons]" ) {
    REQUIRE( (MAKE_INTERVAL(0.,1.,.9) > MAKE_INTERVAL(0.,1.,.5)) );
    REQUIRE( !(MAKE_INTERVAL(0.0, 2.0, 1.0) > MAKE_INTERVAL(0., 1., .5)) );

    REQUIRE( !(MAKE_INTERVAL(0,0) > MAKE_INTERVAL(0,0)) );
    REQUIRE( !(MAKE_INTERVAL(0,1,0) > MAKE_INTERVAL(0,1,1)) );
}

TEST_CASE( "Static Interval Less than", "[comparisons]" ) {
	REQUIRE( (MAKE_INTERVAL(0.,1.,.5) < MAKE_INTERVAL(0.,1.,.9)) );
    REQUIRE( !(MAKE_INTERVAL(0.0, 1.0, 0.5) < MAKE_INTERVAL(0., 2., 1.0 )) );


    REQUIRE( !(MAKE_INTERVAL(0,0) < MAKE_INTERVAL(0,0)) );
    REQUIRE( !(MAKE_INTERVAL(0,1,1) < MAKE_INTERVAL(0,1,0)) );
}

TEST_CASE( "Static Interval Addition Assignment", "[assignment]" ) {
    auto i1 = MAKE_INTERVAL(0., 1., .5);
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(.5));
    i1 += i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(1.0));
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(i1.End));
    i1 += i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(i1.End));
}

TEST_CASE( "Static Interval Subtraction assignment", "[assignment]" ) {
    auto i1 = MAKE_INTERVAL(0., 1., .5);
    REQUIRE(i1.getValue() != i1.End);
    i1 -= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(0.0));
    i1 -= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(0.0));
}

TEST_CASE( "Static Interval Multiplication assignment", "[assignment]" ) {
    auto i1 = MAKE_INTERVAL(0., 1., .5);
    REQUIRE(i1.getValue() != i1.End);
    i1 *= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(.25));
    i1 *= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(.0625));
}

TEST_CASE( "Static Interval Division assignment", "[assignment]" ) {
    auto i1 = MAKE_INTERVAL(0., 10., 10.);
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(i1.End));
    i1 /= i1;
    constexpr auto var1 = i1.End/i1.End;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(var1));
    i1 /= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(var1/var1));
}

TEST_CASE( "Increment Accuracy", "[assignment]" ) {
    const auto numberOfIterations = 99999;
    double initialValue = 1.0;
    auto arithmeticInterval = MAKE_INTERVAL(0.0, 105.0, initialValue, IntervalWrapModes::Wrap);
    // const double scaledVal = initialValue/double(numberOfIterations);
    for(auto val = 0; val < numberOfIterations; ++val) {
        initialValue >= 105.0 ? initialValue -= 105.0 : 0.0;
        initialValue += .5;
        arithmeticInterval += .5;
        REQUIRE_THAT(arithmeticInterval.getValue(), Catch::WithinRel(initialValue));  
    }  
}

TEST_CASE( "Decrement Accuracy", "[assignment]" ) {
    const auto numberOfIterations = 99999;
    double initialValue = 1.0;
    auto arithmeticInterval = MAKE_INTERVAL(0.0, 105.0, initialValue, IntervalWrapModes::Wrap);
    // const double scaledVal = initialValue/double(numberOfIterations);
    for(auto val = 0; val < numberOfIterations; ++val) {
        initialValue -= .5;
        initialValue < 0.0 ? initialValue += 105.0 : 0.0;
        arithmeticInterval -= .5;
        REQUIRE_THAT(arithmeticInterval.getValue(), Catch::WithinRel(initialValue));  
    }  
}

TEST_CASE( "Static Interval Addition by arithmetic value", "[arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = MAKE_INTERVAL(0, v1+v2, v1);
    i1 += v2;
    REQUIRE( i1.getValue() == v1+v2);
}

TEST_CASE( "Static Interval Subtraction by arithmetic value", "[arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = MAKE_INTERVAL(0, v1+v2, v2);
    i1 -= v1;
    REQUIRE( i1.getValue() == v2-v1);
}

TEST_CASE( "Static Interval Multiplication by arithmetic value", "[arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = MAKE_INTERVAL(0, v1*v2, v1);
    i1 *= v2;
    REQUIRE( i1.getValue() == v1*v2);
}

TEST_CASE( "Static Interval Division by arithmetic value", "[arithmetic]" ){
    constexpr auto v1 = 2., v2 = 3.;
    auto i1 = MAKE_INTERVAL(0., v2, v2);
    i1 /= v1;
    REQUIRE(i1.getValue() == v2/v1);
}

TEST_CASE( "Static Interval Clamped", "[bounds]" ) {
    constexpr auto v1 = 2;
    auto i1 = MAKE_INTERVAL(0, v1, v1-(v1/2));
    i1 += v1;
    REQUIRE( !i1.Empty);
    REQUIRE( i1.getValue() == i1.End);
}

TEST_CASE( "Static Interval Wrap", "[bounds]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = MAKE_INTERVAL(0,v2, v2, IntervalWrapModes::Wrap);
    i1 += v1;
    REQUIRE( i1.getValue() == v1%v2);
}
