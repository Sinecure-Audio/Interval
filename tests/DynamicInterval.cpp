#include "../include/Interval.h"

#include <catch2/catch.hpp>

//===================
// Dynamic Interval Tests
//===================

TEST_CASE( "Dynamic Interval Size", "[Dynamic Interval][identities]" ) {
    constexpr auto v1 = 0., v2 = 8.;
    DynamicInterval<double> i(v1, v2, v1);
    REQUIRE_THAT(i.getLength(), Catch::WithinRel(v2-v1));
}

TEST_CASE( "Same Interval Assignment", "[DynamicInterval][identities]" ) {
    constexpr auto v1 = 0, v2 = 10, v3 = 3, v4 = 8;
    DynamicInterval<double> i1(v1, v2, v3);
    DynamicInterval<double> i2(v1, v2, v4);
    i1 = i2;
    REQUIRE( i1 == i2 );
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(static_cast<double>(v4)));
}

TEST_CASE( "Same Interval Different Type Assignment", "[DynamicInterval][identities]" ) {
    constexpr auto v1 = 0, v2 = 10, v3 = 3;
    constexpr auto v4 = 8.0;
    DynamicInterval<int> i1(v1, v2, v3);
    DynamicInterval<double> i2(v1, v2, v4);
    i1 = i2;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(v4));
}

TEST_CASE( "Different Interval Assignment", "[DynamicInterval][identities]" ) {
    constexpr auto v1 = 0., v2 = 10., v3 = 3.;
    DynamicInterval<double> i1(v1, v3, v3/2);
    DynamicInterval<double> i2(v1, v2, v2/2);
    i1 = i2;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(i2.getValue()));
    REQUIRE_THAT(i1.getStart(), Catch::WithinRel(i2.getStart()));
    REQUIRE_THAT(i1.getEnd(), Catch::WithinRel(i2.getEnd()));
}

TEST_CASE( "Different Interval Assignment 2", "[DynamicInterval][identities]" ) {
    DynamicInterval<double> i1(0,100,0);
    DynamicInterval<double> i2(0,1.0,0.5);
    i1 = i2;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(i2.getValue()));
    REQUIRE_THAT(i1.getStart(), Catch::WithinRel(i2.getStart()));
    REQUIRE_THAT(i1.getEnd(), Catch::WithinRel(i2.getEnd()));
}

TEST_CASE( "Value Assignment", "[DynamicInterval][identities]" ) {
    constexpr auto v1 = 0.0, v2 = 10.0, v3 = 3.0;
    DynamicInterval<double> i1(v1, v2);
    i1 = v3;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(v3));
    REQUIRE_THAT(i1.getStart(), Catch::WithinRel(v1));
    REQUIRE_THAT(i1.getEnd(), Catch::WithinRel(v2));
}

TEST_CASE( "Different Type Value Assignment", "[DynamicInterval][identities]" ) {
    constexpr int v1 = 0, v2 = 10;
    constexpr auto v3 = 3.0;
    DynamicInterval<double> i1(v1, v2);
    i1 = 3;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(v3));
    REQUIRE_THAT(i1.getStart(), Catch::WithinRel(0.0));
    REQUIRE_THAT(i1.getEnd(), Catch::WithinRel(10.0));
}

TEST_CASE( "Dynamic Interval Bounds Comparisons", "[DynamicInterval][identities]" ) {
    REQUIRE( DynamicInterval<double>(0,0).hasSameBoundsAs(DynamicInterval<double>(0,0)) );
    REQUIRE( DynamicInterval<double>(0,1).hasSameBoundsAs(DynamicInterval<double>(0,1)) );
    
    REQUIRE( !DynamicInterval<double>(0,0).hasSameBoundsAs(DynamicInterval<double>(0,1)) );
    REQUIRE( !DynamicInterval<double>(1,2).hasSameBoundsAs(DynamicInterval<double>(0,1)) );
}

TEST_CASE( "Dynamic Interval Indentical", "[DynamicInterval][identities]" ) {
    REQUIRE( DynamicInterval<double>(0,0).isIdenticalTo(DynamicInterval<double>(0,0)) );
    REQUIRE( DynamicInterval<double>(0,1).isIdenticalTo(DynamicInterval<double>(0,1)) );

    REQUIRE( !DynamicInterval<double>(0,0).isIdenticalTo(DynamicInterval<double>(0,1)) );
    REQUIRE( !DynamicInterval<double>(1,2).isIdenticalTo(DynamicInterval<double>(0,1)) );
}


TEST_CASE( "Equality", "[DynamicInterval][comparisons]" ) {
    REQUIRE( (DynamicInterval<double>(0,0) == DynamicInterval<double>(0,0)) );
    REQUIRE( (DynamicInterval<double>(0,1) == DynamicInterval<double>(0,1)) );
    REQUIRE( (DynamicInterval<double>(0.0,1.0, .5) == DynamicInterval<double>(0.0,1.0, .5)) );
    REQUIRE( !(DynamicInterval<double>(0.0,1.0,.3) == DynamicInterval<double>(0.0,1.0, .5)) );

    REQUIRE( !(DynamicInterval<double>(0.0,1.0,.3) == DynamicInterval<double>(0.0,3.0, .5)) );
    REQUIRE( (DynamicInterval<double>(0.0,1.0,.5) == DynamicInterval<double>(0.0,2.0, 1.0)) );

    REQUIRE( (DynamicInterval<double>(0,0) == DynamicInterval<double>(0, .000000001)) );
    REQUIRE( (DynamicInterval<double>(0,1) == DynamicInterval<double>(0, .000000001)) ); 
    REQUIRE( !(DynamicInterval<double>(0,0) != DynamicInterval<double>(0,0)) );
    REQUIRE( (DynamicInterval<double>(0,2,1) != DynamicInterval<double>(0,1,1)));
}

