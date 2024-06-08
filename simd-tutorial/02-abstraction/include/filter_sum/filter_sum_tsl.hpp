/*******************************************************************************************************************************
  This file is part of the SIGMOD'24 tutorial: SIMDified Data Processing-Foundations, Abstraction, and Advanced Techniques [1].
  Authors of the file: Johannes Pietrzyk, Dirk Habich.
  [1] https://doi.org/10.1145/3626246.3654694
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************************************************************/
#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>
#include "tsl/tsl.hpp"


#ifdef COMPILER_EXPLORER
#undef FORCE_INLINE
#define FORCE_INLINE
#endif

template<class SimdT>
FORCE_INLINE void filter_eq_sum_tsl(
  typename SimdT::base_type * __restrict__ dst, 
  typename SimdT::base_type const * __restrict__ to_filter, 
  typename SimdT::base_type const value,
  typename SimdT::base_type const * __restrict__ to_sum,
  size_t element_count
) {
  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & (SimdT::vector_element_count() - 1);
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  auto result_vec = tsl::setzero<SimdT>();
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

