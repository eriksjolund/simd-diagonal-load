#ifndef CREATE_MASKS_H
#define CREATE_MASKS_H

#include <functional>
#include <array>

#include <simdpp/simd.h>

#include "lookup_templates.h"

#include "typelookup.h"

#include "every_other.h"
#include "power_of_two.h"

template <unsigned simd_num_bits_per_element,
          unsigned simd_vector_length,
          num_loads_t num_loads,
          num_vertical_subdivisions_t num_vertical_subdivisions,
          num_vertical_mixing_t num_vertical_mixing,
          std::size_t... Is>
inline __attribute__((always_inline)) decltype(auto) create_mask_tuple_impl(
    std::index_sequence<Is...>) {
  constexpr const auto num_masks = logtwo(
      num_blenders<simd_vector_length, num_loads, num_vertical_subdivisions>());

  return std::make_tuple(
      EveryOther<simd_num_bits_per_element * two_to_the_power_of<1 + Is>() *
                     two_to_the_power_of<num_vertical_mixing>() / 2,
                 2 * simd_vector_length /
                     (two_to_the_power_of<1 + Is>() *
                      two_to_the_power_of<num_vertical_mixing>())>::every_other_mask()...);
}

template <unsigned simd_num_bits_per_element,
          unsigned simd_vector_length,
          num_loads_t num_loads,
          num_vertical_subdivisions_t num_vertical_subdivisions,
          num_vertical_mixing_t num_vertical_mixing>
inline __attribute__((always_inline)) decltype(auto) create_mask_tuple() {
  constexpr const auto num_masks = logtwo(
      num_blenders<simd_vector_length, num_loads, num_vertical_subdivisions>());

  return create_mask_tuple_impl<simd_num_bits_per_element, simd_vector_length,
                                num_loads, num_vertical_subdivisions,
                                num_vertical_mixing>(
      std::make_index_sequence<num_masks>{});
}

#endif  // CREATE_MASKS_H
