#ifndef EVERY_OTHER_H
#define EVERY_OTHER_H

#include <simdpp/simd.h>
#include "lookup_templates.h"

#include "typelookup.h"

template <unsigned simd_num_bits_per_element,
          unsigned simd_vector_length,
          std::size_t... InnerArrayIndices>
typename lookup_vec<64,
                    simd_num_bits_per_element * simd_vector_length / 64>::type
get_uint_vec(std::index_sequence<InnerArrayIndices...>) {
  const typename lookup_vec<64, simd_num_bits_per_element* simd_vector_length /
                                    64>::type v1 =

      simdpp::make_uint(
          (InnerArrayIndices / (simd_num_bits_per_element / 64))
              ? 0
              : std::numeric_limits<
                    typename lookup_unsigned<64>::type>::max()...);
  return v1;
}

template <unsigned simd_num_bits_per_element, unsigned simd_vector_length>
struct EveryOther {
  static constexpr decltype(auto) every_other_mask() {
    static_assert(simd_num_bits_per_element >= 8);
    static_assert(simd_vector_length >= 1);  // Or should it be greater than 1?
    if constexpr (simd_num_bits_per_element > 64) {
      typedef
          typename lookup_vec<64, simd_num_bits_per_element *
                                      simd_vector_length / 64>::type vectype;

      const vectype v2 = simdpp::make_zero();
      const vectype v1 =
          get_uint_vec<simd_num_bits_per_element, simd_vector_length>(
              std::make_index_sequence<2 * simd_num_bits_per_element / 64>{});
      const auto mask = simdpp::cmp_eq(v1, v2);
      return mask;
    } else {
      typedef typename lookup_vec<simd_num_bits_per_element,
                                  simd_vector_length>::type vectype;
      typedef typename lookup_unsigned<simd_num_bits_per_element>::type
          simd_element_type;
      const vectype v1 =
          simdpp::make_uint(std::numeric_limits<simd_element_type>::max(), 0);
      const vectype v2 = simdpp::make_zero();
      const auto mask = simdpp::cmp_eq(v1, v2);
      return mask;
    }
  }
};

#endif  // EVERY_OTHER_H
