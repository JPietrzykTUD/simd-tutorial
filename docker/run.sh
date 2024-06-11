#!/bin/bash
SRC_ROOT=$1
TARGET_NAME=$2
COMPILER=$3

function display_usage() {
  echo "Usage: run.sh <src_root> [target_name] [compiler]"
  echo "Arguments:"
  echo "  src_root: The root directory of the source code you want to compile"
  echo "  target_name (optional): The name of the target to compile. Default is all."
  echo "  compiler (optional): The compiler to use (gcc or clang). Default is gcc."
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
    C_COMPILER="gcc-13"
    CXX_COMPILER="g++-13"
    ;;
  clang)
    C_COMPILER="clang-18"
    CXX_COMPILER="clang++-18"
    ;;
  *)
    echo "Unknown compiler: ${COMPILER} using g++ as default"
    C_COMPILER="gcc-13"
    CXX_COMPILER="g++-13"
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

#      -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
cmake -S ${SRC_ROOT} -B ${SRC_ROOT}/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER=${C_COMPILER} -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
      -DPLATFORM=${PLATFORM} -DOUT_PATH=${SRC_ROOT}/bin

cmake --build ${SRC_ROOT}/build --target ${TARGET_NAME}

if [ "$TARGET_NAME" == "all" ]
then
  for file in $(find ${SRC_ROOT}/bin -type f -executable)
  do
    echo $file
    $file
  done
else
  target_out_path=$(echo "$TARGET_NAME" | sed -r 's/(_x86|_arm).*//' | sed -r 's/_/-/')
  if [ "$target_out_path" == "$TARGET_NAME" ]
  then
    exit 0
  fi
  fname=$(echo "$TARGET_NAME" | sed -r 's/.*(_x86_|_arm_)//')
  ${SRC_ROOT}/bin/*-${target_out_path}/${fname}
fi
