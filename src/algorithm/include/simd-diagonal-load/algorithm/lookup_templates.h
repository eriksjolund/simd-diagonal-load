#ifndef LOOKUP_TEMPLATES_H
#define LOOKUP_TEMPLATES_H

#include <simdpp/simd.h>
template <unsigned num_bits>
struct lookup_unsigned {};
template <>
struct lookup_unsigned<8> {
  //  typedef std::uint8_t type;
  typedef unsigned char type;
};

template <>
struct lookup_unsigned<16> {
  typedef std::uint16_t type;
};

template <>
struct lookup_unsigned<32> {
  typedef std::uint32_t type;
};
template <>
struct lookup_unsigned<64> {
  typedef std::uint64_t type;
};

template <unsigned num_bits, unsigned N>
struct lookup_vec {};
template <unsigned N>
struct lookup_vec<8, N> {
  typedef simdpp::uint8<N> type;
};
template <unsigned N>
struct lookup_vec<16, N> {
  typedef simdpp::uint16<N> type;
};
template <unsigned N>
struct lookup_vec<32, N> {
  typedef simdpp::uint32<N> type;
};
template <unsigned N>
struct lookup_vec<64, N> {
  typedef simdpp::uint64<N> type;
};

template <unsigned num_bits, unsigned N>
struct lookup_mask {};
template <unsigned N>
struct lookup_mask<8, N> {
  typedef simdpp::mask_int8<N> type;
};
template <unsigned N>
struct lookup_mask<16, N> {
  typedef simdpp::mask_int16<N> type;
};
template <unsigned N>
struct lookup_mask<32, N> {
  typedef simdpp::mask_int32<N> type;
};
template <unsigned N>
struct lookup_mask<64, N> {
  typedef simdpp::mask_int64<N> type;
};

/*
template <unsigned N>
struct lookup_mask<128, N> {
  typedef simdpp::mask_int128<N> type;
};
template <unsigned N>
struct lookup_mask<256, N> {
  typedef simdpp::mask_int256<N> type;
};
*/

template <unsigned shift,
          unsigned simd_num_bits_per_element,
          unsigned simd_vector_length>
struct lookup_move_r {
  typedef
      typename lookup_vec<simd_num_bits_per_element, simd_vector_length>::type
          vectype;
  static inline auto move_r(const vectype& V1);
};

