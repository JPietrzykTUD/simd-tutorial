#include <cstdint>
#include <cstddef>

#include "utils.hpp"
#include "filter_sum/filter_sum_scalar.hpp"
#include "nsm_dsm/filter_sum_dsm_avx2.hpp"
#include "nsm_dsm/filter_sum_nsm_avx2.hpp"

int main() {
  uint32_t result_scalar, result_dsm, result_nsm;
  constexpr size_t element_count = 1 << 26;

  table_dsm tab_dsm{element_count};
  table_nsm tab_nsm{element_count};

  fill(tab_dsm.col0, element_count, 1, 20);
  fill(tab_dsm.col1, element_count, 0, 100);

  transform(tab_dsm, tab_nsm);

  filter_eq_sum_scalar(&result_scalar, tab_dsm.col0, 10, tab_dsm.col1, element_count);
  filter_eq_sum_dsm_avx2(&result_dsm, tab_dsm, 10);
  filter_eq_sum_nsm_avx2(&result_nsm, tab_nsm, 10);
  
  verify(result_scalar == result_dsm);
  verify(result_dsm == result_nsm);

  return 0;
}
