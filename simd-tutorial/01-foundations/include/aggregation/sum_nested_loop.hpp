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

#ifdef COMPILER_EXPLORER
#define FORCE_INLINE
#else
#define FORCE_INLINE __attribute__((always_inline)) inline
#endif

#ifndef VECTOR_COUNT
#define VECTOR_COUNT 4
#endif

FORCE_INLINE void aggregate_sum_nested_loop(uint32_t * __restrict__ dst, uint32_t const * __restrict__ src, size_t element_count) {
  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & (VECTOR_COUNT - 1);
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  uint32_t result_arr[VECTOR_COUNT] = {};
  uint32_t const * const src_simd_end = src + element_count;
  auto const src_end = src_simd_end + remainder;
  while (src != src_simd_end) {
    #pragma unroll
    for (auto i = 0u; i < VECTOR_COUNT; ++i) {
      result_arr[i] += src[i];
    }
    src += VECTOR_COUNT;
  }
  uint32_t result = 0;
  for (auto i = 0u; i < VECTOR_COUNT; ++i) {
    result += result_arr[i];
  }
  while (src != src_end) {
    result += *src++;
  }
  *dst = result;
}

