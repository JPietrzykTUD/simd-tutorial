/**********************************************************************************
  This file is part of the SIGMOD'24 tutorial: 
  SIMDified Data Processing-Foundations, Abstraction, and Advanced Techniques [1].
  Authors of the file: Johannes Pietrzyk, Dirk Habich.
  [1] https://doi.org/10.1145/3626246.3654694
  
  This work is licensed under the terms of the MIT license.  
  For a copy, see <https://opensource.org/licenses/MIT>.
 ***********************************************************************************/
#include <cstdint>
#include <cstddef>

#include "utils.hpp"
#include "filter_sum/filter_sum_scalar.hpp"
#include "filter_sum/filter_sum_avx2.hpp"

int main() {
  uint32_t result_scalar, result_avx2;
  const size_t element_count = 1 << 26;
  auto to_filter = new uint32_t[element_count];
  auto to_sum = new uint32_t[element_count];

  fill(to_filter, element_count, 1, 20);
  fill(to_sum, element_count, 0, 100);
  filter_eq_sum_scalar(&result_scalar, to_filter, 10, to_sum, element_count);
  filter_eq_sum_avx2(&result_avx2, to_filter, 10, to_sum, element_count);
  
  verify(result_scalar == result_avx2);
  delete[] to_sum;
  delete[] to_filter;
  return 0;
}
