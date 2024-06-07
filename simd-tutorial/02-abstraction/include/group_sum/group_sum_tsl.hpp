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

#include <algorithm>
#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>
#include <algorithm>

#include "tslintrin.hpp"

template<class SimdT>
struct simple_map_soa {
  using T = typename SimdT::base_type;
  T * const keys;
  T * const values;
  size_t const entry_count;
  T const empty_bucket_value;
  T const empty_value;
  explicit simple_map_soa(size_t entry_count, T ebv = 0, T evv = 0)
  : keys(new T[entry_count]),
    values(new T[entry_count]),
    entry_count(entry_count),
    empty_bucket_value(ebv),
    empty_value(evv) {
      std::fill(keys, keys + entry_count, empty_bucket_value);
      std::fill(values, values + entry_count, empty_value);
  }
  simple_map_soa(simple_map_soa const & f) = default;
  simple_map_soa(simple_map_soa && f) = default;
  ~simple_map_soa() {
    delete[] keys;
    delete[] values;
  }
};

template<class SimdT>
auto group_sum_tsl(
  simple_map_soa<SimdT> & dst,
  typename SimdT::base_t const * __restrict__ group_keys,
  typename SimdT::base_t const * __restrict__ group_values,
  size_t element_count
) -> size_t {
  size_t const map_element_count = dst.entry_count;
  size_t groups_count = 0;
  // map to store (key -> idx) pairs
  // idx is used to find key and value in dst (thus, the result is densely packed)
  simple_map_soa<SimdT> group_state(map_element_count, dst.empty_bucket_value);
  auto * const map_keys = group_state.keys;
  auto * const map_values = group_state.values;

  auto const end = group_keys + element_count;
  auto const empty_bucket_vec = tsl::set1<SimdT>(dst.empty_bucket_value);
  auto const all_false_mask = tsl::integral_all_false<SimdT>();

  while (group_keys != end) {
    auto const key = *group_keys;
    auto const key_vec = tsl::set1<SimdT>(key);
    auto const value = *group_values;
    // knuths multiplicative hash method
    auto pos_hint = static_cast<uint64_t>(key)*2654435761ull;
    while (true) {
      pos_hint = pos_hint % map_element_count;
      auto map_keys_vec = tsl::loadu<SimdT>(map_keys + pos_hint);
      auto const key_found_mask = tsl::equal_as_imask<SimdT>(map_keys_vec, key_vec);
      if (tsl::nequal<SimdT>(key_found_mask, all_false_mask)) {
        auto const found_position = tsl::tzc<SimdT>(key_found_mask);
        dst.values[*(map_values + pos_hint + found_position)] += value;
        break;
      }
      auto const empty_bucket_found_mask = tsl::equal_as_imask<SimdT>(map_keys_vec, empty_bucket_vec);
      if (tsl::nequal<SimdT, Idof>(empty_bucket_found_mask, all_false_mask)) {
        auto const empty_bucket_position = tsl::tzc<SimdT>(empty_bucket_found_mask);
        map_keys[pos_hint + empty_bucket_position] = key;
        map_values[pos_hint + empty_bucket_position] = groups_count;
        dst.keys[groups_count] = key;
        dst.values[groups_count] = value;
        ++groups_count;
        break;
      }
      pos_hint += SimdT::vector_element_count();
    }
    ++group_keys;
    ++group_values;
  }
  return groups_count;
}