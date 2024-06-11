cmake_minimum_required(VERSION 3.15)
project(SimdTutorial)

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(TUTORIAL_CXX_STANDARD 20)

if(NOT DEFINED ${OUT_PATH})
  set(TURORIAL_BIN_ROOT ${PROJECT_SOURCE_DIR}/bin)
else()
  set(TURORIAL_BIN_ROOT ${OUT_PATH})
endif()

set(PLATFORM "x86" CACHE STRING "Platform specifier")

set(TUTORIAL_INCLUDE_DIRECTORIES_FOUNDATIONS
  ${PROJECT_SOURCE_DIR}/simd-tutorial/01-foundations/include
)
set(TUTORIAL_INCLUDE_DIRECTORIES_ABSTRACTION
  ${PROJECT_SOURCE_DIR}/simd-tutorial/02-abstraction/include
)
set(TUTORIAL_INCLUDE_DIRECTORIES_ADVANCED
  ${PROJECT_SOURCE_DIR}/simd-tutorial/03-advanced-concepts/include
)
if (PLATFORM STREQUAL "x86")
  add_subdirectory(${PROJECT_SOURCE_DIR}/simd-tutorial/01-foundations/src/x86)
  add_subdirectory(${PROJECT_SOURCE_DIR}/simd-tutorial/02-abstraction/src/x86)
  add_subdirectory(${PROJECT_SOURCE_DIR}/simd-tutorial/03-advanced-concepts/src/x86)
elseif(PLATFORM STREQUAL "arm")
  add_subdirectory(${PROJECT_SOURCE_DIR}/simd-tutorial/01-foundations/src/arm)
  add_subdirectory(${PROJECT_SOURCE_DIR}/simd-tutorial/02-abstraction/src/arm)
else()
  message(FATAL_ERROR "Invalid platform specified. Only x86 and arm are supported.")
endif()