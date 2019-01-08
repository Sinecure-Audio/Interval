#pragma once

#include "FloatConstant.h"

#define SICRCAT( A, B ) A ## B
#define SICRSELECT( NAME, NUM ) SICRCAT( NAME ## _, NUM )

#define SICRGET_COUNT( _1, _2, _3, _4, COUNT, ... ) COUNT
#define SICRVA_SIZE( ... ) SICRGET_COUNT( __VA_ARGS__, 4, 3, 2, 1 )

#define SICRVA_SELECT( NAME, ... ) SICRSELECT( NAME, SICRVA_SIZE(__VA_ARGS__) )(__VA_ARGS__)

#define MAKE_INTERVAL( ... ) SICRVA_SELECT( MAKE_INTERVAL, __VA_ARGS__ )

#define MAKE_INTERVAL_2(start, end) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end))>>{}

#define MAKE_INTERVAL_3(start, end, default) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end))>>(default)

#define MAKE_INTERVAL_4(start, end, default, mode) \
Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end))>, mode>(default)


namespace {
    //used to make sure that the bounds for the Interval are FloatConstants
    template <class T, template <auto...> class Template>
    struct is_specialization : std::false_type {};

    template <template <auto...> class Template, auto... Args>
    struct is_specialization<Template<Args...>, Template> : std::true_type {};
}


enum class IntervalWrapModes
{
    Clamp,
    Wrap
};

template<typename Bound1, typename Bound2, IntervalWrapModes WrapMode = IntervalWrapModes::Clamp>
struct Interval
{
    static_assert(is_specialization<Bound1, FloatConstant>{}, "Start value needs to be a float constant.");
    static_assert(is_specialization<Bound2, FloatConstant>{}, "End value needs to be a float constant.");

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

    constexpr auto operator=(const Interval<Bound1, Bound2>& otherInterval)
    {
        return value = otherInterval.value;
    }

    template<typename Ty, typename Tu>
    constexpr auto operator=(const Interval<Ty, Tu>& otherInterval)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherInterval);
        return value = ((otherInterval.value+offsetAndScale.first)*offsetAndScale.second);
    }

    constexpr auto operator=(const ValueType& newValue)
    {
        return value = newValue;
    }

    //template<typename Ty, typename Tu>
    constexpr auto operator==(const Interval<Bound1, Bound2>& otherInterval) const
    {
        return otherInterval.value == value;
    }

    template<typename Ty, typename Tu>
    constexpr auto operator==(const Interval<Ty, Tu>&) const
    {
        return false;
    }

    constexpr auto operator!=(const Interval<Bound1, Bound2>& otherInterval) const
    {
        return !(otherInterval.value == value);
    }

    template<typename Ty, typename Tu>
    constexpr auto operator!=(const Interval<Ty, Tu>&) const
    {
        return true;
    }

    constexpr auto operator>(const Interval<Bound1, Bound2>& otherInterval) const
    {
	if (value > otherInterval.value)
        	return true;
	else
		return false;
    }

    constexpr auto operator<(const Interval<Bound1, Bound2>& otherInterval) const
    {
	if (value < otherInterval.value)
        	return true;
	else
		return false;
    }

    template<typename Ty, typename Tu>
    static constexpr auto getOffsetAndScale(const Interval<Ty, Tu>& otherInterval)
    {
        constexpr auto offset = otherInterval.Start-Start;
        constexpr auto scale = (End-Start)/(otherInterval.End-otherInterval.Start);
        return std::pair{offset, scale};
    }

    template<typename Ty, typename Tu>
    constexpr auto operator+=(const Interval<Ty, Tu>& otherInterval)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherInterval);
        setValue(value+((otherInterval.value+offsetAndScale.first)*offsetAndScale.second));
        return *this;
    }

    constexpr auto operator+=(const ValueType& newValue)
    {
        setValue(value+newValue);
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator-=(const Interval<Ty, Tu>& otherInterval)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherInterval);
        setValue(value-((otherInterval.value+offsetAndScale.first)*offsetAndScale.second));
        return *this;
    }

    constexpr auto operator-=(const ValueType& newValue)
    {
        setValue(value-newValue);
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator*=(const Interval<Ty, Tu>& otherInterval)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherInterval);
        setValue(value*((otherInterval.value+offsetAndScale.first)*offsetAndScale.second));
        return *this;
    }

    constexpr auto operator*=(const ValueType& newValue)
    {
        setValue(value*newValue);
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator/=(const Interval<Ty, Tu>& otherInterval)
    {
        constexpr auto offsetAndScale = getOffsetAndScale(otherInterval);
        setValue(value/((otherInterval.value+offsetAndScale.first)*offsetAndScale.second));
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
            if constexpr (WrapMode == IntervalWrapModes::Clamp)
            {
                auto valueToUse = newValue;
                if (valueToUse > UpperBound)
                    value = UpperBound;
                else if (valueToUse < LowerBound)
                    value = LowerBound;
                else
                    value = valueToUse;
            }
            else if constexpr(WrapMode == IntervalWrapModes::Wrap)
            {
                constexpr auto offset = Start;
                constexpr auto range = Length;

                auto valueToUse = newValue-offset;
                if (valueToUse > UpperBound || valueToUse < LowerBound)
                    if constexpr(std::is_floating_point_v<ValueType>)
                        value = std::fmod(newValue, range)+offset;
                    else
                        value = (newValue % range)+offset;
                else
                    value = valueToUse;
            }
        }
	}

    ValueType value;
};
