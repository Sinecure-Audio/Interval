
#include "../include/Interval.h"

#define CATCH_CONFIG_MAIN	
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "../tests/catch.hpp"

// std::uint64_t Fibonacci(std::uint64_t number) {
//     return number < 2 ? 1 : Fibonacci(number - 1) + Fibonacci(number - 2);
// }

// TEST_CASE("Fibonacci") {
//     CHECK(Fibonacci(0) == 1);
//     // some more asserts..
//     CHECK(Fibonacci(5) == 8);
//     // some more asserts..

//     // now let's benchmark:
//     BENCHMARK("Fibonacci 20") {
//         volatile auto x = Fibonacci(30);
//     };
// }

// int doSomething(){
//     return 1+2;
// }

volatile void cacheFlush()
{
     const int size = 20*1024*1024; // Allocate 20M. Set much larger then L2
     char *c = (char *)malloc(size);
     for (int i = 0; i < 0xff; i++)
       for (int j = 0; j < size; j++)
         c[j] = i*j;

     free(c);
}

TEST_CASE("Add", "[Interval Benchmarks]") {
    constexpr auto i1 = MAKE_INTERVAL(0., 1., .25);
    constexpr auto i2 = MAKE_INTERVAL(0, 4095, 4095*.25);
    constexpr int i4 = 1; 

    // now let's benchmark:
    ArrayWithIntervalRead<int, 4096> iArray{0};
    std::array<int, 4096> arr{0};

    cacheFlush();

    BENCHMARK("Simple Assignment") {
        // volatile auto x = (1+4);//(i1 + i4);
        volatile auto x = 0.0;
    };

    cacheFlush();

    BENCHMARK("Simple Assignment") {
        // volatile auto x = (1+4);//(i1 + i4);
        volatile auto x = MAKE_INTERVAL(0.0, 1.0, .5).getValue();
    };

    // BENCHMARK("Constant Lookup std") {
    //     // volatile auto x = (1+4);//(i1 + i4);
    //     volatile auto x = arr[0];
    // };

    // cacheFlush();

    // BENCHMARK("Constant Lookup index") {
    //     // volatile auto x = (1+4);//(i1 + i4);
    //     volatile auto x = iArray[0];
    // };

    // cacheFlush();

    // BENCHMARK("Index Lookup std") {
    //     // volatile auto x = (1+4);//(i1 + i4);
    //     volatile auto x = arr[i2.getValue()];
    // };

    // cacheFlush();

    // BENCHMARK("Index Lookup index 1") {
    //     // volatile auto x = (1+4);//(i1 + i4);
    //     volatile auto x = iArray[i2.getValue()];
    // };

    // cacheFlush();

    // BENCHMARK("Index Lookup index 2") {
    //     // volatile auto x = (1+4);//(i1 + i4);
    //     volatile auto x = iArray[i2];
    // };

    // cacheFlush();

    // BENCHMARK("Scaled Index Lookup std") {
    //     // volatile auto x = (1+4);//(i1 + i4);
    //     volatile auto x = arr[i1.getValue()*arr.size()];
    // };

    // cacheFlush();

    // BENCHMARK("Scaled Index Lookup index") {
    //     // volatile auto x = (1+4);//(i1 + i4);
    //     volatile auto x = iArray[i1];
    // };

    // BENCHMARK("Add Intervals") {
    //     volatile auto x = i1 + i2;
    // };

    // BENCHMARK("Add Intervals") {
    //     volatile auto x =  i1+=i4;
    // };

    // BENCHMARK("Add Intervals") {
    //     volatile auto x =  i1+=i4;
    // };
}