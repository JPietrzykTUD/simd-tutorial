/**********************************************************************************
  This file is part of the SIGMOD'24 tutorial: 
  SIMDified Data Processing-Foundations, Abstraction, and Advanced Techniques [1].
  Authors of the file: Johannes Pietrzyk, Dirk Habich.
  [1] https://doi.org/10.1145/3626246.3654694
  
  This work is licensed under the terms of the MIT license.  
  For a copy, see <https://opensource.org/licenses/MIT>.
 ***********************************************************************************/
#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>
#include <iostream>
#include "tsl/tsl.hpp"

#include "../preprocessor.hpp"
#include "table.hpp"

template<class SimdT, typename T1, typename T2, typename T3>
struct filter_eq_sum_dsm {
  static void apply(T1 dst, T2 data, T3 value) {
    std::cout << "  Using DSM with " << tsl::type_name<typename SimdT::target_extension>() << ": ";
    auto element_count = data.row_count;
    typename SimdT::base_type const * to_filter = data.col0();
    typename SimdT::base_type  const * to_sum = data.col1();
    /* Calculate pointers for SIMD processing and scalar remainder */
    const auto remainder = element_count & (SimdT::vector_element_count() - 1);
    element_count -= remainder;
    /* Initialize result and pointers for SIMD processing */
    auto result_vec = tsl::set_zero<SimdT>();
    auto pred_vec = tsl::set1<SimdT>(value);
    auto const to_filter_simd_end = to_filter + element_count;
    /* Calculate pointers for remainder processing */
    auto const to_filter_end = to_filter_simd_end + remainder;
    auto to_sum_remainder = to_sum + element_count;
    /* Start SIMD processing*/ 
    while (to_filter != to_filter_simd_end) {
      auto to_filter_vec = tsl::loadu<SimdT>(to_filter);
      auto to_sum_vec = tsl::loadu<SimdT>(to_sum);
      auto eq_mask = tsl::equal<SimdT>(to_filter_vec, pred_vec);
      result_vec = tsl::add<SimdT>(eq_mask, result_vec, to_sum_vec);
      to_filter += SimdT::vector_element_count();
      to_sum += SimdT::vector_element_count();
    }
    auto result = tsl::hadd<SimdT>(result_vec);
    /* Start remainder processing */
    while (to_filter != to_filter_end) {
      if (*to_filter == value) {
        result += *to_sum_remainder;
      }
      ++to_filter;
      ++to_sum_remainder;    
    }
    *dst = result;
  }
};

