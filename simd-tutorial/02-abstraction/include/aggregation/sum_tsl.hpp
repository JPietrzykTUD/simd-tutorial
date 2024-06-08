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
FORCE_INLINE void aggregate_tsl(typename SimdT::base_type * __restrict__ dst, typename SimdT::base_type const * __restrict__ src, size_t element_count) {
  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & (SimdT::vector_element_count() - 1);
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  auto result_vec = tsl::setzero<SimdT>();
  auto const * const src_simd_end = reinterpret_cast<SimdT::base_type const *>(src + element_count);
  /* Calculate pointers for remainder processing */
  auto src_remainder = src + element_count;
  auto const src_end = src_remainder + remainder;
  /* Start SIMD processing*/
  while (src != src_simd_end) {
    auto src_vec = tsl::loadu<SimdT>(src);
    result_vec = tsl::add<SimdT>(result_vec, src_vec);
    src += SimdT::vector_element_count();
  }
  auto result = tsl::hadd<SimdT>(result_vec);
  /* Start remainder processing */
  while (src_remainder != src_end) {
    result += *src++;
  }
  *dst = result;
}

