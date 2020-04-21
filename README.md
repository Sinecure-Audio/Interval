This is a small utility library that implements two simple classes that represent an interval, as well as two containers that use these intervals as lookups. The purpose of these intervals is to hold a value, constrain it within an inclusive interval (a set of two numbers), and to provide arithmetic operations between themselves, other intervals, and arithmetic types. Currently, this library requires c++17 or higher to compile. Additionally, building and running the tests requires Catch2 to be installed.

There are two kinds of intervals in this library: static intervals, whose bounds and bounding behavior are a part of their type, and dynamic intervals, whose bounds and bounding behavior can be changed at runtime.

Similarly, there are two kinds of containers- a subclass of std::array which changes the operator[] and at() functions to use a static interval instead of a size_t, and a subclass of std::vector, which changes the operator[] and at() functions to use a dynamic interval instead of a size_t.


EXAMPLES:

Use a wrapping interval to represent a normalized phase (0-1) to read the correct value from std::sin, which expects 0-twopi.
```c++
constexpr auto intervalSin(const INTERVAL_TYPE(0.0, 2.0*3.14159, IntervalWrapModes::Wrap)& inputInterval) {
    return std::sin(inputInterval.getValue());
}

auto currentPosition = MAKE_INTERVAL(0.0, 1.0, IntervalWrapModes::Wrap){};

auto readSin(double normalizedAngularVelocity) {
    currentPosition += normalizedAngularVelocity;//incremet the current position by the normalized angular velocity. If the current position is > 1, it will wrap back to 0.
    return intervalSin(currentPosition);// currentPosition is automatically converted to a range of 0 to twopi, and its value is scaled accordingly.
}
```
Now let's look at an example of a ramp generator with a dynamic interval.
```c++
DynamicInterval<double> interval{};//makes an interval with a start end and value of 0.
constexpr auto rampLength = 20; 
// By default, an interval will clamp any value outside of its bounds to the min (if the value is smaller) or max (if the value is larger) of the bounds
void getNewTargetValue(double newTarget) {
	interval.setStart(interval.getValue());
	interval.setEnd(newTarget);
} 

auto getNextRampValue() {
	const auto stepSize = interval.getLength()/rampLength;
	return interval+=stepSize;
}
```