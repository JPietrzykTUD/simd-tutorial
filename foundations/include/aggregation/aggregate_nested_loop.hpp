#ifndef COMPILER_EXPLORER
#pragma once
#endif

#include <cstdint>
#include <cstddef>

#ifdef COMPILER_EXPLORER
#define FORCE_INLINE
#else
#define FORCE_INLINE __attribute__((always_inline)) inline
#endif

#ifndef VECTOR_COUNT
#define VECTOR_COUNT 4
#endif

FORCE_INLINE void aggregate_nested_loop(uint32_t * __restrict__ dst, uint32_t const * __restrict__ src, size_t element_count) {
  uint32_t result_arr[VECTOR_COUNT] = {};
  uint32_t const * const __restrict__ src_end = src + element_count;
  while (src < src_end) {
    #pragma unroll
    for (auto i = 0u; i < VECTOR_COUNT; ++i) {
      result_arr[i] += src[i];
    }
    src += VECTOR_COUNT;
  }
  uint32_t result = 0;
  for (auto i = 0u; i < VECTOR_COUNT; ++i) {
    result += result_arr[i];
  }
  while (src != src_end) {
    result += *src++;
  }
  *dst = result;
}

