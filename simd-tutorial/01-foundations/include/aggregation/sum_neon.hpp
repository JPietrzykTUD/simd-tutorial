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
#include <neon.h>

#ifdef COMPILER_EXPLORER
#define FORCE_INLINE
#else
#define FORCE_INLINE __attribute__((always_inline)) inline
#endif


FORCE_INLINE void aggregate_sum_neon(uint32_t * __restrict__ dst, uint32_t const * __restrict__ src, size_t element_count) {
  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & 0x3;
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  uint32x4_t result_vec = vdupq_n_u32(0);
  uint32_t const * const src_simd_end = reinterpret_cast<uint32_t const *>(src + element_count);
  /* Calculate pointers for remainder processing */
  auto const src_end = src_simd_end + remainder;
  /* Start SIMD processing*/
  while (src != src_simd_end) {
    uint32x4_t src_vec = vld1q_u32(src);
    result_vec = vaddq_u32(result_vec, src_vec);
    src += 4;
  }
  uint32_t result = vaddvq_u32(result_vec);
  /* Start remainder processing */
  while (src != src_end) {
    result += *src++;
  }
  *dst = result;
}

