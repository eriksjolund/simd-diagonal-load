#ifndef POWER_OF_TWO_H
#define POWER_OF_TWO_H

#include <cstdint>
#include <limits>
constexpr bool is_power_of_two(int num) {
  return num && ((num & (num - 1)) == 0);
}



// template <typename T, std::uint8_t num>
// struct two_to_the_power_of_metaprog{
//    static_assert(std::is_integral<T>::value);
//    static_assert(std::is_unsigned<T>::value);
//    static_assert(! (two_to_the_power_of_metaprog<T, num - 1u>::value > (std::numeric_limits< T >::max() / 2) ),
//       "two_to_the_power_of_metaprog<T, num>::value would overflow");
//    static constexpr T value = 2 * two_to_the_power_of_metaprog< T, num - 1 >::value;
// };

// template <typename T, std::uint8_t num>
// struct two_to_the_power_of_metaprog<0> {
//     static_assert(std::is_integral<T>::value);
//     static_assert(std::is_unsigned<T>::value);
//     static constexpr T value = 1;
// };



template <typename T, std::uint8_t num>
constexpr T two_to_the_power_of() {
    static_assert(std::is_integral<T>::value);
    static_assert(std::is_unsigned<T>::value);
  if constexpr (num > 0) {
        constexpr const auto half_max = std::numeric_limits< T >::max() / 2;
        constexpr const auto previous = two_to_the_power_of<T, num - 1u>();
        static_assert(!(previous > half_max));
        return 2u * previous;
  } else {
    return 1u;
  }
}

constexpr unsigned logtwo(unsigned n) {
  return n == 1 ? 0 : 1 + logtwo(n / 2);
}

#endif  // POWER_OF_TWO_H
