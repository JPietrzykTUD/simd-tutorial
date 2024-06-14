/**********************************************************************************
  This file is part of the SIGMOD'24 tutorial: 
  SIMDified Data Processing-Foundations, Abstraction, and Advanced Techniques [1].
  Authors of the file: Johannes Pietrzyk, Dirk Habich.
  [1] https://doi.org/10.1145/3626246.3654694
  
  This work is licensed under the terms of the MIT license.  
  For a copy, see <https://opensource.org/licenses/MIT>.
 ***********************************************************************************/
#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>

#include "table.hpp"

template<typename T>
void filter_eq_sum_dsm_scalar(
  T * __restrict__ dst, 
  table_dsm<T> const & data,
  T const value
) {
  T result = 0;
  auto element_count = data.row_count;
  T const * to_filter = data.col0();
  T const * to_sum = data.col1();

  for (auto i = 0ull; i < element_count; ++i) {
    if (to_filter[i] == value) {
      result += to_sum[i];
    }
  }
  *dst = result;
}
