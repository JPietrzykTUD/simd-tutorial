#include <cstdint>
#include <cstddef>
#include "tsl/tsl.hpp"

#include "utils.hpp"
#include "aggregation/sum_scalar.hpp"
#include "aggregation/sum_neon.hpp"
#include "aggregation/sum_tsl.hpp"
#include "aggregation/sum_tsl_runtime.hpp"


tsl::executor<tsl::runtime::cpu> tsl_exec{};
template<typename T>
static constexpr auto available_parallelism = tsl::runtime::cpu::available_parallelism<T>();



template<typename T>
void sum_test_type(size_t const element_count) {
  std::cout << "Executing sum for type " << tsl::type_name<T>() << std::endl;
  auto data = new T[element_count];
  fill<T>(data, element_count, 0, 100);
  T result_scalar, result_neon, result_tsl;
  T result_tsl_execs[available_parallelism<T>.size()];
  aggregate_sum_scalar<T>(&result_scalar, data, element_count);
  aggregate_sum_neon<T>(&result_neon, data, element_count);
  aggregate_sum_tsl<tsl::simd<T, tsl::neon>>(&result_tsl, data, element_count);

  auto run_all_available = [&]<size_t... Is>(std::index_sequence<Is...>) {
    (tsl_exec.submit<T, available_parallelism<T>[Is], aggregate_sum>(&result_tsl_execs[Is], data, element_count), ...);
    for (auto const& result_tsl_exec : result_tsl_execs) {
      verify(result_scalar == result_tsl_exec);
    }
    
  };
  run_all_available(std::make_index_sequence<available_parallelism<T>.size()>{});
  verify(result_scalar == result_neon);
  verify(result_scalar == result_tsl);
  delete[] data;
}

int main() {
  const size_t element_count = 1 << 26;
  sum_test_type<uint32_t>(element_count);
  sum_test_type<uint64_t>(element_count);
  return 0;
}
