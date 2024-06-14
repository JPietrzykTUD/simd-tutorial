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
#include "group_sum/group_sum_scalar.hpp"
#include "group_sum/group_sum_avx2.hpp"
#include "group_sum/group_sum_tsl.hpp"
#include "group_sum/group_sum_tsl_runtime.hpp"

tsl::executor<tsl::runtime::cpu> tsl_exec{};
template<typename T>
static constexpr auto available_parallelism = tsl::runtime::cpu::available_parallelism<T>();


template<typename T>
void group_sum_test_type(size_t const element_count) {
  std::cout << "Executing sum for type " << tsl::type_name<T>() << std::endl;
  auto to_group = new T[element_count];
  auto to_sum = new T[element_count];
  auto const group_count = 100;
  fill<T>(to_group, element_count, 1, group_count);
  fill<T>(to_sum, element_count, 0, 100);
  simple_map_soa<T> result_scalar(group_count), result_avx2(group_count), result_tsl(group_count);
  std::vector<simple_map_soa<T>> result_tsl_execs;
  for (auto i = 0; i < available_parallelism<T>.size(); ++i) {
    result_tsl_execs.emplace_back(group_count);
  }
  group_sum_scalar<T>(result_scalar, to_group, to_sum, element_count);
  group_sum_avx2<T>(result_avx2, to_group, to_sum, element_count);
  group_sum_tsl<tsl::simd<T, tsl::avx2>>(result_tsl, to_group, to_sum, element_count);

  auto run_all_available = [&]<size_t... Is>(std::index_sequence<Is...>) {
    (tsl_exec.submit<T, available_parallelism<T>[Is], group_sum>(result_tsl_execs[Is], to_group, to_sum, element_count), ...);
    for (auto const& result_tsl_exec : result_tsl_execs) {
      verify(result_scalar == result_tsl_exec);
    }
    
  };
  run_all_available(std::make_index_sequence<available_parallelism<T>.size()>{});

  verify(result_scalar == result_avx2);
  verify(result_scalar == result_tsl);
  delete[] to_sum;
  delete[] to_group;
}

int main() {
  const size_t element_count = 1 << 26;
  group_sum_test_type<uint32_t>(element_count);
  group_sum_test_type<uint64_t>(element_count);
  return 0;
}
