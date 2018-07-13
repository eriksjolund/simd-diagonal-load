#!/bin/sh

# The clang-format-6.0 command makes use of the configuration file 
# simd-diagonal-load/src/.clang-format

files="./algorithm/include/simd-diagonal-load/algorithm/check_template_parameters.h
./algorithm/include/simd-diagonal-load/algorithm/create_masks.h
./algorithm/include/simd-diagonal-load/algorithm/every_other.h
./algorithm/include/simd-diagonal-load/algorithm/lookup_templates.h
./algorithm/include/simd-diagonal-load/algorithm/power_of_two.h
./algorithm/include/simd-diagonal-load/algorithm/simd_diagonal_load.h
./algorithm/include/simd-diagonal-load/algorithm/typelookup.h
./algorithm/include/simd-diagonal-load/algorithm/use_blend_masks.h
./array-helper/include/simd-diagonal-load/array-helper/array_helper.h
./array-helper/include/simd-diagonal-load/array-helper/extract_helper.h
"

for i in $files; do 
  if [ ! -w $i ]; then
     echo "file $i does not exist"
     exit 1
  fi
  clang-format-6.0 -i $i
done
