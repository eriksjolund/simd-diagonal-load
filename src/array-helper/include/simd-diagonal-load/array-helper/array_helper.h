#ifndef ARRAY_HELPER_H
#define ARRAY_HELPER_H

#include <experimental/array>

#include <simd-diagonal-load/algorithm/lookup_templates.h>
#include <simd-diagonal-load/algorithm/power_of_two.h>
#include <simd-diagonal-load/array-helper/array_helper.h>
#include "extract_helper.h"

#include <set>
#include <cassert>
#include <iostream>
#include <cassert>

namespace {
constexpr unsigned two_to_the_power_of_impl(unsigned num) {
    if (num == 0) { return 1u; }
        return 2u * two_to_the_power_of_impl(num - 1u);
}
}

namespace detail {

template <std::size_t N_inner,
          std::size_t N_outer,
          typename Elem,
          std::size_t OuterArrayIndex,
          std::size_t... InnerArrayIndices>
constexpr std::array<Elem, N_inner> split_array_impl2(
    std::array<Elem, N_inner * N_outer>&& array,
    std::index_sequence<InnerArrayIndices...>) {
  return std::experimental::fundamentals_v2::make_array(
      std::get<(OuterArrayIndex + 1) * InnerArrayIndices>(array)...);
}

template <std::size_t N_inner,
          std::size_t N_outer,
          typename Elem,
          std::size_t... OuterArrayIndices>
constexpr std::array<std::array<Elem, N_inner>, N_outer> split_array_impl(
    std::array<Elem, N_inner * N_outer>&& array,
    std::index_sequence<OuterArrayIndices...>) {
  return std::experimental::fundamentals_v2::make_array(
      ::detail::split_array_impl2<N_inner, N_outer, Elem, OuterArrayIndices>(
          std::forward<std::array<Elem, N_inner * N_outer>>(array),
          std::make_index_sequence<N_inner>{})...);
}
}  // namespace detail

constexpr unsigned get_index(unsigned i,
                             unsigned j,
                             unsigned N_inner,
                             unsigned N_outer,
                             unsigned num_vertical_mixing) {
  assert(is_power_of_two(N_inner));
  assert(is_power_of_two(N_outer));


  const auto tmp = N_inner / two_to_the_power_of_impl(num_vertical_mixing);
  const auto vector_length = N_inner * N_outer;
  return (j / tmp) * (vector_length / two_to_the_power_of_impl(num_vertical_mixing)) + (j % tmp) +
         (i * tmp);
}

template <std::size_t N_inner, std::size_t N_outer, typename Elem>

constexpr std::array<std::array<Elem, N_inner>, N_outer> split_array(
    std::array<Elem, N_inner * N_outer>&& array) {
  return detail::split_array_impl<N_inner, N_outer, Elem>(
      std::forward<std::array<Elem, N_inner * N_outer>>(array),
      std::make_index_sequence<N_outer>{});
}

template <unsigned num_vertical_mixing,
          std::size_t N_inner,
          std::size_t N_outer,
          typename Elem>
std::array<std::array<Elem, N_inner>, N_outer> split_array2(
    std::array<Elem, N_inner * N_outer>&& array) {
  std::array<std::array<Elem, N_inner>, N_outer> result;

  std::set<unsigned> index_set;
  for (unsigned i = 0; i < (N_inner * N_outer); ++i) {
    index_set.insert(i);
  }

  for (unsigned i = 0; i < N_outer; ++i) {
    for (unsigned j = 0; j < N_inner; ++j) {
      unsigned index = get_index(i, j, N_inner, N_outer, num_vertical_mixing);
      auto num_erased = index_set.erase(index);
      assert(num_erased == 1);
      result.at(i).at(j) = array.at(index);
    }
  }
  assert(index_set.empty());
  return result;
}

template <std::size_t N_inner,
          std::size_t N_outer,
          unsigned num_vertical_mixing,
          typename Elem>
