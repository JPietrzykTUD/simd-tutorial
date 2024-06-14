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
#include <algorithm>
#include <unordered_map>

#include <memory>

template<typename T>
struct simple_map_soa {
  std::shared_ptr<T> const _keys;
  std::shared_ptr<T> const _values;
  size_t const entry_count;
  T const empty_bucket_value;
  T const empty_value;
  explicit simple_map_soa(size_t entry_count, T ebv = 0, T evv = 0)
  : _keys(new T[entry_count]),
    _values(new T[entry_count]),
    entry_count(entry_count),
    empty_bucket_value(ebv),
    empty_value(evv) {
      std::fill(_keys.get(), _keys.get() + entry_count, empty_bucket_value);
      std::fill(_values.get(), _values.get() + entry_count, empty_value);
  }
  simple_map_soa(simple_map_soa const & f) = default;
  simple_map_soa(simple_map_soa && f) = default;

  auto keys() const {
    return _keys.get();
  }
  auto values() const {
    return _values.get();
  }

  bool operator==(simple_map_soa<T> const & right) const {
    if (entry_count != right.entry_count) {   
      return false;
    }
    std::unordered_map<T, T> left_map;
    std::unordered_map<T, T> right_map;

    for (auto i = 0; i < entry_count; ++i) {
      left_map[keys()[i]] = values()[i];
      right_map[right.keys()[i]] = right.values()[i];
    }

    return (left_map == right_map);
  }
};
