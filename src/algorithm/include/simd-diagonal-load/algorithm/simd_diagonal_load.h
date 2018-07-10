#ifndef SIMD_DIAGONAL_LOAD_H
#define SIMD_DIAGONAL_LOAD_H

#include <functional>
#include <array>
#include <string>
#include <iostream>
#include <simdpp/simd.h>

#include "lookup_templates.h"

#include "typelookup.h"

#include "every_other.h"
#include "use_blend_masks.h"
#include "power_of_two.h"

#include "create_masks.h"
#include "use_blend_masks.h"
#include "check_template_parameters.h"

template <unsigned simd_num_bits_per_element,
          unsigned simd_vector_length,
          unsigned num_loads,
          unsigned num_vertical_subdivisions,
          unsigned num_vertical_mixing,
          unsigned current_masklength>
constexpr inline __attribute__((always_inline)) unsigned calculate_factor() {
  constexpr const unsigned initial_masklength =
      simd_vector_length / (num_loads * num_vertical_subdivisions);
  constexpr const unsigned current_segment_length =
      (simd_vector_length / (num_vertical_subdivisions * num_loads * 2)) /
      (initial_masklength / current_masklength);
  static_assert(current_segment_length != 0);

  constexpr const unsigned extract_segment_length =
      (simd_vector_length / num_vertical_subdivisions) / num_vertical_mixing;
  static_assert(extract_segment_length != 0);
  if constexpr (current_segment_length >= extract_segment_length) {
    constexpr const unsigned factor =
        current_segment_length * num_vertical_subdivisions;
    static_assert(factor <= simd_vector_length / 2);
    return factor;
  } else {
    return current_segment_length;
  }
}

template <unsigned iteration,
          unsigned simd_num_bits_per_element,
          unsigned simd_vector_length,
          unsigned num_loads,
          unsigned num_vertical_subdivisions,
          unsigned num_vertical_mixing,
          unsigned current_masklength>
class UnrollBlending {
  using lookup = TypeLookup<simd_num_bits_per_element,
                            simd_vector_length,
                            num_loads,
                            num_vertical_subdivisions>;

 public:
  static inline __attribute__((always_inline)) void blending(
      typename lookup::blenders_t&& blenders) {
    if constexpr (current_masklength > 1) {
      constexpr const auto blenders_size =
          std::tuple_size<typename lookup::blenders_t>::value;

      constexpr const int from_index =
          (iteration + ((current_masklength)-1)) % blenders_size;

      constexpr const int to_index =
          (iteration + (((current_masklength) / 2) - 1)) % blenders_size;

      constexpr const unsigned factor1 =
          calculate_factor<simd_num_bits_per_element, simd_vector_length,
                           num_loads, num_vertical_subdivisions,
                           num_vertical_mixing, current_masklength>();

      constexpr const unsigned simd_num_bits_per_element_in_mask =
          simd_num_bits_per_element * factor1;
      constexpr const unsigned simd_vector_length_in_mask =
          simd_vector_length * simd_num_bits_per_element /
          simd_num_bits_per_element_in_mask;

      static_assert(simd_num_bits_per_element_in_mask >= 8);
      static_assert(simd_vector_length_in_mask >= 1);

      std::get<to_index>(blenders) = simdpp::blend(
          std::get<to_index>(blenders), std::get<from_index>(blenders),
          EveryOther<simd_num_bits_per_element_in_mask,
                     simd_vector_length_in_mask>::every_other_mask());

      UnrollBlending<iteration, simd_num_bits_per_element, simd_vector_length,
                     num_loads, num_vertical_subdivisions, num_vertical_mixing,
                     current_masklength / 2>::
          blending(std::forward<typename lookup::blenders_t>(blenders));
    }
  }
};

template <unsigned simd_num_bits_per_element,
          unsigned simd_vector_length,
          unsigned num_loads,
          unsigned num_vertical_subdivisions,
          unsigned num_vertical_mixing,
          unsigned matrix_width>

class SimdDiagonalSlider {
  typedef TypeLookup<simd_num_bits_per_element,
                     simd_vector_length,
                     num_loads,
                     num_vertical_subdivisions>
      lookup;
  static_assert(std::tuple_size<typename lookup::blenders_t>::value != 0,
                "blenders array should never be empty");

 public:
  constexpr SimdDiagonalSlider() : blenders_{} {
    check_template_parameters<simd_num_bits_per_element, simd_vector_length,
                              num_vertical_subdivisions, num_vertical_mixing,
                              num_loads>();
  }

  template <unsigned iteration>
  constexpr static void check_valid_iteration_number() {
    static_assert(iteration >= 0);
    static_assert(iteration < num_handle_diagonal_calls());
    return;
  }

