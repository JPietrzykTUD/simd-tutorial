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
#include <chrono>
#include <tuple>
#include <iostream>
#include <iomanip>
#include "tsl/tsl.hpp"

#include "utils.hpp"
#include "nsm_dsm/table.hpp"
#include "nsm_dsm/filter_sum_dsm_scalar.hpp"
#include "nsm_dsm/filter_sum_dsm_tsl.hpp"
#include "nsm_dsm/filter_sum_nsm_tsl.hpp"
#include "nsm_dsm/filter_sum_nsm_bsa_tsl.hpp"

tsl::executor<tsl::runtime::cpu> tsl_exec{};
template<typename T>
static constexpr auto available_parallelism = tsl::runtime::cpu::available_parallelism<T>();

template<typename T>
void nsm_dsm_test_type(size_t const element_count) {

  table_dsm<T> tab_dsm{element_count};
  table_nsm<T> tab_nsm{element_count};
  fill<T>(tab_dsm.col0(), element_count, 1, 20);
  fill<T>(tab_dsm.col1(), element_count, 0, 100);
  
  transform(tab_dsm, tab_nsm);

  T result_dsm_scalar;

  filter_eq_sum_dsm_scalar<T>(&result_dsm_scalar, tab_dsm, 10);

  [&]<size_t... Is>(std::index_sequence<Is...>) {
    ([&]() {
      T result;
      auto bench_result = tsl_exec.submit<T, available_parallelism<T>[Is], filter_eq_sum_dsm>(&result, tab_dsm, 10);
      verify(result_dsm_scalar == result)
      std::cout << "DSM\t1\t" << std::get<1>(bench_result) << "\t" << std::get<2>(bench_result) << "\t" << std::get<0>(bench_result) << " us" << std::endl;
    }(),...);
  }(std::make_index_sequence<available_parallelism<T>.size()>{});
  [&]<size_t... Is>(std::index_sequence<Is...>) {
    ([&]() {
      T result;
      auto bench_result = tsl_exec.submit<T, available_parallelism<T>[Is], filter_eq_sum_nsm>(&result, tab_nsm, 10);
      verify(result_dsm_scalar == result);
      std::cout << "NSM\t1\t" << std::get<1>(bench_result) << "\t" << std::get<2>(bench_result) << "\t" << std::get<0>(bench_result) << " us" << std::endl;
    }(),...);
  }(std::make_index_sequence<available_parallelism<T>.size()>{});
  
  for (auto block_size = 128; block_size <= 16384; block_size *= 2) {
    [&]<size_t... Is>(std::index_sequence<Is...>) {
      ([&]() {
        T result;
        auto bench_result = tsl_exec.submit<T, available_parallelism<T>[Is], filter_eq_sum_nsm_bsa>(&result, tab_nsm, 10, block_size);
        verify(result_dsm_scalar == result);
        std::cout << "NSM - BSA\t" << block_size << "\t" << std::get<1>(bench_result) << "\t" << std::get<2>(bench_result) << "\t" << std::get<0>(bench_result) << " us" << std::endl;
      }(),...);
    }(std::make_index_sequence<available_parallelism<T>.size()>{});
  }
  
}

int main() {
  const size_t element_count = 1 << 26;
  
  nsm_dsm_test_type<uint32_t>(element_count);
  nsm_dsm_test_type<uint64_t>(element_count);
  return 0;
}
