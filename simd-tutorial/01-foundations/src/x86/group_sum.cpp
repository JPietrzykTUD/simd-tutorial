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
#include "group_sum/group_sum_scalar.hpp"
#include "group_sum/group_sum_avx2.hpp"

int main() {
  uint32_t result_scalar, result_avx2;
  const size_t element_count = 1 << 26;
  auto to_group = new uint32_t[element_count];
  auto to_sum = new uint32_t[element_count];

  auto const group_count = 100;
  
  fill(to_group, element_count, 1, group_count);
  fill(to_sum, element_count, 0, 100);
  simple_map_soa dst_result_scalar(group_count);
  simple_map_soa dst_result_avx2(group_count);

  group_sum_scalar(dst_result_scalar, to_group, to_sum, element_count);
  group_sum_avx2(dst_result_avx2, to_group, to_sum, element_count);

  verify(dst_result_scalar == dst_result_avx2);

  delete[] to_sum;
  delete[] to_group;
  return 0;
}
