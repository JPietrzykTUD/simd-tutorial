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
#include <immintrin.h>

#include "table.hpp"


#ifdef COMPILER_EXPLORER
#define FORCE_INLINE
#else
#define FORCE_INLINE __attribute__((always_inline)) inline
#endif

FORCE_INLINE void filter_eq_sum_nsm_avx2(
  uint32_t * __restrict__ dst, 
  table_nsm const & data,
  uint32_t const value
) {
  auto element_count = data.row_count;
  auto const rows_base = const_cast<int32_t const *>(reinterpret_cast<int32_t * const>(data.rows));

  /* Calculate pointers for SIMD processing and scalar remainder */
  const auto remainder = element_count & 0x7;
  element_count -= remainder;
  /* Initialize result and pointers for SIMD processing */
  __m256i result_vec = _mm256_setzero_si256();
  __m256i const pred_vec = _mm256_set1_epi32(value);
  __m256i const index_inc_vec = _mm256_set1_epi32(sizeof(table_nsm::row));
  __m256i const col0_inc_vec = _mm256_set1_epi32(offsetof(table_nsm::row, col0));
  __m256i const col1_inc_vec = _mm256_set1_epi32(offsetof(table_nsm::row, col1));
  __m256i index_vec = _mm256_set_epi32(
    7*sizeof(table_nsm::row), 6*sizeof(table_nsm::row), 5*sizeof(table_nsm::row), 4*sizeof(table_nsm::row),
    3*sizeof(table_nsm::row), 2*sizeof(table_nsm::row), 1*sizeof(table_nsm::row), 0
  );
  for (auto row_idx = 0; row_idx < element_count; row_idx += 8) {
    auto byte_index_row0_vec = _mm256_add_epi32(index_vec, col0_inc_vec);
    auto byte_index_row1_vec = _mm256_add_epi32(index_vec, col1_inc_vec);
    auto to_filter_vec = _mm256_i32gather_epi32(rows_base, byte_index_row0_vec, 1);
    auto to_sum_vec = _mm256_i32gather_epi32(rows_base, byte_index_row1_vec, 1);
    __m256i eq_mask = _mm256_cmpeq_epi32(to_filter_vec, pred_vec);
    __m256i to_sum_adder = _mm256_and_si256(eq_mask, to_sum_vec);
    result_vec = _mm256_add_epi32(result_vec, to_sum_adder);
    index_vec = _mm256_add_epi32(index_vec, index_inc_vec);
  }

  auto res1 = _mm256_srli_si256(result_vec, 8);
  result_vec = _mm256_add_epi32(result_vec, res1);
  auto res2 = _mm256_srli_si256(result_vec, 4);
  result_vec = _mm256_add_epi32(result_vec, res2);
  auto upper = _mm256_permute2f128_si256(result_vec, result_vec, 0x1);
  result_vec = _mm256_add_epi32(result_vec, upper);
  uint32_t result = _mm256_cvtsi256_si32(result_vec);
  
  /* Calculate pointers for remainder processing */
  auto remainder_rows = data.rows + element_count;
  /* Start remainder processing */
  for (auto row_idx = 0; row_idx < remainder; ++row_idx) {
    if (remainder_rows->col0 == value) {
      result += remainder_rows->col1;
    }
  }
  
  *dst = result;
}

