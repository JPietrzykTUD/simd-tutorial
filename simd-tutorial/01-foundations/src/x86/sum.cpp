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
