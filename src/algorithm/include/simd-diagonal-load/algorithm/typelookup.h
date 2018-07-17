#ifndef TYPELOOKUP_H
#define TYPELOOKUP_H

#include <functional>
#include <array>
#include "lookup_templates.h"

template <unsigned simd_vector_length,
          unsigned num_loads,
          unsigned num_vertical_subdivisions>
constexpr unsigned num_blenders() {
  static_assert(simd_vector_length >= (num_loads * two_to_the_power_of<num_vertical_subdivisions>()));
  return simd_vector_length / (num_loads * two_to_the_power_of<num_vertical_subdivisions>());
}

template <typename integerT,
          unsigned simd_vector_length,
          unsigned num_loads,
          unsigned num_vertical_subdivisions>
class TypeLookup {
 public:
  typedef
      typename lookup_vec_from_integertype<integerT, simd_vector_length>::type
          vectype;

  typedef std::array<
      vectype,
      num_blenders<simd_vector_length, num_loads, num_vertical_subdivisions>()>
      blenders_t;
  typedef typename std::function<vectype()> get_next_column_func_type;
  typedef typename std::function<void(vectype)> use_diagonal_func_type;
};

#endif  // TYPELOOKUP_H
