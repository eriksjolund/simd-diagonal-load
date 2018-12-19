#ifndef CHECK_TEMPLATE_PARAMETERS_H
#define CHECK_TEMPLATE_PARAMETERS_H

#include <type_traits>
#include "algorithm_types.h"
#include "power_of_two.h"


template <typename T,
          unsigned simd_vector_length,
          num_vertical_subdivisions_t num_vertical_subdivisions,
          num_vertical_mixing_t num_vertical_mixing,
          num_loads_t num_loads>
constexpr void check_template_parameters() {
  static_assert(is_power_of_two(simd_vector_length));
  static_assert(is_power_of_two(num_loads));

  static_assert(two_to_the_power_of<unsigned, num_vertical_mixing>() <=
                (simd_vector_length / two_to_the_power_of<unsigned, num_vertical_subdivisions>()));

  static_assert(simd_vector_length >= (num_loads * two_to_the_power_of<unsigned, num_vertical_subdivisions>()));
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
