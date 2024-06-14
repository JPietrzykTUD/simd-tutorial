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

