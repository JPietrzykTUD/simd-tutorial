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

#include "table.hpp"

template<typename T>
void filter_eq_sum_dsm_scalar(
  T * __restrict__ dst, 
  table_dsm<T> const & data,
  T const value
) {
  T result = 0;
  auto element_count = data.row_count;
  T const * to_filter = data.col0();
  T const * to_sum = data.col1();

  for (auto i = 0ull; i < element_count; ++i) {
    if (to_filter[i] == value) {
      result += to_sum[i];
    }
  }
  *dst = result;
}

template<typename T>
void filter_eq_sum_nsm_scalar(
  T * __restrict__ dst, 
  table_nsm<T> const & data,
  T const value
) {
  T result = 0;
  auto element_count = data.row_count;
  auto rows = data.rows();
  for (auto i = 0ull; i < element_count; ++i) {
    if (rows[i].col0 == value) {
      result += rows[i].col1;
    }
  }
  *dst = result;
}