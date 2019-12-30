#pragma once

#include "FloatConstant.h"

#include "IntervalUtilities.h"

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

#define INTERVAL_TYPE(start, end, mode)  Interval<FloatConstant<std::get<0>(makeFloatConstantImpl(start)), std::get<1>(makeFloatConstantImpl(start)), std::get<2>(makeFloatConstantImpl(start)), std::get<3>(makeFloatConstantImpl(start)), decltype(std::get<4>(makeFloatConstantImpl(start)))>, \
  	 FloatConstant<std::get<0>(makeFloatConstantImpl(end)), std::get<1>(makeFloatConstantImpl(end)), std::get<2>(makeFloatConstantImpl(end)), std::get<3>(makeFloatConstantImpl(end)), decltype(std::get<4>(makeFloatConstantImpl(end)))>, mode>

template<typename ValueType>
struct DynamicInterval;

template<typename Bound1, typename Bound2, IntervalWrapModes WrapMode = IntervalWrapModes::Clamp>
struct Interval
{
    //static_assert(is_specialization<Bound1, FloatConstant>{}, "Start value needs to be a float constant.");
    //static_assert(is_specialization<Bound2, FloatConstant>{}, "End value needs to be a float constant.");

    using ValueType = std::common_type_t<decltype(Bound1::value), decltype(Bound2::value)>;

    static constexpr ValueType Start = Bound1::value;
    static constexpr ValueType End   = Bound2::value;

    static constexpr IntervalWrapModes Mode = WrapMode; 

    static constexpr ValueType LowerBound   = std::min(Start, End);
    static constexpr ValueType UpperBound   = std::max(Start, End);

    static constexpr ValueType Length = detail::abs(UpperBound-LowerBound);

    static constexpr bool Empty = Start == End;

    static constexpr bool Reverse = Start > End;

    constexpr Interval() = default;
    constexpr Interval(const ValueType& val) {setValue(val);}

    template <typename Ty, typename Tu, IntervalWrapModes M>
    constexpr Interval(const Interval<Ty, Tu, M>& otherInterval) {setValue(rescaleValue(otherInterval));}

    template<IntervalWrapModes M>
    constexpr auto operator=(const Interval<Bound1, Bound2, M>& otherInterval) {
        if constexpr(!Empty)
            value = otherInterval.getValue();
        return *this;
    }

    template<typename T, typename U, IntervalWrapModes M>
    constexpr auto operator=(const Interval<T, U, M>& otherInterval) {
        if constexpr(!Empty) {
            using OtherInterval = Interval<T, U, M>;
            constexpr auto offset = OtherInterval::Start-Start; 
            const auto scaledValue = (otherInterval.getValue()*(End-Start))/(OtherInterval::End-OtherInterval::Start);
            setValue(static_cast<ValueType>(scaledValue+offset));
        }
        return *this;
    }

    constexpr auto operator=(const ValueType& newValue) {
        if constexpr(!Empty)
            value = newValue;
        return *this;
    }

    template<typename Ty, typename Tu, IntervalWrapModes M>
    constexpr auto operator+=(const Interval<Ty, Tu, M>& otherInterval) {
        if constexpr(!Empty && !Interval<Ty, Tu, M>::Empty)
            setValue(value+static_cast<ValueType>(rescaleValue(otherInterval)));
        return *this;
    }

    constexpr auto operator+=(const ValueType& newValue) {
        if constexpr(!Empty)
            setValue(value+newValue);
        return *this;
    }