std::array<Elem, N_inner * N_outer> concat_arrays11(
    std::array<std::array<Elem, N_inner>, N_outer>&& array) {
  std::set<unsigned> index_set;
  for (unsigned i = 0; i < (N_inner * N_outer); ++i) {
    index_set.insert(i);
  }
  std::array<Elem, N_inner * N_outer> result;

  for (unsigned i = 0; i < N_outer; ++i) {
    for (unsigned j = 0; j < N_inner; ++j) {
      unsigned index = get_index(i, j, N_inner, N_outer, num_vertical_mixing);
      auto num_erased = index_set.erase(index);
      assert(num_erased == 1);
      result.at(index) = array.at(i).at(j);
    }
  }
  return result;
}

///////////

namespace detail {

template <std::size_t N_inner,
          std::size_t N_outer,
          unsigned num_vertical_mixing,
          typename Elem,
          std::size_t... InnerArrayIndices>
constexpr std::array<Elem, N_inner * N_outer> concat_arrays_impl(
    std::array<std::array<Elem, N_inner>, N_outer>&& array,
    std::index_sequence<InnerArrayIndices...>) {
  return std::experimental::fundamentals_v2::make_array(
      std::get<(InnerArrayIndices / two_to_the_power_of<num_vertical_mixing>()) +
               (InnerArrayIndices % (N_inner / two_to_the_power_of<num_vertical_mixing>()))>(
          std::get<(InnerArrayIndices % two_to_the_power_of<num_vertical_mixing>()) +
                   (InnerArrayIndices / (N_inner * two_to_the_power_of<num_vertical_mixing>()))>(
              array))...);
}
}  // namespace detail

template <std::size_t N_inner,
          std::size_t N_outer,
          unsigned num_vertical_mixing,
          typename Elem>
constexpr std::array<Elem, N_inner * N_outer> concat_arrays(
    std::array<std::array<Elem, N_inner>, N_outer>&& array) {
  return detail::concat_arrays_impl<N_inner, N_outer, num_vertical_mixing,
                                    Elem>(
      std::forward<std::array<std::array<Elem, N_inner>, N_outer>>(array),
      std::make_index_sequence<N_inner * N_outer>{});
}

///////////

template <unsigned num_bits, unsigned N, std::size_t... I>
decltype(auto) to_array_impl(const typename lookup_vec<num_bits, N>::type& vec,
                             std::index_sequence<I...>) {
  return std::experimental::fundamentals_v2::make_array(
      extract_more_generic<num_bits, N, I>(vec)...);
}

template <unsigned num_bits,
          unsigned N,
          typename Indices = std::make_index_sequence<N>>
decltype(auto) to_array(const typename lookup_vec<num_bits, N>::type& vec) {
  return to_array_impl<num_bits, N>(vec, Indices{});
}

template <unsigned num_bits,
          unsigned N,
          unsigned N_return_type,
          unsigned SHIFT,
          std::size_t... I>
typename lookup_vec<num_bits, N_return_type>::type from_array_to_simdvec_impl(
    const std::array<typename lookup_unsigned<num_bits>::type, N>& array,
    std::index_sequence<I...>) {
  return simdpp::make_uint(std::get<I + SHIFT>(array)...);
}

template <unsigned num_bits, unsigned N, unsigned N_return_type, unsigned SHIFT>
typename lookup_vec<num_bits, N_return_type>::type from_array_to_simdvec(
    const std::array<typename lookup_unsigned<num_bits>::type, N>& array) {
  // libsimdpp does not have simdpp::make_uint() for neither N == 32 nor N == 64
  if constexpr (N_return_type == 32 || N_return_type == 64) {
    typename lookup_vec<num_bits, N_return_type / 2>::type vec1 =
        from_array_to_simdvec<num_bits, N, N_return_type / 2, SHIFT + 0>(array);
    typename lookup_vec<num_bits, N_return_type / 2>::type vec2 =
        from_array_to_simdvec<num_bits, N, N_return_type / 2,
                              SHIFT + (N_return_type / 2)>(array);

    return simdpp::combine(vec1, vec2);
  } else {
    return from_array_to_simdvec_impl<num_bits, N, N_return_type, SHIFT>(
        array, std::make_index_sequence<N_return_type>{});
  }
}

#endif  // ARRAY_HELPER_H
