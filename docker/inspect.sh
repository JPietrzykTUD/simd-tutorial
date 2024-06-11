#!/bin/bash
FILE_PATH=$1
COMPILER=$2
SIMD=$3
ADDITIONAL_FLAGS=$4

function display_usage() {
  echo "Usage: inspect.sh <file_path> [compiler] [simd] [additional_flags]"
  echo "Arguments:"
  echo "  file_path: The path to the file you want to compile"
  echo "  compiler (optional): The compiler to use (gcc or clang). Default is gcc."
  echo "  simd (optional): The SIMD flag to use (sse, avx, avx512). Default is the default SIMD flag."
  echo "  additional_flags (optional): Additional flags to pass to the compiler."
}

if [ -z "$FILE_PATH" ]; then
  echo "FILE_PATH is not set"
  display_usage
  exit 1
fi

source ${TUTORIAL_SCRIPTS}/compiler_explorer.env

if [ -z "$SIMD" ]; then
  FLAGS="${CCEXP_SIMD_DEFAULT_FLAGS}"
else
  # switch case for SIMD (sse,avx,avx512)
  case $SIMD in
    sse)
      FLAGS="${CCEXP_SSE_FLAGS}"
      ;;
    avx)
      FLAGS="${CCEXP_AVX_FLAGS}"
      ;;
    avx512)
      FLAGS="${CCEXP_AVX512_FLAGS}"
      ;;
    *)
      echo "Unknown SIMD flag: $SIMD"
      FLAGS="${CCEXP_SIMD_DEFAULT_FLAGS}"
      ;;
  esac
fi
FLAGS="${FLAGS} ${CCEXP_FLAGS}"

if [ -z "$ADDITIONAL_FLAGS" ]; then
  FLAGS="${FLAGS}"
else
  FLAGS="${FLAGS} ${ADDITIONAL_FLAGS}"
fi

if [ -z "$COMPILER" ]; then
  COMPILER="gcc"
fi

case $COMPILER in
  gcc)
    COMPILER="${CCEXP_GCC}"
    ;;
  clang)
    COMPILER="${CCEXP_CLANG}"
    ;;
  *)
    echo "Unknown compiler: ${COMPILER} using g++ as default"
    COMPILER="${CCEXP_GCC}"
    ;;
esac

cexpl --compiler ${COMPILER} --compare --cflags "${FLAGS}" $FILE_PATH