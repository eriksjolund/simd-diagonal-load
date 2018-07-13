#ifndef USE_BLEND_MASKS_H
#define USE_BLEND_MASKS_H

#include <functional>
#include <array>

#include <simdpp/simd.h>

#include "lookup_templates.h"

#include "typelookup.h"

#include "every_other.h"

#include "power_of_two.h"

template <unsigned iteration,
          typename integerT,
          unsigned simd_vector_length,
          unsigned num_loads,
          unsigned num_vertical_subdivisions,
          unsigned num_vertical_mixing,
          typename Tuple,
          std::size_t Index>
constexpr inline __attribute__((always_inline)) void use_blend_masks_impl(
    typename TypeLookup<integerT,
                        simd_vector_length,
                        num_loads,
                        num_vertical_subdivisions>::blenders_t& blenders,
    const Tuple& mask_tuple) {
  constexpr const auto blenders_size =
      std::tuple_size<typename

                      TypeLookup<integerT, simd_vector_length, num_loads,
                                 num_vertical_subdivisions>::blenders_t>::value;

  constexpr const unsigned current_masklength =
      two_to_the_power_of<Index + 1>();
  constexpr const int from_index =
      (iteration + ((current_masklength)-1)) % blenders_size;

  constexpr const int to_index =
      (iteration + (((current_masklength) / 2) - 1)) % blenders_size;

  constexpr const auto num_masks = logtwo(
      num_blenders<simd_vector_length, num_loads, num_vertical_subdivisions>());

  blenders[to_index] = simdpp::blend(blenders[to_index], blenders[from_index],
                                     std::get<Index>(mask_tuple));

  if constexpr (Index > 0) {
    return use_blend_masks_impl<iteration, integerT, simd_vector_length,
                                num_loads, num_vertical_subdivisions,
                                num_vertical_mixing, Tuple, Index - 1>(
        blenders, mask_tuple);
  } else {
    return;
  }
}

template <unsigned iteration,
          typename integerT,
          unsigned simd_vector_length,
          unsigned num_loads,
          unsigned num_vertical_subdivisions,
          unsigned num_vertical_mixing,
          typename Tuple>

constexpr inline __attribute__((always_inline)) void use_blend_masks(
    typename TypeLookup<integerT,
                        simd_vector_length,
                        num_loads,
                        num_vertical_subdivisions>::blenders_t& blenders,
    const Tuple& mask_tuple) {
  return use_blend_masks_impl<iteration, integerT, simd_vector_length,
                              num_loads, num_vertical_subdivisions,
                              num_vertical_mixing, Tuple,
                              std::tuple_size<Tuple>::value - 1>(

      blenders, mask_tuple);
}

#endif  // USE_BLEND_MASKS_H
