/**********************************************************************************
  This file is part of the SIGMOD'24 tutorial: 
  SIMDified Data Processing-Foundations, Abstraction, and Advanced Techniques [1].
  Authors of the file: Johannes Pietrzyk, Dirk Habich.
  [1] https://doi.org/10.1145/3626246.3654694
  
  This work is licensed under the terms of the MIT license.  
  For a copy, see <https://opensource.org/licenses/MIT>.
 ***********************************************************************************/
#include <tsl/include/generated/declarations/io.hpp>
#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>
#include <chrono>
#include <tuple>
#include "tsl/tsl.hpp"

#include "../preprocessor.hpp"
#include "table.hpp"

template<class SimdT, typename T1, typename TableT, typename T3, typename T4>
struct filter_eq_sum_nsm_bsa {
  static auto apply(T1 dst, TableT data, T3 value, T4 block_size) {
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto const row_count = data.row_count;
    auto const col_count = 2;
    auto const rows_per_block = block_size / (2*sizeof(typename SimdT::base_type));
    auto const block_count = row_count / (SimdT::vector_element_count() * rows_per_block);

    auto rows_base = static_cast<void const *>(data.rows());

    /* Initialize result and pointers for SIMD processing */
    auto result_vec = tsl::set_zero<SimdT>();
    auto const pred_vec = tsl::set1<SimdT>(value);

    auto index_vec = tsl::custom_sequence<SimdT>(0, block_size);
    auto const block_inc_vec = tsl::set1<SimdT>(block_size * SimdT::vector_element_count());
    auto const col0_inc_vec = tsl::set1<SimdT>(offsetof(typename TableT::row, col0));
    auto const col1_inc_vec = tsl::set1<SimdT>(offsetof(typename TableT::row, col1));
    auto const row_inc_vec = tsl::set1<SimdT>(sizeof(typename TableT::row));

    for (auto block = 0; block < block_count; ++block) {
      auto block_index_vec = index_vec;
      for (auto row = 0; row < rows_per_block; ++row) {
        auto byte_index_col0_vec = tsl::add<SimdT>(block_index_vec, col0_inc_vec);
        auto byte_index_col1_vec = tsl::add<SimdT>(block_index_vec, col1_inc_vec);
        auto to_filter_vec = tsl::gather<SimdT, typename SimdT::template transform_extension<typename SimdT::offset_base_type>>(rows_base, byte_index_col0_vec, TSL_CVAL(int,1));
        auto to_sum_vec = tsl::gather<SimdT, typename SimdT::template transform_extension<typename SimdT::offset_base_type>>(rows_base, byte_index_col1_vec, TSL_CVAL(int,1));
        auto eq_mask = tsl::equal<SimdT>(to_filter_vec, pred_vec);
        result_vec = tsl::add<SimdT>(eq_mask, result_vec, to_sum_vec);
        block_index_vec = tsl::add<SimdT>(block_index_vec, row_inc_vec);
      }
      index_vec = tsl::add<SimdT>(index_vec, block_inc_vec);
    }

    auto result = tsl::hadd<SimdT>(result_vec);
    
    *dst = result;
    const auto t_end = std::chrono::high_resolution_clock::now();
    const auto extension = tsl::type_name<typename SimdT::target_extension>();
    const auto type = tsl::type_name<typename SimdT::base_type>();
    return std::make_tuple(std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count(), extension, type);
  }
};

