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
void filter_eq_sum_avx2(T * dst, T const * to_filter, T const value, T const * to_sum, size_t element_count);

template<>
FORCE_INLINE void filter_eq_sum_avx2(
  uint32_t * __restrict__ dst, 
  uint32_t const * __restrict__ to_filter, 
  uint32_t const value,
  uint32_t const * __restrict__ to_sum,
  size_t element_count
) {
  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & 0x7;
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  __m256i result_vec = _mm256_setzero_si256();
  __m256i const pred_vec = _mm256_set1_epi32(value);
  __m256i const * const to_filter_simd_end = reinterpret_cast<__m256i const *>(to_filter + element_count);
  __m256i const * to_filter_simd_current = reinterpret_cast<__m256i const *>(to_filter);
  __m256i const * to_sum_simd_current = reinterpret_cast<__m256i const *>(to_sum);
  /* Calculate pointers for remainder processing */
  auto to_filter_remainder = to_filter + element_count;
  auto const to_filter_src_end = to_filter_remainder + remainder;
  auto to_sum_remainder = to_sum + element_count;
  /* Start SIMD processing*/ 
  while (to_filter_simd_current != to_filter_simd_end) {
    __m256i to_filter_vec = _mm256_loadu_si256(to_filter_simd_current);
    __m256i to_sum_vec = _mm256_loadu_si256(to_sum_simd_current);
    __m256i eq_mask = _mm256_cmpeq_epi32(to_filter_vec, pred_vec);
    __m256i to_sum_adder = _mm256_and_si256(eq_mask, to_sum_vec);
    result_vec = _mm256_add_epi32(result_vec, to_sum_adder);
    ++to_filter_simd_current;
    ++to_sum_simd_current;
  }

  auto res1 = _mm256_srli_si256(result_vec, 8);
  result_vec = _mm256_add_epi32(result_vec, res1);
  auto res2 = _mm256_srli_si256(result_vec, 4);
  result_vec = _mm256_add_epi32(result_vec, res2);
  auto upper = _mm256_permute2f128_si256(result_vec, result_vec, 0x1);
  result_vec = _mm256_add_epi32(result_vec, upper);
  uint32_t result = _mm256_cvtsi256_si32(result_vec);
  /* Start remainder processing */
  while (to_filter_remainder != to_filter_src_end) {
    if (*to_filter_remainder == value) {
      result += *to_sum_remainder;
    }
    ++to_filter_remainder;
    ++to_sum_remainder;    
  }
  *dst = result;
}

template<>
FORCE_INLINE void filter_eq_sum_avx2(
  uint64_t * __restrict__ dst, 
  uint64_t const * __restrict__ to_filter, 
  uint64_t const value,
  uint64_t const * __restrict__ to_sum,
  size_t element_count
) {
  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & 0x3;
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  __m256i result_vec = _mm256_setzero_si256();
  __m256i const pred_vec = _mm256_set1_epi64x(value);
  __m256i const * const to_filter_simd_end = reinterpret_cast<__m256i const *>(to_filter + element_count);
  __m256i const * to_filter_simd_current = reinterpret_cast<__m256i const *>(to_filter);
  __m256i const * to_sum_simd_current = reinterpret_cast<__m256i const *>(to_sum);
  /* Calculate pointers for remainder processing */
  auto to_filter_remainder = to_filter + element_count;
  auto const to_filter_src_end = to_filter_remainder + remainder;
  auto to_sum_remainder = to_sum + element_count;
  /* Start SIMD processing*/ 
  while (to_filter_simd_current != to_filter_simd_end) {
    __m256i to_filter_vec = _mm256_loadu_si256(to_filter_simd_current);
    __m256i to_sum_vec = _mm256_loadu_si256(to_sum_simd_current);
    __m256i eq_mask = _mm256_cmpeq_epi64(to_filter_vec, pred_vec);
    __m256i to_sum_adder = _mm256_and_si256(eq_mask, to_sum_vec);
    result_vec = _mm256_add_epi64(result_vec, to_sum_adder);
    ++to_filter_simd_current;
    ++to_sum_simd_current;
  }

  auto res1 = _mm256_srli_si256(result_vec, 8);
  result_vec = _mm256_add_epi64(result_vec, res1);
  auto upper = _mm256_permute2f128_si256(result_vec, result_vec, 0x1);
  result_vec = _mm256_add_epi64(result_vec, upper);
  uint64_t result = _mm256_extract_epi64(result_vec, 0);
  /* Start remainder processing */
  while (to_filter_remainder != to_filter_src_end) {
    if (*to_filter_remainder == value) {
      result += *to_sum_remainder;
    }
    ++to_filter_remainder;
    ++to_sum_remainder;    
  }
  *dst = result;
}