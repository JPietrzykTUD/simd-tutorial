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
#include <immintrin.h>

#include "../preprocessor.hpp"


template<typename T>
void aggregate_sum_avx2(T * dst, T const * src, size_t element_count);

template<>
FORCE_INLINE void aggregate_sum_avx2(uint32_t * __restrict__ dst, uint32_t const * __restrict__ src, size_t element_count) {
  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & 0x7;
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  __m256i result_vec = _mm256_setzero_si256();
  __m256i const * const src_simd_end = reinterpret_cast<__m256i const *>(src + element_count);
  __m256i const * src_simd_current = reinterpret_cast<__m256i const *>(src);
  /* Calculate pointers for remainder processing */
  auto src_remainder = src + element_count;
  auto const src_end = src_remainder + remainder;
  /* Start SIMD processing*/ 
  while (src_simd_current != src_simd_end) {
    __m256i src_vec = _mm256_loadu_si256(src_simd_current);
    result_vec = _mm256_add_epi32(result_vec, src_vec);
    ++src_simd_current;
  }

  auto res1 = _mm256_srli_si256(result_vec, 8);
  result_vec = _mm256_add_epi32(result_vec, res1);
  auto res2 = _mm256_srli_si256(result_vec, 4);
  result_vec = _mm256_add_epi32(result_vec, res2);
  auto upper = _mm256_permute2f128_si256(result_vec, result_vec, 0x1);
  result_vec = _mm256_add_epi32(result_vec, upper);
  uint32_t result = _mm256_cvtsi256_si32(result_vec);
  /* Start remainder processing */
  while (src_remainder != src_end) {
    result += *src++;
  }
  *dst = result;
}

template<>
FORCE_INLINE void aggregate_sum_avx2(uint64_t * __restrict__ dst, uint64_t const * __restrict__ src, size_t element_count) {
  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & 0x3;
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  __m256i result_vec = _mm256_setzero_si256();
  __m256i const * const src_simd_end = reinterpret_cast<__m256i const *>(src + element_count);
  __m256i const * src_simd_current = reinterpret_cast<__m256i const *>(src);
  /* Calculate pointers for remainder processing */
  auto src_remainder = src + element_count;
  auto const src_end = src_remainder + remainder;
  /* Start SIMD processing*/ 
  while (src_simd_current != src_simd_end) {
    __m256i src_vec = _mm256_loadu_si256(src_simd_current);
    result_vec = _mm256_add_epi64(result_vec, src_vec);
    ++src_simd_current;
  }

  auto res1 = _mm256_srli_si256(result_vec, 8);
  result_vec = _mm256_add_epi64(result_vec, res1);
  auto upper = _mm256_permute2f128_si256(result_vec, result_vec, 0x1);
  result_vec = _mm256_add_epi64(result_vec, upper);
  uint64_t result = _mm256_extract_epi64(result_vec, 0);
  /* Start remainder processing */
  while (src_remainder != src_end) {
    result += *src++;
  }
  *dst = result;
}