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

template<typename T>
struct simple_map_soa {
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