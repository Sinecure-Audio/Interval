#include <numeric>

#include "../include/Interval.h"

#include "catch.hpp"

//===================
// Static Container Tests
//===================


TEST_CASE( "Static Container Different Range", "[Container]" ) {
    ArrayWithIntervalRead<int, 4096> arr;
    ArrayWithIntervalRead<int, 4096> arr2{{0}};// check to see if direct init works
    std::iota(arr.begin(), arr.end(), 0);
    constexpr auto v1 = .25;
    constexpr auto v2 = 1;
    constexpr auto i1 = MAKE_INTERVAL(0., 1., v1);
    arr[static_cast<size_t>(static_cast<double>((arr.size()-1))*v1)] = v2;
    REQUIRE( decltype(arr)::IntervalType{i1}.getValue() == static_cast<size_t>((arr.size()-1)*.25));
    // for(int i = 0; i < arr.size()-1; ++i){
        REQUIRE( arr[i1] == arr[static_cast<size_t>((arr.size()-1)*.25)]);
        REQUIRE( arr.at(i1) == arr[static_cast<size_t>((arr.size()-1)*.25)]);
        // REQUIRE( arr[i1] == arr[static_cast<size_t>((arr.size()-1)*.25)]);
    // }    
    (void)arr2;
}

TEST_CASE( "Static Container Clamped Read", "[Container]" ) {
    ArrayWithIntervalRead<int, 4096> arr;
    std::iota(arr.begin(), arr.end(), 0);
    constexpr auto i1 = MAKE_INTERVAL(size_t(0), size_t(10000), size_t(10000));
    arr[arr.size()-1] = 1;
    REQUIRE(decltype(arr)::IntervalType{i1}.getValue() == arr.size()-1);
    REQUIRE(arr[i1] == 1);
    REQUIRE(arr.at(i1) == 1);
}

TEST_CASE( "Static Container Wrapped Read", "[Container]" ) {
    ArrayWithIntervalRead<int, 4096> arr;
    std::iota(arr.begin(), arr.end(), 0);
    constexpr auto i1 = MAKE_INTERVAL(0, arr.size()-1, arr.size()+1, IntervalWrapModes::Wrap);
    REQUIRE( arr[i1] == 2);
    REQUIRE( arr.at(i1) == 2);
}

TEST_CASE( "Static Container Wrapped Read 2", "[Container]" ) {
    ArrayWithIntervalRead<int, 4096, IntervalWrapModes::Wrap> arr;
    std::iota(arr.begin(), arr.end(), 0);
    constexpr auto i1 = MAKE_INTERVAL(0, arr.size()-1, arr.size()+1, IntervalWrapModes::Wrap);
    REQUIRE( arr[i1] == 2);
    REQUIRE( arr.at(i1) == 2);
    REQUIRE( i1 == 2);  
    REQUIRE( arr[2] == arr[arr.size()+1]);
    REQUIRE( arr.at(2) == arr.at(arr.size()+1));
}

//===================
// Dynamic Container Tests
//===================


TEST_CASE( "Dynamic Container Different Range", "[DynamicInterval][Container]" ) {
    VectorWithIntervalRead<int> vec;
    vec.resize(4096);
    std::iota(vec.begin(), vec.end(), 0);
    constexpr auto v1 = .25;
    constexpr auto v2 = 1;
    auto i1 = DynamicInterval<double>(0., 1., .25);//MAKE_INTERVAL(0., 1., .25);
    vec[static_cast<size_t>(static_cast<double>(vec.size()-1)*v1)] = v2;
    REQUIRE( static_cast<size_t>(i1.getValue()*(vec.size()-1)) == static_cast<size_t>(static_cast<double>(vec.size()-1)*v1));
    REQUIRE( vec[i1] == v2);
    REQUIRE( vec.at(i1) == v2);
}

TEST_CASE( "Dynamic Container Clamped Read", "[DynamicInterval][Container]" ) {
    VectorWithIntervalRead<int> vec;
    vec.resize(4096);
    std::iota(vec.begin(), vec.end(), 0);
    const auto i1 = DynamicInterval<double>(0, 10000, 10000);
    vec[4095] = 1;
    REQUIRE( vec[i1] == 1);
    REQUIRE( vec.at(i1) == 1);
}

