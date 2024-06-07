cmake_minimum_required(VERSION 3.15)
project(SimdTutorial)

set(TUTORIAL_CXX_STANDARD 20)
set(TURORIAL_BIN_ROOT ${PROJECT_SOURCE_DIR}/bin)

set(PLATFORM "x86" CACHE STRING "Platform specifier")

set(TUTORIAL_INCLUDE_DIRECTORIES
  ${PROJECT_SOURCE_DIR}/simd-tutorial/01-foundations/include
  ${PROJECT_SOURCE_DIR}/simd-tutorial/02-abstraction/include
)

if (PLATFORM STREQUAL "x86")
  add_subdirectory(${PROJECT_SOURCE_DIR}/simd-tutorial/01-foundations/src/x86)

elseif(PLATFORM STREQUAL "arm")
  
else()
  message(FATAL_ERROR "Invalid platform specified. Only x86 and arm are supported.")
endif()