#pragma once

enum class IntervalWrapModes {
    Clamp,
    Wrap
};

template<typename T>
constexpr auto clampValue(const T& value, const T& lowerBound, const T& upperBound) noexcept {
    if constexpr(__cplusplus >= 201703L)
        return std::clamp(value, lowerBound, upperBound);
    else {
        if (value > upperBound)
            return upperBound;
        else if (value < lowerBound)
            return lowerBound;
    }
}

template<typename T>
constexpr auto wrapValue(const T& value, const T& lowerBound, const T& upperBound) noexcept {
    const auto offset = 0-lowerBound;//might want it to be lower bound
    const auto range = detail::abs(upperBound-lowerBound);

    auto valueToUse = value-offset;
    while(valueToUse < lowerBound)
        valueToUse += range;

    if constexpr(std::is_floating_point_v<T>)
        return std::fmod(valueToUse, range)+offset;
    else
        return (valueToUse % range)+offset;
}