#pragma once

#include <type_traits>

#include "IntervalUtilities.h"

template<typename ValueType>
struct DynamicInterval
{
    constexpr DynamicInterval(const ValueType& defaultStart, const ValueType& defaultEnd) : 
                                                start(defaultStart), end(defaultEnd), 
                                                lowerBound(std::min(start, end)), upperBound(std::max(start, end)),
                                                length(end-start), empty(start == end) {
            setValue(start);
        }

	constexpr DynamicInterval(const ValueType& defaultStart, const ValueType& defaultEnd, const ValueType& defaultValue) noexcept : start(defaultStart), end(defaultEnd), lowerBound(std::min(start, end)), upperBound(std::max(start, end)), length(end-start), empty(start == end) {static_assert(std::is_arithmetic_v<ValueType>, "ValueType should be algebraic!");  setValue(defaultValue);}
    constexpr DynamicInterval(const ValueType& defaultStart, const ValueType& defaultEnd, const ValueType& defaultValue, const IntervalWrapModes defaultWrapMode) noexcept : start(defaultStart), end(defaultEnd), lowerBound(std::min(start, end)), upperBound(std::max(start, end)), length(end-start), empty(start == end), wrapMode(defaultWrapMode) {static_assert(std::is_arithmetic_v<ValueType>, "ValueType should be algebraic!");  setValue(defaultValue);}

    template<typename T>
    constexpr DynamicInterval(const DynamicInterval<T>& otherInterval) noexcept {
        setStart(static_cast<ValueType>(otherInterval.getStart()));
        setEnd(static_cast<ValueType>(otherInterval.getEnd()));
        setValue(static_cast<ValueType>(rescaleValue(otherInterval)));
    }

    constexpr auto operator=(const ValueType& newValue) noexcept {
        setValue(newValue);
        return *this;
    }

