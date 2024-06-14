/**********************************************************************************
  This file is part of the SIGMOD'24 tutorial: 
  SIMDified Data Processing-Foundations, Abstraction, and Advanced Techniques [1].
  Authors of the file: Johannes Pietrzyk, Dirk Habich.
  [1] https://doi.org/10.1145/3626246.3654694
  
  This work is licensed under the terms of the MIT license.  
  For a copy, see <https://opensource.org/licenses/MIT>.
 ***********************************************************************************/

#include <algorithm>
#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>
#include <algorithm>

#include "tsl/tsl.hpp"

#include "map.hpp"
#include "../preprocessor.hpp"

template<class SimdT, typename T1, typename T2, typename T3, typename T4>
struct group_sum {
  FORCE_INLINE static auto apply(
    T1 & dst,
    T2 group_keys,
    T3 group_values,
    T4 element_count
  ) -> size_t {
    std::cout << "Using " << tsl::type_name<typename SimdT::target_extension>() << std::endl;
    size_t const map_element_count = dst.entry_count;
    size_t groups_count = 0;
    // map to store (key -> idx) pairs
    // idx is used to find key and value in dst (thus, the result is densely packed)
    simple_map_soa<typename SimdT::base_type> group_state(map_element_count, dst.empty_bucket_value);
    auto map_keys = group_state.keys();
    auto map_values = group_state.values();
    auto dst_keys = dst.keys();
    auto dst_values = dst.values();

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
          dst_values[*(map_values + pos_hint + found_position)] += value;
          break;
        }
        auto const empty_bucket_found_mask = tsl::equal_as_imask<SimdT>(map_keys_vec, empty_bucket_vec);
        if (tsl::nequal<SimdT>(empty_bucket_found_mask, all_false_mask)) {
          auto const empty_bucket_position = tsl::tzc<SimdT>(empty_bucket_found_mask);
          map_keys[pos_hint + empty_bucket_position] = key;
          map_values[pos_hint + empty_bucket_position] = groups_count;
          dst_keys[groups_count] = key;
          dst_values[groups_count] = value;
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
};