#include <cstdint>
#include <cstddef>
#include "tsl/tsl.hpp"

#include "utils.hpp"
#include "aggregation/sum_scalar.hpp"
#include "aggregation/sum_avx2.hpp"
#include "aggregation/sum_tsl.hpp"

template<typename T>
void sum_test_type(size_t const element_count) {
  auto data = new T[element_count];
  fill<T>(data, element_count, 0, 100);
  T result_scalar, result_avx2, result_tsl;
  aggregate_sum_scalar<T>(&result_scalar, data, element_count);
  aggregate_sum_avx2<T>(&result_avx2, data, element_count);
  aggregate_sum_tsl<T>(&result_tsl, data, element_count);
  verify(result_scalar == result_avx2);
  verify(result_scalar == result_tsl);
  delete[] data;
}

int main() {
  const size_t element_count = 1 << 26;
  sum_test_type<uint32_t>(element_count);
  sum_test_type<uint64_t>(element_count);
  return 0;
}