    template<typename... T>
    constexpr auto operator=(const Interval<T...>& otherInterval) noexcept {
        setValue(rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator=(const DynamicInterval<T>& otherInterval) {
        setValue(rescaleValue(otherInterval));
        return *this;
    }


    constexpr auto operator-() const noexcept {
        return decltype(*this)(value * ValueType{-1.0});
    }

    constexpr auto operator++(int) noexcept {
        const auto out = *this;
        value++;
        return out;
    }

    constexpr auto operator--(int) noexcept {
        const auto out = *this;
        value--;
        return out;
    }

    constexpr auto& operator++() noexcept {
        ++value;
        return *this;
    }

    constexpr auto& operator--() noexcept {
        --value;
        return *this;
    }

    template<typename... T>
    constexpr auto operator+=(const Interval<T...>& otherInterval) noexcept {
        setValue(value+rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator+=(const DynamicInterval<T>& otherInterval) noexcept {
        setValue(value+rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator+=(const T& newValue) noexcept {
        setValue(value+newValue);
        return *this;
    }


    template<typename... T>
    constexpr auto operator-=(const Interval<T...>& otherInterval) noexcept {
        setValue(value-rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator-=(const DynamicInterval<T>& otherInterval) noexcept {
        setValue(value-rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator-=(const T& newValue) noexcept {
        setValue(value-newValue);
        return *this;
    }


    template<typename... T>
    constexpr auto operator*=(const Interval<T...>& otherInterval) noexcept {
        setValue(value*rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator*=(const DynamicInterval<T>& otherInterval) noexcept {
        setValue(value*rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator*=(const T& newValue) noexcept {
        setValue(value*newValue);
        return *this;
    }


    template<typename... T>
    constexpr auto operator/=(const Interval<T...>& otherInterval) noexcept  {
        setValue(value/rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator/=(const DynamicInterval<T>& otherInterval) noexcept {
        setValue(value/rescaleValue(otherInterval));
        return *this;
    }

    template<typename T>
    constexpr auto operator/=(const T& newValue) noexcept {
        setValue(value/newValue);
        return *this;
    }

    template<typename Start, typename End, IntervalWrapModes Mode, typename Default>
    constexpr bool hasSameBoundsAs(const Interval<Start, End, Mode, Default>& /*otherInterval*/) const noexcept { return start == Start::value && end == End::value; }

    template<typename T>
    constexpr bool hasSameBoundsAs(const DynamicInterval<T>& otherInterval) const noexcept { return start == otherInterval.getStart() && end == otherInterval.getEnd();}

    template<typename Start, typename End, IntervalWrapModes Mode, typename Default>
    constexpr bool isIdenticalTo(const Interval<Start, End, Mode, Default>& otherInterval) const noexcept { return hasSameBoundsAs(otherInterval) && wrapMode == Mode && otherInterval.getValue() == value; }

    template<typename T>
    constexpr bool isIdenticalTo(const DynamicInterval<T>& otherInterval) const noexcept { return hasSameBoundsAs(otherInterval) && wrapMode == otherInterval.getWrapMode() && otherInterval.getValue() == value; }

    template<typename... T>
    constexpr void setValue(const Interval<T...>& otherInterval) noexcept {
        if (!empty && !otherInterval.isEmpty())
            value = rescaleValue(otherInterval);    
    }

    template<typename T>
    constexpr void setValue(const DynamicInterval<T>& otherInterval) noexcept {
        if (!empty && !otherInterval.isEmpty())
            value = rescaleValue(otherInterval); 
    }

    constexpr void setValue(const ValueType& newValue) noexcept {
        if (empty)
            return;

        if (newValue <= upperBound && newValue >= lowerBound)
            value = newValue;
        else 
            if (wrapMode == IntervalWrapModes::Clamp)
                value = clampValue(newValue, lowerBound, upperBound);
            else if (wrapMode == IntervalWrapModes::Wrap)
                value = wrapValue(newValue, lowerBound, upperBound);
	}

    constexpr void setStart(const ValueType& newStart) noexcept {
        start = newStart;
        upperBound = std::max(start, end);
        lowerBound = std::min(start, end);
        empty = start == end;
        length = end-start;
        setValue(value);
    }

    constexpr auto getStart() const noexcept { return start; }

    constexpr void setEnd(const ValueType& newEnd) noexcept {
        end = newEnd;
        upperBound = std::max(start, end);
        lowerBound = std::min(start, end);
        empty = start == end;
        length = end-start;
        setValue(value);
    }

    constexpr auto getEnd() const noexcept { return end; }

    constexpr auto isEmpty() const noexcept { return empty; }

    void setWrapMode(const IntervalWrapModes& newWrapMode) noexcept { wrapMode = newWrapMode; }

    constexpr auto getWrapMode() const noexcept { return wrapMode; }

    constexpr auto getLength() const noexcept { return length; }

    constexpr auto getValue() const noexcept { return value; }

    constexpr auto getNormalizedValue() const noexcept {
        using ReturnType = std::conditional_t<std::is_floating_point_v<ValueType>, ValueType, double>;
        if(empty)
            return ReturnType(0);
        else
            return static_cast<ReturnType>(value-start)/static_cast<ReturnType>(length);
    }

private:
    static constexpr auto rescaleValueImpl = [](const auto& otherInterval, const ValueType& start, const ValueType& length){const auto offset = otherInterval.getStart() - start; return static_cast<ValueType>((otherInterval.getValue()*length)/otherInterval.getLength()+offset);};
    
    /// Rescales the value of another interval 
    template<typename T>
    constexpr auto rescaleValue(const DynamicInterval<T>& otherInterval) const noexcept {
        if(!otherInterval.isEmpty())
            return rescaleValueImpl(otherInterval, start, length);
        else
            return start;
    }

    template<typename... T>
    constexpr auto rescaleValue(const Interval<T...>& otherInterval) const noexcept {
        if constexpr(!otherInterval.Empty)
            return rescaleValueImpl(otherInterval, start, length);
        else
            return start;
    }

    ValueType start{}, end{}, lowerBound{}, upperBound{}, length{}, value{};
	bool empty{true};
	IntervalWrapModes wrapMode{IntervalWrapModes::Clamp};
};



    template<typename T, typename U>
    constexpr auto operator==(const DynamicInterval<T>& lhs, const DynamicInterval<U>& rhs) noexcept { return lhs.getNormalizedValue() == rhs.getNormalizedValue(); }

    template<typename T, typename U>
    constexpr auto operator!=(const DynamicInterval<T>& lhs, const DynamicInterval<U>& rhs) noexcept { return !(lhs == rhs); }

    template<typename T, typename U>
    constexpr auto operator>(const DynamicInterval<T>& lhs, const DynamicInterval<U>& rhs) noexcept  { return lhs.getNormalizedValue()   > rhs.getNormalizedValue(); }

    template<typename T, typename U>
    constexpr auto operator<(const DynamicInterval<T>& lhs, const DynamicInterval<U>& rhs) noexcept  { return lhs.getNormalizedValue()   < rhs.getNormalizedValue(); }

    template<typename T, typename U>
    constexpr auto operator>=(const DynamicInterval<T>& lhs, const DynamicInterval<U>& rhs) noexcept { return (lhs.getNormalizedValue() >= rhs.getNormalizedValue()); }

    template<typename T, typename U>
    constexpr auto operator<=(const DynamicInterval<T>& lhs, const DynamicInterval<U>& rhs) noexcept { return (lhs.getNormalizedValue() <= rhs.getNormalizedValue()); }



    // All arithmetic operators return a range with the same bounds as the left operand
    // The value of this is calculated by performing the operand with a value,
    // or, if the other value is an Interval, by normalizing the values of both intervals and performing the operation, and unnormalizing the result.
    template<typename T, typename U>
    constexpr auto operator+(const DynamicInterval<T>& lhs, const U& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()+rhs)> interval = lhs;
        interval.setValue(lhs.getValue()+rhs);
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator+(const DynamicInterval<T>& lhs, const  DynamicInterval<U>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()+lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()+lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename... U>
    constexpr auto operator+(const DynamicInterval<T>& lhs, const  Interval<U...>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()+lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()+lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator-(const DynamicInterval<T>& lhs, const U& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()-rhs)> interval = lhs;
        interval.setValue(lhs.getValue()-rhs);
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator-(const DynamicInterval<T>& lhs, const  DynamicInterval<U>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()-lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()-lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename... U>
    constexpr auto operator-(const DynamicInterval<T>& lhs, const  Interval<U...>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()-lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()-lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator*(const DynamicInterval<T>& lhs, const U& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()*rhs)> interval = lhs;
        interval.setValue(lhs.getValue()*rhs);
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator*(const DynamicInterval<T>& lhs, const  DynamicInterval<U>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()*lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()*lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename... U>
    constexpr auto operator*(const DynamicInterval<T>& lhs, const  Interval<U...>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()*lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()*lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator/(const DynamicInterval<T>& lhs, const U& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()/rhs)> interval = lhs;
        interval.setValue(lhs.getValue()/rhs);
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator/(const DynamicInterval<T>& lhs, const  DynamicInterval<U>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()/lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()/lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename... U>
    constexpr auto operator/(const DynamicInterval<T>& lhs, const  Interval<U...>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()/lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()/lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator%(const DynamicInterval<T>& lhs, const U& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()%rhs)> interval = lhs;
        interval.setValue(lhs.getValue()%rhs);
        return interval;
    }

    template<typename T, typename U>
    constexpr auto operator%(const DynamicInterval<T>& lhs, const  DynamicInterval<U>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()%lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()%lhs.rescaleValue(rhs));
        return interval;
    }

    template<typename T, typename... U>
    constexpr auto operator%(const DynamicInterval<T>& lhs, const  Interval<U...>& rhs) noexcept {
        DynamicInterval<decltype(lhs.getValue()%lhs.rescaleValue(rhs))> interval = lhs;
        interval.setValue(lhs.getValue()%lhs.rescaleValue(rhs));
        return interval;
    }