TEST_CASE( "Dynamic Container Wrapped Read", "[DynamicInterval][Container]" ) {
    VectorWithIntervalRead<size_t> vec;
    vec.resize(4096);
    std::iota(vec.begin(), vec.end(), 0);
    const auto i1 = DynamicInterval<size_t>(0, vec.size()-1, 4097, IntervalWrapModes::Wrap);
    REQUIRE( vec[i1] == 2);
    REQUIRE( vec.at(i1) == 2);
}

TEST_CASE( "Dynamic Container Wrapped Read 2", "[DynamicInterval][Container]" ) {
    VectorWithIntervalRead<size_t, IntervalWrapModes::Wrap> vec;
    vec.resize(4096);
    std::iota(vec.begin(), vec.end(), 0);
    const auto i1 = DynamicInterval<size_t>(0, vec.size()-1, 4097, IntervalWrapModes::Wrap);
    REQUIRE( vec[4097] == 2);
    REQUIRE( vec.at(4097) == 2);
    REQUIRE( vec[i1] == 2);
    REQUIRE( vec.at(i1) == 2);
    REQUIRE( vec[4096] == 1);
    REQUIRE( vec.at(4096) == 1);
    REQUIRE( vec[1] == vec[4096]);
    REQUIRE( vec.at(1) == vec[4096]);
}

TEST_CASE( "Resize", "[DynamicInterval][Vector Functionality]" ) {
    VectorWithIntervalRead<size_t, IntervalWrapModes::Clamp> vec;
    const auto i1 = DynamicInterval<double>(0, 1.0, 1.0, IntervalWrapModes::Clamp);
    vec.resize(1024);
    std::iota(vec.begin(), vec.end(), 0);
    REQUIRE(vec[i1] == vec.size()-1);
    vec.resize(4096);
    std::iota(vec.begin(), vec.end(), 0);
    REQUIRE(vec[i1] == vec.size()-1);
}

TEST_CASE( "Push Back", "[DynamicInterval][Vector Functionality]" ) {
    VectorWithIntervalRead<size_t, IntervalWrapModes::Clamp> vec;
    const auto i1 = DynamicInterval<double>(0, 1.0, 1.0, IntervalWrapModes::Clamp);
    vec.push_back(1);
    REQUIRE(vec[i1] == 1);
}

TEST_CASE( "Pop Back", "[DynamicInterval][Vector Functionality]" ) {
    VectorWithIntervalRead<size_t, IntervalWrapModes::Clamp> vec;
    const auto i1 = DynamicInterval<double>(0, 1.0, 1.0, IntervalWrapModes::Clamp);
    vec.push_back(1);
    vec.push_back(1);
    vec.pop_back();
    REQUIRE(vec[i1] == vec[i1]);
    REQUIRE(vec.size() == 1);
}

TEST_CASE( "Emplace", "[DynamicInterval][Vector Functionality]" ) {
    VectorWithIntervalRead<size_t, IntervalWrapModes::Clamp> vec;
    const auto i1 = DynamicInterval<double>(0, 1.0, 1.0, IntervalWrapModes::Clamp);
    vec.emplace(vec.cbegin(), 1);
    REQUIRE(vec[i1] == 1);
    REQUIRE(vec.size() == 1);
}

TEST_CASE( "Emplace Back", "[DynamicInterval][Vector Functionality]" ) {
    VectorWithIntervalRead<size_t, IntervalWrapModes::Clamp> vec;
    const auto i1 = DynamicInterval<double>(0, 1.0, 1.0, IntervalWrapModes::Clamp);
    vec.emplace_back(1);
    REQUIRE(vec[i1] == 1);
}

TEST_CASE( "Assign 1", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Assign 2", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Assign 3", "[DynamicInterval][Vector Functionality]" ) {

}


TEST_CASE( "Insert 1", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Insert 2", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Insert 3", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Insert 4", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Insert 5", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Erase 1", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Erase 2", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Swap 1", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Swap 2", "[DynamicInterval][Vector Functionality]" ) {

}

TEST_CASE( "Clear", "[DynamicInterval][Vector Functionality]" ) {
    VectorWithIntervalRead<size_t, IntervalWrapModes::Wrap> vec;
    vec.resize(4096);
    std::iota(vec.begin(), vec.end(), 0);
    vec.clear();
    REQUIRE(vec.size() == 0);
}
