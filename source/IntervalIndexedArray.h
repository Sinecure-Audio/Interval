#pragma once

#include "StaticInterval.h"
#include <array>

template<typename T, size_t Size, IntervalWrapModes Mode = IntervalWrapModes::Clamp>
class ArrayWithIntervalRead : public std::array<T, Size>
{
public:
    using ContainerType = std::array<T, Size>;
    using IntervalType = INTERVAL_TYPE(size_t(0), size_t(Size-1), Mode);

    constexpr auto&       operator[](const IntervalType& inputInterval) noexcept       { return std::array<T, Size>::operator[](inputInterval.getValue()); }
    constexpr const auto& operator[](const IntervalType& inputInterval) const noexcept { return std::array<T, Size>::operator[](inputInterval.getValue()); }

    constexpr auto&       operator[](const typename ContainerType::size_type& index) noexcept       { return this->operator[](IntervalType{index}); }
    constexpr const auto& operator[](const typename ContainerType::size_type& index) const noexcept { return this->operator[](IntervalType{index}); }

    constexpr auto&       at(const IntervalType& inputInterval) noexcept       { return std::array<T, Size>::operator[](inputInterval.getValue()); }
    constexpr const auto& at(const IntervalType& inputInterval) const noexcept { return std::array<T, Size>::operator[](inputInterval.getValue()); }

    constexpr auto&       at(const typename ContainerType::size_type& index) noexcept       { return this->operator[](IntervalType{index}); }
    constexpr const auto& at(const typename ContainerType::size_type& index) const noexcept { return this->operator[](IntervalType{index}); }
};

template<typename T, size_t Size>
using BoundedArray = ArrayWithIntervalRead<T, Size, IntervalWrapModes::Clamp>;

template<typename T, size_t Size>
using PeriodicArray = ArrayWithIntervalRead<T, Size, IntervalWrapModes::Wrap>;
