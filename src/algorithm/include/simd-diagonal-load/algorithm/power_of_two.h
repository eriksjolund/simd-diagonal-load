#ifndef POWER_OF_TWO_H
#define POWER_OF_TWO_H

constexpr bool is_power_of_two(int num) {
  return num && ((num & (num - 1)) == 0);
}

template <unsigned num>
constexpr unsigned two_to_the_power_of() {
  if constexpr (num > 0) {
    return two_to_the_power_of<num - 1>() * 2;
  } else {
    return 1;
  }
}

constexpr unsigned logtwo(unsigned n) {
  return n == 1 ? 0 : 1 + logtwo(n / 2);
}

#endif  // POWER_OF_TWO_H
