add_executable(aggregate_sum sum.cpp)
set_target_properties(aggregate_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(aggregate_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(aggregate_sum PRIVATE -mavx2)

add_executable(filter_sum filter_sum.cpp)
set_target_properties(filter_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(filter_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(filter_sum PRIVATE -mavx2)

add_executable(group_sum group_sum.cpp)
set_target_properties(group_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(group_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(group_sum PRIVATE -mavx2)

add_executable(nsm_dsm nsm_dsm.cpp)
set_target_properties(nsm_dsm PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(nsm_dsm PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(nsm_dsm PRIVATE -mavx2)