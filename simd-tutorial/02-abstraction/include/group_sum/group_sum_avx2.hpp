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
#include <bit>
#include <immintrin.h>

#include "../preprocessor.hpp"
#include "map.hpp"


template<typename T>
auto group_sum_avx2(
  simple_map_soa<T> & dst,
  T const * __restrict__ group_keys,
  T const * __restrict__ group_values,
  size_t element_count
) -> size_t;


template<>
auto group_sum_avx2(
  simple_map_soa<uint32_t> & dst,
  uint32_t const * __restrict__ group_keys,
  uint32_t const * __restrict__ group_values,
  size_t element_count
) -> size_t {
  size_t const map_element_count = dst.entry_count;
  size_t groups_count = 0;
  // map to store (key -> idx) pairs
  // idx is used to find key and value in dst (thus, the result is densely packed)
  simple_map_soa group_state(map_element_count, dst.empty_bucket_value);
  auto map_keys = group_state.keys();
  auto map_values = group_state.values();
  auto dst_keys = dst.keys();
  auto dst_values = dst.values();

  auto const end = group_keys + element_count;
  auto const empty_bucket_vec = _mm256_set1_epi32(dst.empty_bucket_value);
  auto const all_false_mask = 0;

  while (group_keys != end) {
    auto const key = *group_keys;
    auto const key_vec = _mm256_set1_epi32(key);
    auto const value = *group_values;
    // knuths multiplicative hash method
    auto pos_hint = static_cast<uint64_t>(key)*2654435761ull;
    while (true) {
      pos_hint = pos_hint % map_element_count;
      auto map_keys_vec = _mm256_loadu_si256(reinterpret_cast<__m256i const *>(map_keys + pos_hint));
      auto const key_found_vec = _mm256_cmpeq_epi32(map_keys_vec, key_vec);
      auto const key_found_mask = _mm256_movemask_ps(_mm256_castsi256_ps(key_found_vec));
      if (key_found_mask != all_false_mask) {
        auto const found_position = std::countr_zero<uint32_t>(key_found_mask);
        dst_values[*(map_values + pos_hint + found_position)] += value;
        break;
      }
      auto const empty_bucket_found_vec = _mm256_cmpeq_epi32(map_keys_vec, empty_bucket_vec);
      auto const empty_bucket_found_mask = _mm256_movemask_ps(_mm256_castsi256_ps(empty_bucket_found_vec));
      if (empty_bucket_found_mask != all_false_mask) {
        auto const empty_bucket_position = std::countr_zero<uint32_t>(empty_bucket_found_mask);
        map_keys[pos_hint + empty_bucket_position] = key;
        map_values[pos_hint + empty_bucket_position] = groups_count;
        dst_keys[groups_count] = key;
        dst_values[groups_count] = value;
        ++groups_count;
        break;
      }
      pos_hint += 8;
    }
    ++group_keys;
    ++group_values;
  }
  return groups_count;
}

template<>
auto group_sum_avx2(
  simple_map_soa<uint64_t> & dst,
  uint64_t const * __restrict__ group_keys,
  uint64_t const * __restrict__ group_values,
  size_t element_count
) -> size_t {
  size_t const map_element_count = dst.entry_count;
  size_t groups_count = 0;
  // map to store (key -> idx) pairs
  // idx is used to find key and value in dst (thus, the result is densely packed)
  simple_map_soa group_state(map_element_count, dst.empty_bucket_value);
  auto map_keys = group_state.keys();
  auto map_values = group_state.values();

  auto dst_keys = dst.keys();
  auto dst_values = dst.values();

  auto const end = group_keys + element_count;
  auto const empty_bucket_vec = _mm256_set1_epi64x(dst.empty_bucket_value);
  auto const all_false_mask = 0;

  while (group_keys != end) {
    auto const key = *group_keys;
    auto const key_vec = _mm256_set1_epi64x(key);
    auto const value = *group_values;
    // knuths multiplicative hash method
    auto pos_hint = static_cast<uint64_t>(key)*2654435761ull;
    while (true) {
      pos_hint = pos_hint % map_element_count;
      auto map_keys_vec = _mm256_loadu_si256(reinterpret_cast<__m256i const *>(map_keys + pos_hint));
      auto const key_found_vec = _mm256_cmpeq_epi64(map_keys_vec, key_vec);
      auto const key_found_mask = _mm256_movemask_pd(_mm256_castsi256_pd(key_found_vec));
      if (key_found_mask != all_false_mask) {
        auto const found_position = std::countr_zero<uint64_t>(key_found_mask);
        dst_values[*(map_values + pos_hint + found_position)] += value;
        break;
      }
      auto const empty_bucket_found_vec = _mm256_cmpeq_epi64(map_keys_vec, empty_bucket_vec);
      auto const empty_bucket_found_mask = _mm256_movemask_pd(_mm256_castsi256_pd(empty_bucket_found_vec));
      if (empty_bucket_found_mask != all_false_mask) {
        auto const empty_bucket_position = std::countr_zero<uint64_t>(empty_bucket_found_mask);
        map_keys[pos_hint + empty_bucket_position] = key;
        map_values[pos_hint + empty_bucket_position] = groups_count;
        dst_keys[groups_count] = key;
        dst_values[groups_count] = value;
        ++groups_count;
        break;
      }
      pos_hint += 4;
    }
    ++group_keys;
    ++group_values;
  }
  return groups_count;
}