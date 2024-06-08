#!/bin/bash
SRC_ROOT=$1
COMPILER=$2
TARGET_NAME=$3

function display_usage() {
  echo "Usage: run.sh <src_root> [compiler] [target_name]"
  echo "Arguments:"
  echo "  src_root: The root directory of the source code you want to compile"
  echo "  compiler (optional): The compiler to use (gcc or clang). Default is gcc."
  echo "  target_name (optional): The name of the target to compile. Default is all."
}

if [ -z "$SRC_ROOT" ]; then
  echo "src_root is not set"
  display_usage
  exit 1
fi

if [ -z "$COMPILER" ]; then
  COMPILER="gcc"
fi

case $COMPILER in
  gcc)
    COMPILER="gcc-13"
    ;;
  clang)
    COMPILER="clang++-18"
    ;;
  *)
    echo "Unknown compiler: ${COMPILER} using g++ as default"
    COMPILER="gcc-13"
    ;;
esac

if [ -z "$TARGET_NAME" ]; then
  TARGET_NAME="all"
fi

. ${TUTORIAL_SCRIPTS}/compiler_explorer.env

case $UNAME in
  ARM64)
    PLATFORM="arm"
    ;;
  x86-64)
    PLATFORM="x86"
    ;;
  *)
    PLATFORM="unknown"
    ;;
esac

cmake -S ${SRC_ROOT} -B ${SRC_ROOT}/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER=${COMPILER} -DCMAKE_CXX_COMPILER=${COMPILER} \
      -DPLATFORM=${PLATFORM}

cmake --build ${SRC_ROOT}/build --target ${TARGET_NAME}

