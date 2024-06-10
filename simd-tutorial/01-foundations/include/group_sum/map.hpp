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
#include <algorithm>
#include <unordered_map>

struct simple_map_soa {
  uint32_t * const keys;
  uint32_t * const values;
  size_t const entry_count;
  uint32_t const empty_bucket_value;
  uint32_t const empty_value;
  explicit simple_map_soa(size_t entry_count, uint32_t ebv = 0, uint32_t evv = 0)
  : keys(new uint32_t[entry_count]),
    values(new uint32_t[entry_count]),
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

  friend bool operator==(simple_map_soa const & left, simple_map_soa const & right);
};

bool operator==(simple_map_soa const & left, simple_map_soa const & right) {
  if (left.entry_count != right.entry_count) {
    return false;
  }
  std::unordered_map<uint32_t, uint32_t> left_map;
  std::unordered_map<uint32_t, uint32_t> right_map;

  for (auto i = 0; i < left.entry_count; ++i) {
    left_map[left.keys[i]] = left.values[i];
    right_map[right.keys[i]] = right.values[i];
  }

  return (left_map == right_map);
}