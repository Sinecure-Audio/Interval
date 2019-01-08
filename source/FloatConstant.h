#pragma once

#include <tuple>
#include <cfloat>
#include <climits>
#include <cmath>

namespace {
    //A simple integral constexpr pow, used to get a value to convert the decimal part of the float to or from an integral type.
    template <auto Base, auto Power>
    constexpr auto integralPowImpl(const decltype(Base)& val)
    {
        if constexpr (Power > 0)
            return integralPowImpl<Base, Power-1>(val*Base);
        else
            return val;
    }

    template <auto Base, auto Power>
    constexpr auto integralPow()
    {
        if constexpr (Power == 0)
            return 1;
        else if (Power == 1)
            return Base;
        else if constexpr (Power > 1)
            return integralPowImpl<Base, Power-1>(Base);
    }

    template<typename T>
    constexpr auto getSignificantDigitsForType()
    {
        //Doesn't use numeric_limits because for some reason those function don't return the same as these macros, and these macros are correct in clang.
        if constexpr(std::is_integral_v<T>)
            return 0;
        else if constexpr(std::is_same_v<T, float>)
            return FLT_DIG;
        else if constexpr(std::is_same_v<T, double>)
            return DBL_DIG;
        else if constexpr(std::is_same_v<T, long double>)
            return LDBL_DIG;
    }
}// namespace

template<auto Sign, auto Integral, auto Digits, auto Remainder>
struct FloatConstant
{
private:
    static constexpr auto makeFloat()
    {
        if constexpr (Digits != 0)
        {
            const auto signFlag = IsPositive ? 1. : -1.;
            const auto actualRemainder = NumberOfDigits == 0 ? 0 : (static_cast<long double>(Decimal)/NumberOfDigits);
            return signFlag*(WholeNumber+actualRemainder);
        }
        else
            if constexpr (IsPositive)
            { 
                if constexpr(Integral > UINT_MAX)
                    return static_cast<unsigned int>(Integral);
                else if constexpr(Integral > ULONG_MAX)
                    return static_cast<unsigned long>(Integral);
                else if constexpr(Integral > ULLONG_MAX)
                    return static_cast<unsigned long long>(Integral);
            }
            else
            {
                if constexpr(Integral > INT_MAX)
                    return static_cast<int>(Integral);
                else if constexpr(Integral > LONG_MAX)
                    return static_cast<long>(Integral);
                else if constexpr(Integral > LLONG_MAX)
                    return static_cast<long long>(Integral);
            }
    }

public:
    static constexpr auto IsPositive = Sign;
    static constexpr auto WholeNumber = Integral;
    static constexpr auto NumberOfDigits = Digits;
    static constexpr auto Decimal = Remainder;

    static constexpr auto value = makeFloat();
};

namespace{
    constexpr auto makeFloatConstantImpl = [](const auto& fl)
    {
        using InputType = std::decay_t<decltype(fl)>;
        const auto flag = fl >= 0;
        const auto whole = flag ? static_cast<unsigned long long>(fl) : static_cast<unsigned long long>(fl*-1);
        const unsigned long long digits = integralPow<10ull, std::min(9, getSignificantDigitsForType<InputType>())>();
        const auto remainder = std::is_floating_point_v<InputType> ? std::llround(static_cast<long double>(digits)*(static_cast<long double>(fl)-static_cast<long double>(whole))) : 0l;

        return std::make_tuple(flag, whole, digits, remainder);
    };
}// namespace
