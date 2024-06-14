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
#include "aggregation/sum_scalar.hpp"
#include "aggregation/sum_nested_loop.hpp"
#include "aggregation/sum_avx2.hpp"

int main() {
  uint32_t result_scalar, result_nested_loop, result_avx2;
  const size_t element_count = 1 << 26;
  auto data = new uint32_t[element_count];
  fill(data, element_count, 0, 100);
  aggregate_sum_scalar(&result_scalar, data, element_count);
  aggregate_sum_nested_loop(&result_nested_loop, data, element_count);
  aggregate_sum_avx2(&result_avx2, data, element_count);
  verify(result_scalar == result_nested_loop);
  verify(result_scalar == result_avx2);
  delete[] data;
  return 0;
}
