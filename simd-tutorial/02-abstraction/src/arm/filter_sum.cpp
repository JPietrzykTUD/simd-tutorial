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
#include "tsl/tsl.hpp"

#include "utils.hpp"
#include "filter_sum/filter_sum_scalar.hpp"
#include "filter_sum/filter_sum_neon.hpp"
#include "filter_sum/filter_sum_tsl.hpp"
#include "filter_sum/filter_sum_tsl_runtime.hpp"

tsl::executor<tsl::runtime::cpu> tsl_exec{};
template<typename T>
static constexpr auto available_parallelism = tsl::runtime::cpu::available_parallelism<T>();


template<typename T>
void filter_sum_test_type(size_t const element_count) {
  std::cout << "Executing sum for type " << tsl::type_name<T>() << std::endl;
  auto to_filter = new T[element_count];
  auto to_sum = new T[element_count];
  fill<T>(to_filter, element_count, 1, 20);
  fill<T>(to_sum, element_count, 0, 100);
  T result_scalar, result_neon, result_tsl;
  T result_tsl_execs[available_parallelism<T>.size()];
  filter_eq_sum_scalar<T>(&result_scalar, to_filter, 10, to_sum, element_count);
  filter_eq_sum_neon<T>(&result_neon, to_filter, 10, to_sum, element_count);
  filter_eq_sum_tsl<tsl::simd<T, tsl::neon>>(&result_tsl, to_filter, 10, to_sum, element_count);

  auto run_all_available = [&]<size_t... Is>(std::index_sequence<Is...>) {
    (tsl_exec.submit<T, available_parallelism<T>[Is], filter_eq_sum>(&result_tsl_execs[Is], to_filter, 10, to_sum, element_count), ...);
    for (auto const& result_tsl_exec : result_tsl_execs) {
      verify(result_scalar == result_tsl_exec);
    }
    
  };
  run_all_available(std::make_index_sequence<available_parallelism<T>.size()>{});

  verify(result_scalar == result_neon);
  verify(result_scalar == result_tsl);
  delete[] to_sum;
  delete[] to_filter;
}

int main() {
  const size_t element_count = 1 << 26;
  filter_sum_test_type<uint32_t>(element_count);
  filter_sum_test_type<uint64_t>(element_count);
  return 0;
}