    template<typename T>
    constexpr auto operator+=(const DynamicInterval<T>& otherInterval) {
        if constexpr(!Empty)
            if(!otherInterval.empty())
                setValue(value+static_cast<ValueType>(rescaleValue(otherInterval)));
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator-=(const Interval<Ty, Tu>& otherInterval) {
        if constexpr(!Empty && !Interval<Ty, Tu>::Empty)
            setValue(value-static_cast<ValueType>(rescaleValue(otherInterval)));
        return *this;
    }

    constexpr auto operator-=(const ValueType& newValue) {
        if constexpr(!Empty)
            setValue(value-newValue);
        return *this;
    }

    template<typename T>
    constexpr auto operator-=(const DynamicInterval<T>& otherInterval) {
        if constexpr(!Empty)
            if(!otherInterval.empty())
                setValue(value-static_cast<ValueType>(rescaleValue(otherInterval)));
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator*=(const Interval<Ty, Tu>& otherInterval) {
        if constexpr(!Empty && !Interval<Ty, Tu>::Empty)
            setValue(value*static_cast<ValueType>(rescaleValue(otherInterval)));
        return *this;
    }

    template<typename T>
    constexpr auto operator*=(const DynamicInterval<T>& otherInterval) {
        if constexpr(!Empty)
            if(!otherInterval.empty())
                setValue(value*static_cast<ValueType>(rescaleValue(otherInterval)));
        return *this;
    }

    constexpr auto operator*=(const ValueType& newValue) {
        if constexpr(!Empty)
            setValue(value*newValue);
        return *this;
    }


    template<typename Ty, typename Tu>
    constexpr auto operator/=(const Interval<Ty, Tu>& otherInterval) {
        if constexpr(!Empty && !Interval<Ty, Tu>::Empty)
            setValue(value/static_cast<ValueType>(rescaleValue(otherInterval)));
        return *this;
    }

    template<typename T>
    constexpr auto operator/=(const DynamicInterval<T>& otherInterval) {
        if constexpr(!Empty)
            if(!otherInterval.empty())
                setValue(value/static_cast<ValueType>(rescaleValue(otherInterval)));
        return *this;
    }

    constexpr auto operator/=(const ValueType& newValue) {
        if constexpr(!Empty)
            setValue(value/newValue);
        return *this;
    }

    template<typename Start1, typename End1, IntervalWrapModes Mode1>
    constexpr bool hasSameBoundsAs(const Interval<Start1, End1, Mode1>& /*otherInterval*/) const noexcept {
        return Start == Start1::value && End == End1::value;
    }

    template<typename T>
    constexpr bool hasSameBoundsAs(const DynamicInterval<T>& otherInterval) const noexcept {
        return Start == otherInterval.getStart() && End == otherInterval.getEnd();
    }

    template<typename Start1, typename End1, IntervalWrapModes Mode1>
    constexpr bool isIdenticalTo(const Interval<Start1, End1, Mode1>& otherInterval) const noexcept {
        return hasSameBoundsAs(otherInterval) && Mode == Mode1 && otherInterval.getValue() == value;
    }

    template<typename T>
    constexpr bool isIdenticalTo(const DynamicInterval<T>& otherInterval) const noexcept {
        return hasSameBoundsAs(otherInterval) && otherInterval.getValue() == value;
    }

    constexpr const auto& getValue() const noexcept { return value; }

    constexpr auto getNormalizedValue() const noexcept {
        using ReturnType = std::conditional_t<std::is_floating_point_v<ValueType>, ValueType, float>;
        if constexpr(Empty)
            return ReturnType{0};
        else
            return static_cast<ReturnType>(value-Start)/static_cast<ReturnType>(Length);
    }

private:
    ValueType value{Start};

    constexpr void setValue(const ValueType& newValue) noexcept {
        if constexpr(!Empty) {
            if (newValue <= UpperBound && newValue >= LowerBound)
                value = newValue;
            else 
                if constexpr (WrapMode == IntervalWrapModes::Clamp)
                    value = clampValue(newValue, LowerBound, UpperBound);
                else if constexpr(WrapMode == IntervalWrapModes::Wrap)
                    value = wrapValue(newValue, LowerBound, UpperBound);
        }
		else
            (void)newValue;// removes a warning about unused variable
	}
    
    template<typename T>
    constexpr ValueType rescaleValue(const DynamicInterval<T>& otherInterval) const noexcept {
        if(!otherInterval.isEmpty()) {
            using OtherInterval = DynamicInterval<T>;
            constexpr auto offset = OtherInterval::Start-Start; 
            const auto scaledValue = (otherInterval.getValue()*(End-Start))/(OtherInterval::End-OtherInterval::Start);
            return static_cast<ValueType>(scaledValue+offset);
        }
        else
            return Start;
    }

    template<typename T, typename U, IntervalWrapModes M>
    constexpr ValueType rescaleValue(const Interval<T, U, M>& otherInterval) const noexcept {
        if constexpr(!Interval<T, U, M>::Empty) {
            using OtherInterval = Interval<T, U, M>;
            constexpr auto offset = OtherInterval::Start-Start; 
            const auto scaledValue = (otherInterval.getValue()*(End-Start))/(OtherInterval::End-OtherInterval::Start);
            return static_cast<ValueType>(scaledValue+offset);
        }
        else
            return value;
    }
};  

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator==(const Interval<Start, End, Mode>& lhs, const Interval<Start1, End1, Mode1>& rhs) noexcept {
    return lhs.getNormalizedValue() == rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator!=(const Interval<Start, End, Mode>& lhs, const Interval<Start1, End1, Mode1>& rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator>(const Interval<Start, End, Mode>& lhs, const Interval<Start1, End1, Mode1>& rhs) noexcept {
    return lhs.getNormalizedValue() > rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator<(const Interval<Start, End, Mode>& lhs, const Interval<Start1, End1, Mode1>& rhs) noexcept {
    return lhs.getNormalizedValue() < rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator>=(const Interval<Start, End, Mode>& lhs, const Interval<Start1, End1, Mode1>& rhs) noexcept {
    return (lhs.getNormalizedValue() >= rhs.getNormalizedValue());
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator<=(const Interval<Start, End, Mode>& lhs, const Interval<Start1, End1, Mode1>& rhs) noexcept {
    return (lhs.getNormalizedValue() <= rhs.getNormalizedValue());
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator==(const Interval<Start, End, Mode>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return lhs.getNormalizedValue() == rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator!=(const Interval<Start, End, Mode>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator>(const Interval<Start, End, Mode>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return lhs.getNormalizedValue() > rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator<(const Interval<Start, End, Mode>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return lhs.getNormalizedValue() < rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator>=(const Interval<Start, End, Mode>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return (lhs.getNormalizedValue() >= rhs.getNormalizedValue());
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator<=(const Interval<Start, End, Mode>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return (lhs.getNormalizedValue() <= rhs.getNormalizedValue());
}

template<typename Start, typename End, IntervalWrapModes Mode>
constexpr auto operator==(const Interval<Start, End, Mode>& lhs, const typename Interval<Start, End, Mode>::ValueType& rhs) noexcept {
    return lhs.getValue() == rhs;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator!=(const Interval<Start, End, Mode>& lhs, const typename Interval<Start, End, Mode>::ValueType& rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator>(const Interval<Start, End, Mode>& lhs, const typename Interval<Start, End, Mode>::ValueType& rhs) noexcept {
    return lhs.getValue() > rhs;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator<(const Interval<Start, End, Mode>& lhs, const typename Interval<Start, End, Mode>::ValueType& rhs) noexcept {
    return lhs.getValue() < rhs;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator>=(const Interval<Start, End, Mode>& lhs, const typename Interval<Start, End, Mode>::ValueType& rhs) noexcept {
    return (lhs.getValue() >= rhs);
}

template<typename Start, typename End, IntervalWrapModes Mode, typename T>
constexpr auto operator<=(const Interval<Start, End, Mode>& lhs, const typename Interval<Start, End, Mode>::ValueType& rhs) noexcept {
    return (lhs.getValue() <= rhs);
}


template<typename Start, typename End, IntervalWrapModes Mode, typename RhsType>
constexpr auto operator+(const Interval<Start, End, Mode>& lhsInterval, const RhsType& rhsValue) noexcept {
    const auto interval = MAKE_INTERVAL(lhsInterval.Start, lhsInterval.End, lhsInterval.getValue());
    if constexpr(!interval.Empty)
        interval -= rhsValue;
    return interval;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator+(const Interval<Start, End, Mode>& lhsInterval, const Interval<Start1, End1, Mode1>& rhsInterval) noexcept {
    if constexpr(Interval<Start, End, Mode>::Empty || Interval<Start1, End1, Mode1>::Empty)
        return Interval<Start, End, Mode>{lhsInterval.getValue()};
    else {
        using ValueType = typename Interval<Start, End, Mode>::ValueType;
        return Interval<Start, End, Mode>{MAKE_INTERVAL(ValueType{0}, ValueType{1}, lhsInterval.getNormalizedValue()+rhsInterval.getNormalizedValue())};
    }
}

template<typename Start, typename End, IntervalWrapModes Mode, typename RhsType>
constexpr auto operator-(const Interval<Start, End, Mode>& lhsInterval, const RhsType& rhsValue) noexcept {
    const auto interval = MAKE_INTERVAL(lhsInterval.Start, lhsInterval.End, lhsInterval.getValue());
    if constexpr(!interval.Empty)
        interval -= rhsValue;
    return interval;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator-(const Interval<Start, End, Mode>& lhsInterval, const Interval<Start1, End1, Mode1>& rhsInterval) noexcept {
    if constexpr(Interval<Start, End, Mode>::Empty || Interval<Start1, End1, Mode1>::Empty)
        return Interval<Start, End, Mode>{lhsInterval.getValue()};
    else {
        using ValueType = typename Interval<Start, End, Mode>::ValueType;
        return Interval<Start, End, Mode>{MAKE_INTERVAL(ValueType{0}, ValueType{1}, lhsInterval.getNormalizedValue()-rhsInterval.getNormalizedValue())};
    }
}

template<typename Start, typename End, IntervalWrapModes Mode, typename RhsType>
constexpr auto operator*(const Interval<Start, End, Mode>& lhsInterval, const RhsType& rhsValue) noexcept {
    const auto interval = MAKE_INTERVAL(lhsInterval.Start, lhsInterval.End, lhsInterval.getValue());
    if constexpr(!interval.Empty)
        interval *= rhsValue;
    return interval;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator*(const Interval<Start, End, Mode>& lhsInterval, const Interval<Start1, End1, Mode1>& rhsInterval) noexcept {
    if constexpr(Interval<Start, End, Mode>::Empty || Interval<Start1, End1, Mode1>::Empty)
        return Interval<Start, End, Mode>{lhsInterval.getValue()};
    else {
        using ValueType = typename Interval<Start, End, Mode>::ValueType;        
        return Interval<Start, End, Mode>{MAKE_INTERVAL(ValueType{0}, ValueType{1}, lhsInterval.getNormalizedValue()*rhsInterval.getNormalizedValue())};
    }
}

template<typename Start, typename End, IntervalWrapModes Mode, typename RhsType>
constexpr auto operator/(const Interval<Start, End, Mode>& lhsInterval, const RhsType& rhsValue) noexcept {
    const auto interval = MAKE_INTERVAL(lhsInterval.Start, lhsInterval.End, lhsInterval.getValue());
    if constexpr(!interval.Empty)
        interval /= rhsValue;
    return interval;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Start1, typename End1, IntervalWrapModes Mode1>
constexpr auto operator/(const Interval<Start, End, Mode>& lhsInterval, const Interval<Start1, End1, Mode1>& rhsInterval) noexcept {
    if constexpr(Interval<Start, End, Mode>::Empty || Interval<Start1, End1, Mode1>::Empty)
        return Interval<Start, End, Mode>{lhsInterval.getValue()};
    else {
        using ValueType = typename Interval<Start, End, Mode>::ValueType;
        return Interval<Start, End, Mode>{MAKE_INTERVAL(ValueType{0}, ValueType{1}, lhsInterval.getNormalizedValue()/rhsInterval.getNormalizedValue())};
    }
}
