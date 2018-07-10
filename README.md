# simd-diagonal-load

simd-diagonal-load is a C++ library that efficiently loads diagonals into SIMD vectors,
when the data is stored in such a way that only columns can be loaded.

The problem is described in 

https://stackoverflow.com/questions/15198011/how-to-load-a-sliding-diagonal-vector-from-data-stored-column-wise-withsse

A naive implementation could be formulated as:

```c++
int width=1000000; // a big number
uint8_t matrix[width][16];
fill_matrix_with_interesting_values(&matrix);

for (int i=0; i < width - 16; ++i) {
  uint8_t diagonal_vector[16];
  for (int j=0; j<16; ++j) {
    diagonal_vector[j] = matrix[i+j][j];
  }
  do_something(&diagonal_vector);
}
```
