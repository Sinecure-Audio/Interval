#pragma once

#include "FloatConstant.h"

#define SICRCAT( A, B ) A ## B
#define SICRSELECT( NAME, NUM ) SICRCAT( NAME ## _, NUM )

#define SICRGET_COUNT( _1, _2, _3, COUNT, ... ) COUNT
#define SICRVA_SIZE( ... ) SICRGET_COUNT( __VA_ARGS__, 3, 2, 1 )

#define SICRVA_SELECT( NAME, ... ) SICRSELECT( NAME, SICRVA_SIZE(__VA_ARGS__) )(__VA_ARGS__)

#define MAKE_INTERVAL( ... ) SICRVA_SELECT( MAKE_INTERVAL, __VA_ARGS__ )

#define MAKE_INTERVAL_2(start, end) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end))>>{}

#define MAKE_INTERVAL_3(start, end, default) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end))>>(default)



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

    static constexpr ValueType Length = UpperBound-LowerBound;

    static constexpr bool Empty = Start==End;

    static constexpr bool Reverse = Start>End;

    constexpr Interval(const ValueType& val = Start) : value(val){}
    constexpr Interval(const Interval&) = default;
	constexpr Interval(Interval&&) = default;

    constexpr auto operator=(const Interval<Bound1, Bound2>& otherRange)
    {
        return value = otherRange.value;
    }

    template<typename Ty, typename Tu>
    constexpr auto operator=(const Interval<Ty, Tu>& otherRange)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherRange);
        return value = ((otherRange.value+offsetAndScale.first)*offsetAndScale.second);
    }

    constexpr auto operator=(const ValueType& newValue)
    {
        return value = newValue;
    }

    //template<typename Ty, typename Tu>
    constexpr auto operator==(const Interval<Bound1, Bound2>& otherRange) const
    {
        return otherRange.value == value;
    }

    template<typename Ty, typename Tu>
    constexpr auto operator==(const Interval<Ty, Tu>&) const
    {
        return false;
    }

    constexpr auto operator!=(const Interval<Bound1, Bound2>& otherRange) const
    {
        return !(otherRange.value == value);
    }

    template<typename Ty, typename Tu>
    constexpr auto operator!=(const Interval<Ty, Tu>&) const
    {
        return true;
    }

    constexpr auto operator>(const Interval<Bound1, Bound2>& otherRange) const
    {
	if (value > otherRange.value)
        	return true;
	else
		return false;
    }

    constexpr auto operator<(const Interval<Bound1, Bound2>& otherRange) const
    {
	if (value < otherRange.value)
        	return true;
	else
		return false;
    }

    // template<typename Ty, typename Tu>
    // constexpr auto operator+=(const Interval<Ty, Tu>& otherRange)
    // {
    //     const auto offset = otherRange.Start-Start;
    //     const auto scale = (otherRange.End-otherRange.Start)/(End-Start);
    //     setValue(otherRange.value+offset*scale);
    //     return *this;
    // }

    // template<typename T>
    // constexpr auto operator+(const Interval<Bound1, T>& otherInteveral)
    // {
    //     // if both intervals aren't going in the same direction, the range should be between the smallest of start, end1, and end2. Otherwise, it should go from start to end1+end2  
    //     if constexpr 
    //     return Interval<Bound1, T::value+>
    // }

    template<typename Ty, typename Tu>
    static constexpr auto getOffsetAndScale(const Interval<Ty, Tu>& otherRange)
    {
        constexpr auto offset = otherRange.Start-Start;
        constexpr auto scale = (End-Start)/(otherRange.End-otherRange.Start);
        return std::pair{offset, scale};
    }

    template<typename Ty, typename Tu>
    constexpr auto operator+=(const Interval<Ty, Tu>& otherRange)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherRange);
        setValue(value+((otherRange.value+offsetAndScale.first)*offsetAndScale.second));
        return *this;
    }

    constexpr auto operator+=(const ValueType& newValue)
    {
        setValue(value+newValue);
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator-=(const Interval<Ty, Tu>& otherRange)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherRange);
        setValue(value-((otherRange.value+offsetAndScale.first)*offsetAndScale.second));
        return *this;
    }

    constexpr auto operator-=(const ValueType& newValue)
    {
        setValue(value-newValue);
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator*=(const Interval<Ty, Tu>& otherRange)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherRange);
        setValue(value*((otherRange.value+offsetAndScale.first)*offsetAndScale.second));
        return *this;
    }

    constexpr auto operator*=(const ValueType& newValue)
    {
        setValue(value*newValue);
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator/=(const Interval<Ty, Tu>& otherRange)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherRange);
        setValue(value/((otherRange.value+offsetAndScale.first)*offsetAndScale.second));
        return *this;
    }

    constexpr auto operator/=(const ValueType& newValue)
    {
        setValue(value/newValue);
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

    constexpr void setValue(const ValueType& newValue)
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
	}

    ValueType value;
};
