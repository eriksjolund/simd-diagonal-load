#ifndef EXTRACT_HELPER_H
#define EXTRACT_HELPER_H

#include <simdpp/simd.h>
#include <simd-diagonal-load/algorithm/lookup_templates.h>

template <unsigned num_bits, unsigned N, unsigned K>
typename lookup_unsigned<num_bits>::type extract_more_generic(
    const typename lookup_vec<num_bits, N>::type& vec) {
  // libsimdpp does not support simdpp::extract with vectors bigger than 128
  // bit.
  // As a workaround we use simdpp::split
  if constexpr (num_bits * N >= 256) {
    typename lookup_vec<num_bits, N / 2>::type half_vec_1{};
    typename lookup_vec<num_bits, N / 2>::type half_vec_2{};
    simdpp::split(vec, half_vec_1, half_vec_2);
    if constexpr (K < N / 2) {
      return extract_more_generic<num_bits, N / 2, K>(half_vec_1);
    } else {
      return extract_more_generic<num_bits, N / 2, K - (N / 2)>(half_vec_2);
    }
  } else {
    return simdpp::extract<K>(vec);
  }
}
#endif  // EXTRACT_HELPER_H