  constexpr static unsigned extra_save_columns_to_num_load_bigger_than_one() {
    return (num_loads - 1) *
           (simd_vector_length / (num_vertical_subdivisions * num_loads));
  }

  constexpr static unsigned number_save_columns_needed() {
    return matrix_width + extra_save_columns_to_num_load_bigger_than_one();
  }

  constexpr static unsigned max_pos() {
    constexpr const auto max_pos =
        (simd_vector_length / num_vertical_subdivisions) - 1;
    static_assert(max_pos >= 0);
    return max_pos;
  }

  constexpr static unsigned num_handle_diagonal_calls() {
    return matrix_width + max_pos();
  }

  template <unsigned iteration>
  constexpr static unsigned start_position_of_relevant_diagonal_elements() {
    check_valid_iteration_number<iteration>();
    return max_pos() -
           std::min(max_pos(), num_handle_diagonal_calls() - iteration - 1);
  }

  template <unsigned iteration>
  constexpr static unsigned end_position_of_relevant_diagonal_elements() {
    check_valid_iteration_number<iteration>();
    return std::min(max_pos(), iteration);
  }

  template <unsigned iteration>
  constexpr inline typename lookup::vectype& diagonal() {
    check_valid_iteration_number<iteration>();
    constexpr const auto blenders_size =
        std::tuple_size<typename lookup::blenders_t>::value;
    return std::get<iteration % (blenders_size)>(blenders_);
  }

  template <unsigned iteration>
  constexpr inline void save_column(
      const typename lookup::vectype&& column_vec) {
    if constexpr (iteration == 0) {
      // fix this .... Maybe create a constexpr function?

      for (auto& blender : blenders_) {
        blender = simdpp::make_zero();
      }
    }

    check_valid_iteration_number<iteration>();
    constexpr const auto blenders_size =
        std::tuple_size<typename lookup::blenders_t>::value;
    std::get<(iteration + ((blenders_size)-1)) % blenders_size>(blenders_) =
        std::forward<const typename lookup::vectype>(column_vec);
  }

  template <unsigned iteration>
  inline void blend_diagonal() {
    check_valid_iteration_number<iteration>();

    constexpr const auto blenders_size =
        std::tuple_size<typename lookup::blenders_t>::value;

    if constexpr (blenders_size > 1) {
      if constexpr (true) {
        constexpr const auto initial_masklength =
            simd_vector_length / (num_loads * num_vertical_subdivisions);

        UnrollBlending<iteration % blenders_size, simd_num_bits_per_element,
                       simd_vector_length, num_loads, num_vertical_subdivisions,
                       num_vertical_mixing, initial_masklength>::
            blending(
                std::forward<typename lookup::blenders_t>(this->blenders_));
      } else {
        const auto mask_tuple =
            create_mask_tuple<simd_num_bits_per_element, simd_vector_length,
                              num_loads, num_vertical_subdivisions,
                              num_vertical_mixing>();

        use_blend_masks<iteration % blenders_size, simd_num_bits_per_element,
                        simd_vector_length, num_loads,
                        num_vertical_subdivisions, num_vertical_mixing>(

            std::forward<typename lookup::blenders_t>(this->blenders_),
            std::forward<decltype(mask_tuple)>(mask_tuple));
      }
    }
  }
  void fill_irrelevant_diagonal_elements_with_zeroes() {
#ifdef FILL_IRRELEVANT_DIAGONAL_ELEMENTS_WITH_ZEROES
    // blenders[ blenders_size - 1] will be the first element of the
    // std::array to be loaded from
    // get_next_column();
    // There is no need to set its content here as it will be
    // overwritten before being read from, hence
    // num_blenders_to_zero_out =  blenders_size - 1
    // instead of
    // num_blenders_to_zero_out = blenders_size
    constexpr const auto num_blenders_to_zero_out = blenders_size - 1;
    for (typename lookup::blenders_t::size_type i = 0;
         i < num_blenders_to_zero_out; ++i) {
      // This loop could be rolled out
      blenders_[i] = simdpp::make_zero();
    }
#endif
  }

  // private:

  typename lookup::blenders_t blenders_;
};

template <
    unsigned iteration,
    unsigned simd_num_bits_per_element,
    unsigned simd_vector_length,
    unsigned num_loads,
    unsigned num_vertical_subdivisions,
    unsigned num_vertical_mixing,
    unsigned matrix_width,
    template <unsigned, unsigned, unsigned, unsigned, unsigned, std::size_t>

    typename MATRIX>
class UnrollInnerLoop {
  typedef TypeLookup<simd_num_bits_per_element,
                     simd_vector_length,
                     num_loads,
                     num_vertical_subdivisions>
      lookup;

