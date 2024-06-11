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

struct table_dsm {
  uint32_t * const col0;
  uint32_t * const col1;
  size_t const row_count;
  explicit table_dsm(size_t row_count)
  : col0(new uint32_t[row_count]),
    col1(new uint32_t[row_count]),
    row_count(row_count){ }
  table_dsm(table_dsm const & f) = default;
  table_dsm(table_dsm && f) = default;
  ~table_dsm() {
    delete[] col0;
    delete[] col1;
  }
};

struct table_nsm {
  struct row {
    uint32_t col0;
    uint32_t col1;
  };
  row * const rows;
  size_t const row_count;
  explicit table_nsm(size_t row_count)
  : rows(new row[row_count]),
    row_count(row_count){ }
  table_nsm(table_nsm const & f) = default;
  table_nsm(table_nsm && f) = default;
  ~table_nsm() {
    delete[] rows;
  }
  constexpr static auto offset_of_col0() {
    return offsetof(row, col0);
  }
  constexpr static auto offset_of_col1() {
    return offsetof(row, col1);
  }
};

void transform(table_dsm & src, table_nsm const & dst) {
  for (size_t i = 0; i < src.row_count; ++i) {
    dst.rows[i].col0 = src.col0[i];
    dst.rows[i].col1 = src.col1[i];
  }
}
