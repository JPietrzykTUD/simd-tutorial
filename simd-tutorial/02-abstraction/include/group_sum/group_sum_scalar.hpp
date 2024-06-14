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

#include "map.hpp"

template<typename T>
auto group_sum_scalar(
  simple_map_soa<T> & dst,
  T const * __restrict__ group_keys,
  T const * __restrict__ group_values,
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
  auto const all_false_mask = 0;

  while (group_keys != end) {
    auto const key = *group_keys;
    auto const value = *group_values;
    // knuths multiplicative hash method
    auto pos_hint = static_cast<uint64_t>(key)*2654435761ull;
    while (true) {
      pos_hint = pos_hint % map_element_count;
      if (map_keys[pos_hint] == key) {
        dst_values[*(map_values + pos_hint)] += value;
        break;
      }
      if (map_keys[pos_hint] == dst.empty_bucket_value) {
        map_keys[pos_hint] = key;
        map_values[pos_hint] = groups_count;
        dst_keys[groups_count] = key;
        dst_values[groups_count] = value;
        ++groups_count;
        break;
      }
      ++pos_hint;
    }
    ++group_keys;
    ++group_values;
  }
  return groups_count;
}