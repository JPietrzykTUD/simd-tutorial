add_executable(foundations_aggregate_sum sum.cpp)
set_target_properties(foundations_aggregate_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}/01-foundations
  OUTPUT_NAME aggregate_sum
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(foundations_aggregate_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(foundations_aggregate_sum PRIVATE -mavx2)

add_executable(foundations_filter_sum filter_sum.cpp)
set_target_properties(foundations_filter_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}/01-foundations
  OUTPUT_NAME filter_sum
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(foundations_filter_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(foundations_filter_sum PRIVATE -mavx2)

add_executable(foundations_group_sum group_sum.cpp)
set_target_properties(foundations_group_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}/01-foundations
  OUTPUT_NAME group_sum
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(foundations_group_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(foundations_group_sum PRIVATE -mavx2)

add_executable(foundations_nsm_dsm nsm_dsm.cpp)
set_target_properties(foundations_nsm_dsm PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}/01-foundations
  OUTPUT_NAME nsm_dsm
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(foundations_nsm_dsm PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(foundations_nsm_dsm PRIVATE -mavx2)