template <unsigned shift>
struct lookup_move_r<shift, 8, 16> {
  typedef typename lookup_vec<8, 16>::type vectype;
  static inline auto move_r(const vectype& V1) {
    return simdpp::move16_r<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_r<shift, 16, 8> {
  typedef typename lookup_vec<16, 8>::type vectype;
  static inline auto move_r(const vectype& V1) {
    return simdpp::move8_r<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_r<shift, 32, 4> {
  typedef typename lookup_vec<32, 4>::type vectype;
  static inline auto move_r(const vectype& V1) {
    return simdpp::move4_r<shift>(V1);
  }
};
template <unsigned shift>
struct lookup_move_r<shift, 64, 2> {
  typedef typename lookup_vec<64, 2>::type vectype;
  static inline auto move_r(const vectype& V1) {
    return simdpp::move2_r<shift>(V1);
  }
};

template <unsigned shift,
          unsigned simd_num_bits_per_element,
          unsigned simd_vector_length>

struct lookup_move_l {
  typedef
      typename lookup_vec<simd_num_bits_per_element, simd_vector_length>::type
          vectype;
  static inline auto move_l(const vectype& V1);
};

template <unsigned shift>
struct lookup_move_l<shift, 8, 16> {
  typedef typename lookup_vec<8, 16>::type vectype;
  static inline auto move_l(const vectype& V1) {
    return simdpp::move16_l<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_l<shift, 16, 8> {
  typedef typename lookup_vec<16, 8>::type vectype;
  static inline auto move_l(const vectype& V1) {
    return simdpp::move8_l<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_l<shift, 32, 4> {
  typedef typename lookup_vec<32, 4>::type vectype;
  static inline auto move_l(const vectype& V1) {
    return simdpp::move4_l<shift>(V1);
  }
};
template <unsigned shift>
struct lookup_move_l<shift, 64, 2> {
  typedef typename lookup_vec<64, 2>::type vectype;
  static inline auto move_l(const vectype& V1) {
    return simdpp::move2_l<shift>(V1);
  }
};

template <unsigned shift,
          unsigned simd_num_bits_per_element,
          unsigned simd_vector_length>
struct lookup_align {
  typedef
      typename lookup_vec<simd_num_bits_per_element, simd_vector_length>::type
          vectype;
  static inline auto align(const vectype& V1, const vectype& V2);
};

template <unsigned shift>
struct lookup_align<shift, 8, 16> {
  typedef typename lookup_vec<8, 16>::type vectype;
  static inline auto align(const vectype& V1, const vectype& V2) {
    return simdpp::align16<shift>(V1, V2);
  }
};

template <unsigned shift>
struct lookup_align<shift, 16, 8> {
  typedef typename lookup_vec<16, 8>::type vectype;
  static inline auto align(const vectype& V1, const vectype& V2) {
    return simdpp::align8<shift>(V1, V2);
  }
};

template <unsigned shift>
struct lookup_align<shift, 32, 4> {
  typedef typename lookup_vec<32, 4>::type vectype;
  static inline auto align(const vectype& V1, const vectype& V2) {
    return simdpp::align4<shift>(V1, V2);
  }
};

template <unsigned shift>
struct lookup_align<shift, 64, 2> {
  typedef typename lookup_vec<64, 2>::type vectype;
  static inline auto align(const vectype& V1, const vectype& V2) {
    return simdpp::align2<shift>(V1, V2);
  }
};

// 256 bit SIMD

template <unsigned shift>
struct lookup_align<shift, 8, 32> {
  typedef typename lookup_vec<8, 32>::type vectype;
  static inline auto align(const vectype& V1, const vectype& V2) {
    return simdpp::align16<shift>(V1, V2);
  }
};

template <unsigned shift>
struct lookup_align<shift, 16, 16> {
  typedef typename lookup_vec<16, 16>::type vectype;
  static inline auto align(const vectype& V1, const vectype& V2) {
    return simdpp::align8<shift>(V1, V2);
  }
};

template <unsigned shift>
struct lookup_align<shift, 32, 8> {
  typedef typename lookup_vec<32, 8>::type vectype;
  static inline auto align(const vectype& V1, const vectype& V2) {
    return simdpp::align4<shift>(V1, V2);
  }
};

template <unsigned shift>
struct lookup_align<shift, 64, 4> {
  typedef typename lookup_vec<64, 4>::type vectype;
  static inline auto align(const vectype& V1, const vectype& V2) {
    return simdpp::align2<shift>(V1, V2);
  }
};

/*
template <unsigned shift>
struct lookup_align<shift, 128, 2> {
  typedef typename lookup_vec<128, 2>::type vectype;
  static inline auto align(const vectype & V1, const vectype & V2) { return
simdpp::align2<shift>(V1,V2); }
};
*/

template <unsigned shift>
struct lookup_move_r<shift, 8, 32> {
  typedef typename lookup_vec<8, 32>::type vectype;
  static inline auto move_r(const vectype& V1) {
    return simdpp::move16_r<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_r<shift, 16, 16> {
  typedef typename lookup_vec<16, 16>::type vectype;
  static inline auto move_r(const vectype& V1) {
    return simdpp::move8_r<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_r<shift, 32, 8> {
  typedef typename lookup_vec<32, 8>::type vectype;
  static inline auto move_r(const vectype& V1) {
    return simdpp::move4_r<shift>(V1);
  }
};
template <unsigned shift>
struct lookup_move_r<shift, 64, 4> {
  typedef typename lookup_vec<64, 4>::type vectype;
  static inline auto move_r(const vectype& V1) {
    return simdpp::move2_r<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_l<shift, 8, 32> {
  typedef typename lookup_vec<8, 32>::type vectype;
  static inline auto move_l(const vectype& V1) {
    return simdpp::move16_l<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_l<shift, 16, 16> {
  typedef typename lookup_vec<16, 16>::type vectype;
  static inline auto move_l(const vectype& V1) {
    return simdpp::move8_l<shift>(V1);
  }
};

template <unsigned shift>
struct lookup_move_l<shift, 32, 8> {
  typedef typename lookup_vec<32, 8>::type vectype;
  static inline auto move_l(const vectype& V1) {
    return simdpp::move4_l<shift>(V1);
  }
};
template <unsigned shift>
struct lookup_move_l<shift, 64, 4> {
  typedef typename lookup_vec<64, 4>::type vectype;
  static inline auto move_l(const vectype& V1) {
    return simdpp::move2_l<shift>(V1);
  }
};

/*
template <unsigned shift>
struct lookup_move_l<shift, 128, 2> {
  typedef typename lookup_vec<128, 2>::type vectype;
  static inline auto move_l(const vectype & V1) { return
simdpp::move2_l<shift>(V1); }
};
*/

#endif  // LOOKUP_TEMPLATES_H
