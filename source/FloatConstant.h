#pragma once

#include <tuple>
#include <cfloat>
#include <climits>
#include <cmath>
#include <limits>
#include <algorithm>

namespace {
	namespace detail {
		template<typename T>
		constexpr T abs(const T& value) {
			if constexpr(std::is_unsigned_v<T>)
				return value;
			else
				return	value >= T{0} ? value : value * T{-1};
			}

		template<typename T, typename U>
		constexpr auto fmod(const T& value, const U& modulus) {
			if constexpr (std::is_integral_v<T> && std::is_integral_v<U>)
				return value % modulus;

			using ReturnType = std::conditional_t<std::is_floating_point_v<T> && std::is_floating_point_v<U>, T, std::conditional_t<std::is_floating_point_v<U>, U, T>>;
			if (modulus == 0 || value == 0)
				return ReturnType{ 0 };

			const auto sign = value > 0 ? ReturnType{ 1} : ReturnType{ -1 };
			const auto absVal = detail::abs(value);
			const auto divVal = static_cast<unsigned long long>(detail::abs(value / modulus));

			if (absVal < detail::abs(modulus) || divVal == 0)
				return ReturnType{ value };

			return ReturnType{ sign * (absVal / static_cast<ReturnType>(divVal) - modulus) };
		}



		template<auto Base, auto Power, auto Value = Base>
		class integralPowImpl : public std::conditional_t<(Power > 0), integralPowImpl<Base, Power - 1, Value*Base>, integralPowImpl<Base, 0, Value>> {};

		template <auto Base, auto Value>
		class integralPowImpl<Base, 0, Value> {
		public:
			static constexpr auto getValue() { return Value;}
		};

	 //   //A simple integral constexpr pow, used to get a value to convert the decimal part of the float to or from an integral type.
	 //   template <auto Base, auto Power>
	 //   constexpr auto integralPowImpl(const decltype(Base)& val)
	 //   {
		//	if constexpr (Power > 0)
		//	{
		//		if (Power <= 0)
		//			return val;
		//		//static_assert(Power > 0, "Uhoh");
		//		return integralPowImpl<Base, Power-1>(val * Base);
		//	}
	 //       else
	 //           return val;
	 //   }

		//template <auto Base>
		//constexpr auto integralPowImpl<Base,0>(const decltype(Base)& val)
		//{
		//	return val;
		//}
		template <typename T, typename U, typename V>
		constexpr unsigned long long iPowImpl(const T& base, const U& power, const V& value) {
			if (power <= 0)
				return value;
			if (power == 1)
				return value*base;
			else
				return iPowImpl(base, power-1, value*base);
		}

		template <typename T, typename U>
		constexpr unsigned long long iPow(const T& base, const U& power) {
			if (power == 0)
				return T{1};
			else if (power == 1)
				return base;
			else if (power > 1)
			{
				return iPowImpl(base, power-1, base);
			}
			else
				return 0;
		}

		template<typename T>
		constexpr auto round(const T& value) {
			//const auto absValue = detail::abs(value);
			//const long long truncatedValue = static_cast<long long>(detail::abs(value));
			if (value >= 0) {
				return (value - static_cast<long long>(value)) > .5L ? static_cast<long long>(value)+1 : static_cast<long long>(value);// 
				//return absValue - truncatedValue > .5L ? truncatedValue + 1 : truncatedValue;
			}
			else {
				return (detail::abs(value) - static_cast<long long>(detail::abs(value))) > .5L ? static_cast<long long>(value) - 1 : static_cast<long long>(value)	;//
				//return absValue - truncatedValue > .5L ? static_cast<long long>(value) + 1 : static_cast<long long>(value);
			}
		}

		template <class T>
		constexpr size_t numDigits(T number) {
		    size_t digits = number < 0 ? 1 : 0;// remove this ternary if '-' counts as a digit
		    while (number) {
		        number /= T{10};
		        digits++;
		    }
		    return digits;
		}
	}// namespace
}

template<bool IsPositive, unsigned long long WholeNumber, size_t Scalar, unsigned long long Remainder, typename T>
struct FloatConstant
{
private:
	using Type = std::decay_t<T>;
    static constexpr Type makeFloat() 
    {
        if constexpr (Scalar == 0)	
        	return IsPositive*WholeNumber;
        else{
            constexpr Type signFlag = IsPositive ? 1 : -1;

            // constexpr Type Divisor = 1.0 / static_cast<Type>(detail::integralPow<10ull, NumberOfDigits+1>::getValue());
			constexpr auto actualRemainder = T(Remainder)/T(Scalar);//static_cast<Type>(Remainder*Divisor);

             return signFlag*(WholeNumber + actualRemainder);
        }
    }

public:
    static constexpr Type value = makeFloat();
};

namespace {
	template<typename InputType>
	constexpr auto makeFloatConstantImpl(const InputType& fl)
	{
		if constexpr (std::is_integral_v<InputType>)
			return std::make_tuple(fl >= 0, static_cast<unsigned long long>(fl), 0, 0, InputType{});
		else {
			const bool flag = fl >= 0;
			const unsigned long long whole = static_cast<unsigned long long>(detail::abs(fl));
			const size_t numWholeDigits = detail::numDigits(whole);
			const size_t numberOfDecimalPlaces = std::numeric_limits<InputType>::digits10 - numWholeDigits;
			const unsigned long long scalar = detail::iPow(10ull, numberOfDecimalPlaces);
			const unsigned long long remainder = static_cast<unsigned long long>(detail::round(static_cast<long double>(scalar) * (static_cast<long double>(detail::abs(fl)) - static_cast<long double>(whole))));

			return std::make_tuple(flag, whole, scalar, remainder, InputType{});
		}
	}
}// namespace


// the goal of the remainder value is to capture all useful digits after the decimal places
// the number of useful digits is a function of the whole number i.e. the larger the whole number, the less useful the decimal places are
// 
