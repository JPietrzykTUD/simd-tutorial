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
#include <bit>
#include <array>
#include <neon.h>

#include "../preprocessor.hpp"
#include "map.hpp"



auto group_sum_avx2(
  simple_map_soa & dst,
  uint32_t const * __restrict__ group_keys,
  uint32_t const * __restrict__ group_values,
  size_t element_count
) -> size_t {
  size_t const map_element_count = dst.entry_count;
  size_t groups_count = 0;
  // map to store (key -> idx) pairs
  // idx is used to find key and value in dst (thus, the result is densely packed)
  simple_map_soa group_state(map_element_count, dst.empty_bucket_value);
  auto * const map_keys = group_state.keys;
  auto * const map_values = group_state.values;

  auto const end = group_keys + element_count;
  auto const empty_bucket_vec = vdupq_n_u32(dst.empty_bucket_value);
  auto const all_false_mask = 0;
  // create a vector with values ~1, ~2, ~3, ~4
  std::array<uint32_t, 4> mask_values{{ 0xfffffffe, 0xfffffffd, 0xfffffffc, 0xfffffffb }};
  auto const mask_value_vec = vld1q_u32(mask_values.data());

  while (group_keys != end) {
    auto const key = *group_keys;
    auto const key_vec = vdupq_n_u32(key);
    auto const value = *group_values;
    // knuths multiplicative hash method
    auto pos_hint = static_cast<uint64_t>(key)*2654435761ull;
    while (true) {
      pos_hint = pos_hint % map_element_count;
      auto map_keys_vec = vld1q_u32(map_keys + pos_hint);
      auto const key_found_vec = vceqq_u32(map_keys_vec, key_vec);
      auto const key_found_xor_mask_value_vec = veorq_u32(key_found_vec, mask_value_vec);
      auto const key_found_min_mask_value = vminvq_u32(key_found_xor_mask_value_vec);
      if (key_found_min_mask_value <= 4) {
        dst.values[*(map_values + pos_hint + key_found_min_mask_value - 1)] += value;
        break;
      }
      auto const empty_bucket_found_vec = vceqq_u32(map_keys_vec, empty_bucket_vec);
      auto const empty_bucket_xor_mask_value_vec = veorq_u32(empty_bucket_found_vec, mask_value_vec);
      auto const empty_bucket_min_mask_value = vminvq_u32(empty_bucket_xor_mask_value_vec);
      if (empty_bucket_min_mask_value <= 4) {
        auto pos = pos_hint + empty_bucket_min_mask_value - 1;
        map_keys[pos] = key;
        map_values[pos] = groups_count;
        dst.keys[groups_count] = key;
        dst.values[groups_count] = value;
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