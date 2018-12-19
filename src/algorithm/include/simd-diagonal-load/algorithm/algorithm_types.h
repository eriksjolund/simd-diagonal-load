#ifndef ALGORITHM_TYPES_H
#define ALGORITHM_TYPES_H

// num_loads, num_vertical_subdivisions, num_vertical_mixing
// store log2 value and should not be to big.
// std::numeric_limits<std::uint8_t>::max() is more than enough

typedef std::uint8_t num_loads_t;
typedef std::uint8_t num_vertical_subdivisions_t;
typedef std::uint8_t num_vertical_mixing_t;

#endif  // ALGORITHM_TYPES_H
