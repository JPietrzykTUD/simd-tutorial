add_executable(foundations_arm_aggregate_sum sum.cpp)
set_target_properties(foundations_arm_aggregate_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}/01-foundations
  OUTPUT_NAME aggregate_sum
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(foundations_arm_aggregate_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES_FOUNDATIONS})
target_compile_options(foundations_arm_aggregate_sum PRIVATE -march=native)

add_executable(foundations_arm_filter_sum filter_sum.cpp)
set_target_properties(foundations_arm_filter_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}/01-foundations
  OUTPUT_NAME filter_sum
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(foundations_arm_filter_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES_FOUNDATIONS})
target_compile_options(foundations_arm_filter_sum PRIVATE -march=native)

add_executable(foundations_arm_group_sum group_sum.cpp)
set_target_properties(foundations_arm_group_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}/01-foundations
  OUTPUT_NAME group_sum
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(foundations_arm_group_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES_FOUNDATIONS})
target_compile_options(foundations_arm_group_sum PRIVATE -march=native)
