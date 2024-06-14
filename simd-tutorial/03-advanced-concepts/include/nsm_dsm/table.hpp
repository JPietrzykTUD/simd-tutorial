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
#include <memory>

template<typename T>
struct table_dsm {
  std::shared_ptr<T> const _col0;
  std::shared_ptr<T> const _col1;
  size_t const row_count;
  explicit table_dsm(size_t row_count)
  : _col0(new T[row_count]),
    _col1(new T[row_count]),
    row_count(row_count){ }
  table_dsm(table_dsm const & f) = default;
  table_dsm(table_dsm && f) = default;
  auto col0() const { return _col0.get(); }
  auto col1() const { return _col1.get(); }
};

template<typename T>
struct table_nsm {
  struct row {
    T col0;
    T col1;
  };
  std::shared_ptr<row> const _rows;
  size_t const row_count;
  explicit table_nsm(size_t row_count)
  : _rows(new row[row_count]),
    row_count(row_count){ }
  table_nsm(table_nsm const & f) = default;
  table_nsm(table_nsm && f) = default;
  constexpr static auto offset_of_col0() {
    return offsetof(row, col0);
  }
  constexpr static auto offset_of_col1() {
    return offsetof(row, col1);
  }
  auto rows() const { return _rows.get(); }
};

template<typename T>
void transform(table_dsm<T> & src, table_nsm<T> const & dst) {
  for (size_t i = 0; i < src.row_count; ++i) {
    dst.rows()[i].col0 = src.col0()[i];
    dst.rows()[i].col1 = src.col1()[i];
  }
}



// /*******************************************************************************************************************************
//   This file is part of the SIGMOD'24 tutorial: SIMDified Data Processing-Foundations, Abstraction, and Advanced Techniques [1].
//   Authors of the file: Johannes Pietrzyk, Dirk Habich.
//   [1] https://doi.org/10.1145/3626246.3654694
  
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, version 3.
  
//   This program is distributed in the hope that it will be useful, but
//   WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//   General Public License for more details.
  
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, see <http://www.gnu.org/licenses/>.
//  ******************************************************************************************************************************/
// #ifndef COMPILER_EXPLORER
// #pragma once
// #endif

// #include <cstdint>
// #include <cstddef>
// #include <memory>

// template<typename T>
// struct table_dsm {
//   std::shared_ptr<T> const data;
//   size_t const row_count;
//   size_t const col_count;
//   explicit table_dsm(size_t row_count, size_t col_count)
//   : data(new T[row_count*col_count]),
//     row_count(row_count), col_count(col_count){ }
//   table_dsm(table_dsm const & f) = default;
//   table_dsm(table_dsm && f) = default;
//   auto column(size_t col_idx) {
//     return data + (row_count*col_idx);
//   }
// };

// template<typename T>
// struct table_nsm {
//   std::shared_ptr<T> const data;
//   size_t const row_count;
//   size_t const col_count;
//   table_nsm(size_t row_count, size_t col_count)
//   : data(new T[row_count * col_count]),
//     row_count(row_count), col_count(col_count){ }
//   table_nsm(table_nsm const & f) = default;
//   table_nsm(table_nsm && f) = default;
//   auto offset_of(size_t col_idx) {
//     return col_idx * sizeof(T);
//   }
//   auto rows() const { return data.get(); }
// };

// template<typename T>
// void transform(table_dsm<T> & src, table_nsm<T> const & dst) {
//   auto const row_count = src.row_count;
//   auto const col_count = src.col_count;
//   for (size_t col = 0; col < row_count; ++col) {
//     auto col_data = src.column(col);
//     for (size_t row = 0; row < row_count; ++row) {
//       dst.rows()[(row * col_count) + col] = col_data[row];
//     }
//   }
// }

