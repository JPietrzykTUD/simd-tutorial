#include <cstdint>
#include <cstddef>

#include "utils.hpp"
#include "aggregation/sum_scalar.hpp"
#include "aggregation/sum_nested_loop.hpp"
#include "aggregation/sum_neon.hpp"

int main() {
  uint32_t result_scalar, result_nested_loop, result_neon;
  const size_t element_count = 1 << 26;
  auto data = new uint32_t[element_count];
  fill(data, element_count, 0, 100);
  aggregate_sum_scalar(&result_scalar, data, element_count);
  aggregate_sum_nested_loop(&result_nested_loop, data, element_count);
  aggregate_sum_neon(&result_neon, data, element_count);
  verify(result_scalar == result_nested_loop);
  verify(result_scalar == result_neon);
  delete[] data;
  return 0;
}
