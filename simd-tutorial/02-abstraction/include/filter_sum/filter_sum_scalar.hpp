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


void filter_eq_sum_tsl(
  uint32_t * __restrict__ dst, 
  uint32_t const * __restrict__ to_filter, 
  uint32_t const value,
  uint32_t const * __restrict__ to_sum,
  size_t element_count
) {
  uint32_t result = 0;
  for (auto i = 0ull; i < element_count; ++i) {
    if (to_filter[i] == value) {
      result += to_sum[i];
    }
  }
  *dst = result;
}

