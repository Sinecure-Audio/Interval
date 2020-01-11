#pragma once

enum class IntervalWrapModes {
    Clamp,
    Wrap
};

template<typename T>
constexpr auto clampValue(const T& value, const T& lowerBound, const T& upperBound) noexcept {
    if constexpr(__cplusplus >= 201703L)
        return std::clamp(value, lowerBound, upperBound);
    else 
        return (upperBound * (value > upperBound)) + (lowerBound * (value < lowerBound)) + (value * (value > upperBound)+(value < lowerBound));
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


#define GLUE(x, y) x y

#define RETURN_ARG_COUNT(_1_, _2_, _3_, _4_, _5_, count, ...) count
#define EXPAND_ARGS(args) RETURN_ARG_COUNT args
#define COUNT_ARGS_MAX5(...) EXPAND_ARGS((__VA_ARGS__, 5, 4, 3, 2, 1, 0))

#define OVERLOAD_MACRO2(name, count) name##_##count
#define OVERLOAD_MACRO1(name, count) OVERLOAD_MACRO2(name, count)
#define OVERLOAD_MACRO(name, count) OVERLOAD_MACRO1(name, count)

#define CALL_OVERLOAD(name, ...) GLUE(OVERLOAD_MACRO(name, COUNT_ARGS_MAX5(__VA_ARGS__)), (__VA_ARGS__))

#define MAKE_INTERVAL_2(start, end) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start)), decltype(std::get<4>(makeFloatConstantImpl(start)))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end)), decltype(std::get<4>(makeFloatConstantImpl(end)))>>{}

#define MAKE_INTERVAL_3(start, end, default) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start)), decltype(std::get<4>(makeFloatConstantImpl(start)))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end)), decltype(std::get<4>(makeFloatConstantImpl(end)))>>(default)

#define MAKE_INTERVAL_4(start, end, default, mode) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start)), decltype(std::get<4>(makeFloatConstantImpl(start)))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end)), decltype(std::get<4>(makeFloatConstantImpl(end)))>, mode>(default)

#define MAKE_INTERVAL( ... ) CALL_OVERLOAD( MAKE_INTERVAL, __VA_ARGS__ )

#define INTERVAL_TYPE_2(start, end)  Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start)), decltype(std::get<4>(makeFloatConstantImpl(start)))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end)), decltype(std::get<4>(makeFloatConstantImpl(end)))>, IntervalWrapModes::Clamp>

#define INTERVAL_TYPE_3(start, end, mode)  Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start)), decltype(std::get<4>(makeFloatConstantImpl(start)))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end)), decltype(std::get<4>(makeFloatConstantImpl(end)))>, mode>

#define INTERVAL_TYPE_4(start, end, mode, default)  Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start)), decltype(std::get<4>(makeFloatConstantImpl(start)))>, \
  	                                                         FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end)), decltype(std::get<4>(makeFloatConstantImpl(end)))>, mode, \
                                                             FloatConstant<std::get<0>(makeFloatConstantImpl(default)), std::get<1>(makeFloatConstantImpl(default)), std::get<2>(makeFloatConstantImpl(default)), std::get<3>(makeFloatConstantImpl(default)), decltype(std::get<4>(makeFloatConstantImpl(default)))>>

#define INTERVAL_TYPE( ... ) CALL_OVERLOAD( INTERVAL_TYPE, __VA_ARGS__ )
