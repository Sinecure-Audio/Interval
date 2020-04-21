#pragma once

#include <type_traits>

#include "FloatConstant.h"
#include "IntervalUtilities.h"

template<typename ValueType>
struct DynamicInterval;

template<typename Bound1, typename Bound2, IntervalWrapModes WrapMode = IntervalWrapModes::Clamp, typename Default = Bound1>
struct Interval
{
    using ValueType = std::common_type_t<decltype(Bound1::value), decltype(Bound2::value)>;

    static constexpr ValueType Start          =  Bound1::value;
    static constexpr ValueType End            =  Bound2::value;
    static constexpr ValueType DefaultValue   = Default::value;

    static constexpr IntervalWrapModes Mode = WrapMode; 

    static constexpr ValueType LowerBound   = std::min(Start, End);
    static constexpr ValueType UpperBound   = std::max(Start, End);

    static constexpr ValueType Length = detail::abs(UpperBound-LowerBound);

    static constexpr bool Empty = Start == End;

    static constexpr bool Reverse = Start > End;

    constexpr Interval() = default;
    constexpr Interval(const ValueType& val) {setValue(val);}

    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr Interval(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& otherInterval) {setValue(rescaleValue(otherInterval));}

    template<IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr auto operator=(const Interval<Bound1, Bound2, OtherMode, OtherDefault>& otherInterval) {
        if constexpr(!Empty)
            value = otherInterval.getValue();
        return *this;
    }

    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr auto operator=(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& otherInterval) {
        if constexpr(!Empty) {
            using OtherInterval = Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>;
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

    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr auto operator+=(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& otherInterval) {
        if constexpr(!Empty && !Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>::Empty)
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


    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr auto operator-=(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& otherInterval) {
        if constexpr(!Empty && !Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>::Empty)
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


    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr auto operator*=(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& otherInterval) {
        if constexpr(!Empty && !Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>::Empty)
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


    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr auto operator/=(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& otherInterval) {
        if constexpr(!Empty && !Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>::Empty)
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

    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr bool hasSameBoundsAs(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& /*otherInterval*/) const noexcept {
        return Start == OtherStart::value && End == OtherEnd::value;
    }

    template<typename T>
    constexpr bool hasSameBoundsAs(const DynamicInterval<T>& otherInterval) const noexcept {
        return Start == otherInterval.getStart() && End == otherInterval.getEnd();
    }

    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr bool isIdenticalTo(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& otherInterval) const noexcept {
        return hasSameBoundsAs(otherInterval) && Mode == OtherMode && otherInterval.getValue() == value;
    }

    template<typename T>
    constexpr bool isIdenticalTo(const DynamicInterval<T>& otherInterval) const noexcept {
        return hasSameBoundsAs(otherInterval) && otherInterval.getValue() == value;
    }

    constexpr const auto& getValue() const noexcept { return value; }

    constexpr auto getNormalizedValue() const noexcept {// TODO: make this behave as expected with integral types-- ditch the float in the conditional
        using ReturnType = std::conditional_t<std::is_floating_point_v<ValueType>, ValueType, float>;
        if constexpr(Empty)
            return ReturnType{0};
        else
            return static_cast<ReturnType>(value-Start)/static_cast<ReturnType>(Length);
    }

private:
    ValueType value{DefaultValue};

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

    template<typename OtherStart, typename OtherEnd, IntervalWrapModes OtherMode, typename OtherDefault>
    constexpr ValueType rescaleValue(const Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>& otherInterval) const noexcept {
        using OtherInterval = Interval<OtherStart, OtherEnd, OtherMode, OtherDefault>;
        if constexpr(!OtherInterval::Empty) {
            constexpr auto offset = OtherInterval::Start-Start; 
            const auto scaledValue = (otherInterval.getValue()*(End-Start))/static_cast<ValueType>(OtherInterval::End-OtherInterval::Start);
            return static_cast<ValueType>(scaledValue+offset);
        }
        else
            return value;
    }
};  

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator==(const Interval<Start, End, Mode, Default>& lhs, const Interval<Start1, End1, Mode1, Default1>& rhs) noexcept {
    return lhs.getNormalizedValue() == rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator!=(const Interval<Start, End, Mode, Default>& lhs, const Interval<Start1, End1, Mode1, Default1>& rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator>(const Interval<Start, End, Mode, Default>& lhs, const Interval<Start1, End1, Mode1, Default1>& rhs) noexcept {
    return lhs.getNormalizedValue() > rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator<(const Interval<Start, End, Mode, Default>& lhs, const Interval<Start1, End1, Mode1, Default1>& rhs) noexcept {
    return lhs.getNormalizedValue() < rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator>=(const Interval<Start, End, Mode, Default>& lhs, const Interval<Start1, End1, Mode1, Default1>& rhs) noexcept {
    return (lhs.getNormalizedValue() >= rhs.getNormalizedValue());
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator<=(const Interval<Start, End, Mode, Default>& lhs, const Interval<Start1, End1, Mode1, Default1>& rhs) noexcept {
    return (lhs.getNormalizedValue() <= rhs.getNormalizedValue());
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator==(const Interval<Start, End, Mode, Default>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return lhs.getNormalizedValue() == rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator!=(const Interval<Start, End, Mode, Default>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator>(const Interval<Start, End, Mode, Default>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return lhs.getNormalizedValue() > rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator<(const Interval<Start, End, Mode, Default>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return lhs.getNormalizedValue() < rhs.getNormalizedValue();
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator>=(const Interval<Start, End, Mode, Default>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return (lhs.getNormalizedValue() >= rhs.getNormalizedValue());
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator<=(const Interval<Start, End, Mode, Default>& lhs, const DynamicInterval<T>& rhs) noexcept {
    return (lhs.getNormalizedValue() <= rhs.getNormalizedValue());
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default>
constexpr auto operator==(const Interval<Start, End, Mode, Default>& lhs, const typename Interval<Start, End, Mode, Default>::ValueType& rhs) noexcept {
    return lhs.getValue() == rhs;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator!=(const Interval<Start, End, Mode, Default>& lhs, const typename Interval<Start, End, Mode, Default>::ValueType& rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator>(const Interval<Start, End, Mode, Default>& lhs, const typename Interval<Start, End, Mode, Default>::ValueType& rhs) noexcept {
    return lhs.getValue() > rhs;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator<(const Interval<Start, End, Mode, Default>& lhs, const typename Interval<Start, End, Mode, Default>::ValueType& rhs) noexcept {
    return lhs.getValue() < rhs;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator>=(const Interval<Start, End, Mode, Default>& lhs, const typename Interval<Start, End, Mode, Default>::ValueType& rhs) noexcept {
    return (lhs.getValue() >= rhs);
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename T>
constexpr auto operator<=(const Interval<Start, End, Mode, Default>& lhs, const typename Interval<Start, End, Mode, Default>::ValueType& rhs) noexcept {
    return (lhs.getValue() <= rhs);
}


template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename RhsType>
constexpr auto operator+(const Interval<Start, End, Mode, Default>& lhsInterval, const RhsType& rhsValue) noexcept {
    const auto interval = MAKE_INTERVAL(lhsInterval.Start, lhsInterval.End, lhsInterval.getValue());
    if constexpr(!interval.Empty)
        interval -= rhsValue;
    return interval;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator+(const Interval<Start, End, Mode, Default>& lhsInterval, const Interval<Start1, End1, Mode1, Default1>& rhsInterval) noexcept {
    using ReturnType = Interval<Start, End, Mode, Default>;
    if constexpr(ReturnType::Empty || Interval<Start1, End1, Mode1, Default1>::Empty)
        return ReturnType{lhsInterval.getValue()};
    else {
        using ValueType = typename ReturnType::ValueType;
        return ReturnType{MAKE_INTERVAL(ValueType{0}, ValueType{1}, lhsInterval.getNormalizedValue()+rhsInterval.getNormalizedValue())};
    }
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename RhsType>
constexpr auto operator-(const Interval<Start, End, Mode, Default>& lhsInterval, const RhsType& rhsValue) noexcept {
    const auto interval = MAKE_INTERVAL(lhsInterval.Start, lhsInterval.End, lhsInterval.getValue());
    if constexpr(!interval.Empty)
        interval -= rhsValue;
    return interval;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator-(const Interval<Start, End, Mode, Default>& lhsInterval, const Interval<Start1, End1, Mode1, Default1>& rhsInterval) noexcept {
    using ReturnType = Interval<Start, End, Mode, Default>;
    if constexpr(ReturnType::Empty || Interval<Start1, End1, Mode1, Default1>::Empty)
        return ReturnType{lhsInterval.getValue()};
    else {
        using ValueType = typename ReturnType::ValueType;
        return ReturnType{MAKE_INTERVAL(ValueType{0}, ValueType{1}, lhsInterval.getNormalizedValue()-rhsInterval.getNormalizedValue())};
    }
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename RhsType>
constexpr auto operator*(const Interval<Start, End, Mode, Default>& lhsInterval, const RhsType& rhsValue) noexcept {
    const auto interval = MAKE_INTERVAL(lhsInterval.Start, lhsInterval.End, lhsInterval.getValue());
    if constexpr(!interval.Empty)
        interval *= rhsValue;
    return interval;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator*(const Interval<Start, End, Mode, Default>& lhsInterval, const Interval<Start1, End1, Mode1, Default1>& rhsInterval) noexcept {
    using ReturnType = Interval<Start, End, Mode, Default>;
    if constexpr(ReturnType::Empty || Interval<Start1, End1, Mode1, Default1>::Empty)
        return ReturnType{lhsInterval.getValue()};
    else {
        using ValueType = typename ReturnType::ValueType;
        return ReturnType{MAKE_INTERVAL(ValueType{0}, ValueType{1}, lhsInterval.getNormalizedValue()*rhsInterval.getNormalizedValue())};
    }
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename RhsType>
constexpr auto operator/(const Interval<Start, End, Mode, Default>& lhsInterval, const RhsType& rhsValue) noexcept {
    const auto interval = MAKE_INTERVAL(lhsInterval.Start, lhsInterval.End, lhsInterval.getValue());
    if constexpr(!interval.Empty)
        interval /= rhsValue;
    return interval;
}

template<typename Start, typename End, IntervalWrapModes Mode, typename Default, typename Start1, typename End1, IntervalWrapModes Mode1, typename Default1>
constexpr auto operator/(const Interval<Start, End, Mode, Default>& lhsInterval, const Interval<Start1, End1, Mode1, Default1>& rhsInterval) noexcept {
    using ReturnType = Interval<Start, End, Mode, Default>;
    if constexpr(ReturnType::Empty || Interval<Start1, End1, Mode1, Default1>::Empty)
        return ReturnType{lhsInterval.getValue()};
    else {
        using ValueType = typename ReturnType::ValueType;
        return ReturnType{MAKE_INTERVAL(ValueType{0}, ValueType{1}, lhsInterval.getNormalizedValue()/rhsInterval.getNormalizedValue())};
    }
}

template<typename... Args>
using StaticInterval = Interval<Args...>;
