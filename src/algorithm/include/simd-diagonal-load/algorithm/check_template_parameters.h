#ifndef CHECK_TEMPLATE_PARAMETERS_H
#define CHECK_TEMPLATE_PARAMETERS_H

#include <type_traits>
#include "power_of_two.h"

template <typename T,
          unsigned simd_vector_length,
          unsigned num_vertical_subdivisions,
          unsigned num_vertical_mixing,
          unsigned num_loads>
constexpr void check_template_parameters() {
  static_assert(is_power_of_two(simd_vector_length));
  static_assert(is_power_of_two(num_loads));

  static_assert(two_to_the_power_of<num_vertical_mixing>() <=
                (simd_vector_length / two_to_the_power_of<num_vertical_subdivisions>()));

  static_assert(simd_vector_length >= (num_loads * two_to_the_power_of<num_vertical_subdivisions>()));
  static_assert(std::is_same<T, std::uint8_t>::value ||
                std::is_same<T, std::uint16_t>::value ||
                std::is_same<T, std::uint32_t>::value ||
                std::is_same<T, std::uint64_t>::value ||
                std::is_same<T, std::int8_t>::value ||
                std::is_same<T, std::int16_t>::value ||
                std::is_same<T, std::int32_t>::value ||
                std::is_same<T, std::int64_t>::value);
}

#endif  // CHECK_TEMPLATE_PARAMETERS_H
