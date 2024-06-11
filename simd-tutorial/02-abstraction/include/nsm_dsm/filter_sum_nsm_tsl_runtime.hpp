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
#include <tsl/include/generated/declarations/io.hpp>
#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>
#include <iostream>
#include "tsl/tsl.hpp"

#include "../preprocessor.hpp"
#include "table.hpp"

template<class SimdT, typename T1, typename TableT, typename T3>
struct filter_eq_sum_nsm {
  static void apply(T1 dst, TableT data, T3 value) {
    std::cout << "  Using NSM with " << tsl::type_name<typename SimdT::target_extension>() << ": ";
    auto element_count = data.row_count;
    auto rows_base = static_cast<void const *>(data.rows());

    /* Calculate pointers for SIMD processing and scalar remainder */
    const auto remainder = element_count & (SimdT::vector_element_count() - 1);
    element_count -= remainder;
    /* Initialize result and pointers for SIMD processing */
    auto result_vec = tsl::set_zero<SimdT>();
    auto const pred_vec = tsl::set1<SimdT>(value);
    auto const index_inc_vec = tsl::set1<SimdT>(sizeof(typename TableT::row)*SimdT::vector_element_count());
    auto const col0_inc_vec = tsl::set1<SimdT>(offsetof(typename TableT::row, col0));
    auto const col1_inc_vec = tsl::set1<SimdT>(offsetof(typename TableT::row, col1));
    auto index_vec = tsl::custom_sequence<SimdT>(0,sizeof(typename TableT::row));
    for (auto row_idx = 0; row_idx < element_count; row_idx += SimdT::vector_element_count()) {
      auto byte_index_col0_vec = tsl::add<SimdT>(index_vec, col0_inc_vec);
      auto byte_index_col1_vec = tsl::add<SimdT>(index_vec, col1_inc_vec);
      auto to_filter_vec = tsl::gather<SimdT, typename SimdT::template transform_extension<typename SimdT::offset_base_type>>(rows_base, byte_index_col0_vec, TSL_CVAL(int,1));
      auto to_sum_vec = tsl::gather<SimdT, typename SimdT::template transform_extension<typename SimdT::offset_base_type>>(rows_base, byte_index_col1_vec, TSL_CVAL(int,1));
      auto eq_mask = tsl::equal<SimdT>(to_filter_vec, pred_vec);
      result_vec = tsl::add<SimdT>(eq_mask, result_vec, to_sum_vec);
      index_vec = tsl::add<SimdT>(index_vec, index_inc_vec);
    }

    auto result = tsl::hadd<SimdT>(result_vec);
    
    /* Calculate pointers for remainder processing */
    auto remainder_rows = data.rows() + element_count;
    /* Start remainder processing */
    for (auto row_idx = 0; row_idx < remainder; ++row_idx) {
      if (remainder_rows->col0 == value) {
        result += remainder_rows->col1;
      }
    }
    
    *dst = result;
  }
};

