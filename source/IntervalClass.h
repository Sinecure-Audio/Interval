#pragma once

#include "FloatConstant.h"

#define MAKE_INTERVAL(x, y) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(x)), std::get<1>(makeFloatConstantImpl(x)), std::get<2>(makeFloatConstantImpl(x)), std::get<3>(makeFloatConstantImpl(x))>, \
  FloatConstant<std::get<0>(makeFloatConstantImpl(y)), std::get<1>(makeFloatConstantImpl(y)), std::get<2>(makeFloatConstantImpl(y)), std::get<3>(makeFloatConstantImpl(y))>>{};



template<typename Bound1, typename Bound2, typename ConstantValue = Bound1>
struct Interval
{
    static_assert(is_specialization<Bound1,   FloatConstant>{}, "Start value needs to be a float constant.");
    static_assert(is_specialization<Bound2,   FloatConstant>{}, "End value needs to be a float constant.");

    using ValueType = std::common_type_t<decltype(Bound1::value), decltype(Bound2::value)>;

    static constexpr ValueType Start = Bound1::value;
    static constexpr ValueType End   = Bound2::value;

    static constexpr ValueType LowerBound   = std::min(Start, End);
    static constexpr ValueType UpperBound   = std::max(Start, End);

    static constexpr bool Empty = Start==End;

    constexpr Interval(const ValueType& val = Start) : value(val){};
    constexpr Interval(const Interval&) = default;
	constexpr Interval(Interval&&) = default;

    // template<typename Ty, typename Tu>
    // constexpr auto operator+=(const Interval<Ty, Tu>& otherRange)
    // {
    //     const auto offset = otherRange.Start-Start;
    //     const auto scale = (otherRange.End-otherRange.Start)/(End-Start);
    //     setValue(otherRange.value+offset*scale);
    //     return *this;
    // }

    template<typename Ty, typename Tu>
    constexpr auto getOffsetAndScale(const Interval<Ty, Tu>& otherRange)
    {
        const auto offset = otherRange.Start-Start;
        const auto scale = (End-Start)/(otherRange.End-otherRange.Start);
        return std::pair{offset, scale};
    }

    template<typename Ty, typename Tu>
    constexpr auto operator+=(const Interval<Ty, Tu>& otherRange)
    {
        const auto offset = otherRange.Start-Start;
        const auto scale = (End-Start)/(otherRange.End-otherRange.Start);
        setValue(value+((otherRange.value+offset)*scale));
        return *this;
    }

    template<typename Ty, typename Tu>
    constexpr auto operator-=(const Interval<Ty, Tu>& otherRange)
    {
        const auto offset = otherRange.Start-Start;
        const auto scale = (End-Start)/(otherRange.End-otherRange.Start);
        setValue(value-((otherRange.value+offset)*scale));
        return *this;
    }

    constexpr auto operator+(const ValueType& newValue)
    {
        if constexpr(!Empty)
            setValue(newValue+value);
        return *this;
    }

    constexpr auto operator-(const ValueType& newValue)
    {
        if constexpr(!Empty)
            setValue(value-newValue);
        return *this;
    }

    constexpr auto operator*(const ValueType& newValue)
    {
        if constexpr(!Empty)
            setValue(newValue*value);
        return *this;
    }

    constexpr auto operator/(const ValueType& newValue)
    {
        if constexpr(!Empty)
            setValue(value/newValue);
        return *this;
    }

    constexpr void setValue(const ValueType& newValue, const bool& scaleValue = false)
	{
        if constexpr(!Empty)
        {
		    auto valueToUse = newValue;
            if (valueToUse > UpperBound)
                value = UpperBound;
            else if (valueToUse < LowerBound)
                value = LowerBound;
            else
                value = valueToUse;
        }
	};

    ValueType value;
};