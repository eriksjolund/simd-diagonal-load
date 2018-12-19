#ifndef TWO_TO_THE_POWER_OF_H
#define TWO_TO_THE_POWER_OF_H
#include <cassert>
#include <limits>
namespace {

// This is confusing. There is also a
// two_to_the_power_of_impl()
// in src/array-helper/include/simd-diagonal-load/array-helper/array_helper.h

// runtime-version

template <typename T>
T two_to_the_power_of_impl(std::uint8_t num) {
    static_assert(std::is_integral<T>::value);
    static_assert(std::is_unsigned<T>::value);
    if (num == 0) { return static_cast<T>(1u); }
        auto half_max = std::numeric_limits< T >::max() / 2;
        auto previous = two_to_the_power_of_impl<T>(num - 1u);
        assert(!(previous > half_max));
        auto twice_as_much = 2u * previous;
        return twice_as_much;
}
}

#endif  // TWO_TO_THE_POWER_OF_H
