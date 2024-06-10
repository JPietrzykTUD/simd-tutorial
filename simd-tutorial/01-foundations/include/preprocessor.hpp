#pragma once
#ifdef COMPILER_EXPLORER
#define FORCE_INLINE
#else
#define FORCE_INLINE __attribute__((always_inline)) inline
#endif