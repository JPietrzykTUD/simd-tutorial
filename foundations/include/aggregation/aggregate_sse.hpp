#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>
#include <immintrin.h>


#ifdef COMPILER_EXPLORER
#define FORCE_INLINE
#else
#define FORCE_INLINE __attribute__((always_inline)) inline
#endif


FORCE_INLINE void aggregate_sse(uint32_t * __restrict__ dst, uint32_t const * __restrict__ src, size_t element_count) {
  __m128i result_arr = _mm_setzero_si128();
  __m128i const * const __restrict__ src_end = reinterpret_cast<__m128i const *>(src + element_count);
  __m128i const * __restrict__ src_current = reinterpret_cast<__m128i const *>(src);
  while (src_current < src_end) {
    __m128i src_vec = _mm_loadu_si128(src_current);
    result_arr = _mm_add_epi32(result_arr, src_vec);
    ++src_current;
  }
  alignas(16) uint32_t result_arr_aligned[4];
  _mm_store_si128(reinterpret_cast<__m128i *>(result_arr_aligned), result_arr);
  uint32_t result = 0;
  for (auto i = 0u; i < 4; ++i) {
    result += result_arr_aligned[i];
  }
  while (src_current != src_end) {
    result += *src++;
  }
  *dst = result;
}

