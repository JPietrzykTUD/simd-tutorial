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
#include "nsm_dsm/table.hpp"
#include "nsm_dsm/filter_sum_dsm_nsm_scalar.hpp"
#include "nsm_dsm/filter_sum_dsm_avx2.hpp"
#include "nsm_dsm/filter_sum_dsm_tsl_runtime.hpp"
#include "nsm_dsm/filter_sum_nsm_avx2.hpp"
#include "nsm_dsm/filter_sum_nsm_tsl_runtime.hpp"

tsl::executor<tsl::runtime::cpu> tsl_exec{};
template<typename T>
static constexpr auto available_parallelism = tsl::runtime::cpu::available_parallelism<T>();

template<typename T>
void nsm_dsm_test_type(size_t const element_count) {
  std::cout << "Executing sum for type " << tsl::type_name<T>() << std::endl;

  table_dsm<T> tab_dsm{element_count};
  table_nsm<T> tab_nsm{element_count};
  fill<T>(tab_dsm.col0(), element_count, 1, 20);
  fill<T>(tab_dsm.col1(), element_count, 0, 100);
  
  transform(tab_dsm, tab_nsm);

  T result_dsm_scalar, result_nsm_scalar;
  T result_dsm_avx2, result_nsm_avx2;
  T result_tsl_execs_dsm[available_parallelism<T>.size()];
  T result_tsl_execs_nsm[available_parallelism<T>.size()];

  filter_eq_sum_dsm_scalar<T>(&result_dsm_scalar, tab_dsm, 10);
  filter_eq_sum_nsm_scalar<T>(&result_nsm_scalar, tab_nsm, 10);
  verify(result_dsm_scalar == result_nsm_scalar);

  filter_eq_sum_dsm_avx2<T>(&result_dsm_avx2, tab_dsm, 10);
  verify(result_dsm_scalar == result_dsm_avx2);

  filter_eq_sum_nsm_avx2<T>(&result_nsm_avx2, tab_nsm, 10);
  verify(result_nsm_scalar == result_nsm_avx2);

  auto run_all_available_dsm = [&]<size_t... Is>(std::index_sequence<Is...>) {
    ([&]() {
      tsl_exec.submit<T, available_parallelism<T>[Is], filter_eq_sum_dsm>(&result_tsl_execs_dsm[Is], tab_dsm, 10);
      verify(result_dsm_scalar == result_tsl_execs_dsm[Is]);
    }(),...);
  };
  run_all_available_dsm(std::make_index_sequence<available_parallelism<T>.size()>{});
  
  auto run_all_available_nsm = [&]<size_t... Is>(std::index_sequence<Is...>) {
    ([&]() {
      tsl_exec.submit<T, available_parallelism<T>[Is], filter_eq_sum_nsm>(&result_tsl_execs_nsm[Is], tab_nsm, 10);
      verify(result_nsm_scalar == result_tsl_execs_nsm[Is]);
    }(),...);
  };
  run_all_available_nsm(std::make_index_sequence<available_parallelism<T>.size()>{});
}


int main() {
  const size_t element_count = 1 << 26;
  
  nsm_dsm_test_type<uint32_t>(element_count);
  nsm_dsm_test_type<uint64_t>(element_count);
  return 0;
}