TEST_CASE( "Greater than", "[DynamicInterval][comparisons]" ) {
    REQUIRE( (DynamicInterval<double>(0.,1.,.9) > DynamicInterval<double>(0.,1.,.5)) );
    REQUIRE( (DynamicInterval<double>(0.,10.,.9) > DynamicInterval<double>(0.,100.,5.)) );

    REQUIRE( !(DynamicInterval<double>(0,0) > DynamicInterval<double>(0,0)) );
    REQUIRE( (DynamicInterval<double>(0,1,1) > DynamicInterval<double>(0,1,0)) );
}

TEST_CASE( "Greater than or Equal", "[DynamicInterval][comparisons]" ) {
    REQUIRE( (DynamicInterval<double>(0.,1.,.9) >= DynamicInterval<double>(0.,1.,.5)) );
    REQUIRE( (DynamicInterval<double>(0.,1.,.9) >= DynamicInterval<double>(0.,1.,.9)) );

    REQUIRE( (DynamicInterval<double>(0,0) >= DynamicInterval<double>(0,0)) );
    REQUIRE( (DynamicInterval<double>(0,1,1) >= DynamicInterval<double>(0,1,0)) );
}

TEST_CASE( "Less than", "[DynamicInterval][comparisons]" ) {
	REQUIRE( (DynamicInterval<double>(0.,1.,.5) < DynamicInterval<double>(0.,1.,.9)) );

    REQUIRE( !(DynamicInterval<double>(0,0) < DynamicInterval<double>(0,0)) );
    REQUIRE( (DynamicInterval<double>(0,1,0) < DynamicInterval<double>(0,1,1)) );
}

TEST_CASE( "Less than or Equal", "[DynamicInterval][comparisons]" ) {
    REQUIRE( (DynamicInterval<double>(0.,1.,.5) <= DynamicInterval<double>(0.,1.,.9)) );

    REQUIRE( (DynamicInterval<double>(0.,1.,.5) <= DynamicInterval<double>(0.,1.,.5)) );

    REQUIRE( (DynamicInterval<double>(0,0) <= DynamicInterval<double>(0,0)) );
    REQUIRE( (DynamicInterval<double>(0,1,0) <= DynamicInterval<double>(0,1,1)) );
}

TEST_CASE( "Addition Assignment", "[DynamicInterval][assignment]" ) {
    auto i1 = DynamicInterval<double>(0., 1., .5);
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(.5));
    i1 += i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(1.0));
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(i1.getEnd()));
    i1 += i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(i1.getEnd()));
}

TEST_CASE( "Subtraction assignment", "[DynamicInterval][assignment]" ) {
    auto i1 = DynamicInterval<double>(0., 1., .5);
    REQUIRE(i1.getValue() != i1.getEnd());
    i1 -= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(0.0));
    i1 -= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(0.0));
}

TEST_CASE( "Multiplication assignment", "[DynamicInterval][assignment]" ) {
    auto i1 = DynamicInterval<double>(0., 1., .5);
    REQUIRE(i1.getValue() != i1.getEnd());
    i1 *= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(0.25));
    i1 *= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(0.0625));
}

TEST_CASE( "Division assignment", "[DynamicInterval][assignment]" ) {
    auto i1 = DynamicInterval<double>(0., 10. ,10.);
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(i1.getEnd()));
    i1 /= i1;
    const auto var1 = i1.getEnd()/i1.getEnd();
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(var1));
    i1 /= i1;
    REQUIRE_THAT(i1.getValue(), Catch::WithinRel(var1/var1));
}

TEST_CASE( "Addition by arithmetic value", "[DynamicInterval][arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = DynamicInterval<double>(0, v1+v2, v1);
    i1 += v2;
    REQUIRE_THAT( i1.getValue(), Catch::WithinRel(static_cast<double>(v1+v2)));
}

TEST_CASE( "Subtraction by arithmetic value", "[DynamicInterval][arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = DynamicInterval<double>(0, v1+v2, v2);
    i1 -= v1;
    REQUIRE_THAT( i1.getValue(), Catch::WithinRel(static_cast<double>(v2-v1)));
}

TEST_CASE( "Multiplication by arithmetic value", "[DynamicInterval][arithmetic]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = DynamicInterval<double>(0, v1*v2, v1);
    i1 *= v2;
    REQUIRE_THAT( i1.getValue(), Catch::WithinRel(static_cast<double>(v1*v2)));
}

TEST_CASE( "Division by arithmetic value", "[DynamicInterval][arithmetic]" ){
    constexpr auto v1 = 2., v2 = 3.;
    auto i1 = DynamicInterval<double>(0., v2, v2);
    i1 /= v1;
    REQUIRE_THAT( i1.getValue(), Catch::WithinRel(static_cast<double>(v2/v1)));
}

TEST_CASE( "Clamped", "[DynamicInterval][bounds]" ) {
    constexpr auto v1 = 2;
    auto i1 = DynamicInterval<double>(0, v1, v1-(v1/2));
    i1 += v1;
    REQUIRE( !i1.isEmpty());
    REQUIRE_THAT( i1.getValue(), Catch::WithinRel(i1.getEnd()));
}

TEST_CASE( "Wrap", "[DynamicInterval][bounds]" ) {
    constexpr auto v1 = 2, v2 = 3;
    auto i1 = DynamicInterval<double>(0,v2, v2, IntervalWrapModes::Wrap);
    i1 += v1;
    REQUIRE_THAT( i1.getValue(), Catch::WithinRel(static_cast<double>(v1%v2)));
}
