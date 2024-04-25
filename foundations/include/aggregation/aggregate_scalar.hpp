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

FORCE_INLINE void aggregate_scalar(uint32_t * __restrict__ dst, uint32_t const * __restrict__ src, size_t element_count) {
  uint32_t result = 0;
  for(auto i = 0u; i < element_count; ++i) {
    result += src[i];
  }
  *dst = result;
}

