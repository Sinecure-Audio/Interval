#pragma once

#include "IntervalClass.h"
#include <array>

template<typename T, size_t Size, IntervalWrapModes Mode = IntervalWrapModes::Clamp>
class ArrayWithIntervalRead : public std::array<T, Size>
{
    // using ContainerInterval = decltype(MAKE_INTERVAL(0, Size));
public:
    constexpr ArrayWithIntervalRead() = default;
    // template<typename... Args>
    // constexpr ArrayWithIntervalRead(const Args&... args) : std::array<T, Size>{args...}{}
    
    constexpr ArrayWithIntervalRead(const ArrayWithIntervalRead&) = default;
    constexpr ArrayWithIntervalRead(ArrayWithIntervalRead&&) = default;

    template<typename Start, typename End, IntervalWrapModes M>
    constexpr auto& operator[](const Interval<Start, End, M>& inputInterval)
    {
        const decltype(MAKE_INTERVAL(0, Size-1, 0, Mode)) outputInterval = inputInterval;
        return std::array<T, Size>::operator[](outputInterval.value);
        //return outputInterval.value;
    }

    constexpr auto& operator[](const size_t& index)
    {
        return this->operator[](MAKE_INTERVAL(0, Size-1, index, Mode));
    }

// .at()
    // constexpr auto& operator[]()
    // {

    // };
};