 public:
  static inline __attribute__((always_inline)) void inner_loop2(
      SimdDiagonalSlider<simd_num_bits_per_element,
                         simd_vector_length,
                         num_loads,
                         num_vertical_subdivisions,
                         num_vertical_mixing,
                         matrix_width>&& simd_diagonal_slider,
      MATRIX<simd_num_bits_per_element,
             simd_vector_length,
             num_loads,
             num_vertical_subdivisions,
             num_vertical_mixing,
             matrix_width>&& matrix) {
    if constexpr (iteration <
                  std::decay_t<decltype(
                      simd_diagonal_slider)>::num_handle_diagonal_calls()) {
      if constexpr (iteration <
                    std::decay_t<decltype(
                        simd_diagonal_slider)>::number_save_columns_needed()) {
        simd_diagonal_slider.template save_column<iteration>(
            matrix.template get_next_column());
      } else {
        // The zeroes loaded here will only end up outside of the range
        // (startpos, endpos).
        //  matrix.template handle_diagonal<start_pos,
        //  end_pos>(blending_result);
        //
        // We could also skip loading them for perfomance reasons.

#ifdef FILL_IRRELEVANT_DIAGONAL_ELEMENTS_WITH_ZEROES
        simd_diagonal_slider.template save_column<iteration>(
            simdpp::make_zero());
#endif
      }

      simd_diagonal_slider.template blend_diagonal<iteration>();
      matrix.template handle_diagonal<
          std::decay_t<decltype(simd_diagonal_slider)>::
              template start_position_of_relevant_diagonal_elements<
                  iteration>(),
          std::decay_t<decltype(simd_diagonal_slider)>::
              template end_position_of_relevant_diagonal_elements<iteration>()>(

          simd_diagonal_slider.template diagonal<iteration>()

      );

      UnrollInnerLoop<iteration + 1, simd_num_bits_per_element,
                      simd_vector_length, num_loads, num_vertical_subdivisions,
                      num_vertical_mixing, matrix_width, MATRIX>::
          inner_loop2(
              std::forward<SimdDiagonalSlider<
                  simd_num_bits_per_element, simd_vector_length, num_loads,
                  num_vertical_subdivisions, num_vertical_mixing,
                  matrix_width> >(simd_diagonal_slider),
              std::forward<MATRIX<simd_num_bits_per_element, simd_vector_length,
                                  num_loads, num_vertical_subdivisions,
                                  num_vertical_mixing, matrix_width> >(matrix));
    }
  }
};

template <
    unsigned simd_num_bits_per_element,
    unsigned simd_vector_length,
    unsigned num_loads,
    unsigned num_vertical_subdivisions,
    unsigned num_vertical_mixing,
    unsigned matrix_width,
    template <unsigned, unsigned, unsigned, unsigned, unsigned, std::size_t>
    typename MATRIX>
class IterateDiagonalsTemplate {
  typedef TypeLookup<simd_num_bits_per_element,
                     simd_vector_length,
                     num_loads,
                     num_vertical_subdivisions>
      lookup;

 public:
  constexpr IterateDiagonalsTemplate() : simd_diagonal_slider_{} {}
  void iterate_diagonals(MATRIX<simd_num_bits_per_element,
                                simd_vector_length,
                                num_loads,
                                num_vertical_subdivisions,
                                num_vertical_mixing,
                                matrix_width>& matrix) {
    check_template_parameters<simd_num_bits_per_element, simd_vector_length,
                              num_vertical_subdivisions, num_vertical_mixing,
                              num_loads>();

    constexpr const auto blenders_size =
        std::tuple_size<typename lookup::blenders_t>::value;

    static_assert(blenders_size != 0, "blenders array should never be empty");

    constexpr const unsigned iteration = 0;

    UnrollInnerLoop<iteration, simd_num_bits_per_element, simd_vector_length,
                    num_loads, num_vertical_subdivisions, num_vertical_mixing,
                    matrix_width, MATRIX>::
        inner_loop2(

            std::forward<

                SimdDiagonalSlider<simd_num_bits_per_element,
                                   simd_vector_length, num_loads,
                                   num_vertical_subdivisions,
                                   num_vertical_mixing, matrix_width> >(
                simd_diagonal_slider_),
            std::forward<

                MATRIX<simd_num_bits_per_element, simd_vector_length, num_loads,
                       num_vertical_subdivisions, num_vertical_mixing,
                       matrix_width> >(matrix)

        );

    return;
  }

 private:
  SimdDiagonalSlider<simd_num_bits_per_element,
                     simd_vector_length,
                     num_loads,
                     num_vertical_subdivisions,
                     num_vertical_mixing,
                     matrix_width>
      simd_diagonal_slider_;
};

#endif  // SIMD_DIAGONAL_LOAD_H
