#ifndef CHECK_TEMPLATE_PARAMETERS_H
#define CHECK_TEMPLATE_PARAMETERS_H

#include "power_of_two.h"

template <unsigned simd_num_bits_per_element,
          unsigned simd_vector_length,
          unsigned num_vertical_subdivisions,
          unsigned num_vertical_mixing,
          unsigned num_loads>
constexpr void check_template_parameters() {
  static_assert(is_power_of_two(num_vertical_subdivisions));
  static_assert(is_power_of_two(num_vertical_mixing));
  static_assert(is_power_of_two(simd_vector_length));
  static_assert(is_power_of_two(num_loads));

  static_assert(num_vertical_mixing <=
                (simd_vector_length / num_vertical_subdivisions));

  static_assert(simd_vector_length >= (num_loads * num_vertical_subdivisions));
  static_assert(
      simd_num_bits_per_element == 8 || simd_num_bits_per_element == 16 ||
          simd_num_bits_per_element == 32 || simd_num_bits_per_element == 64,
      "simd_num_bits_per_element should 8, 16, 32 or 64");
}

#endif  // CHECK_TEMPLATE_PARAMETERS_H
