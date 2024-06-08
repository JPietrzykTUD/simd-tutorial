add_executable(abstraction_aggregate_sum sum.cpp)
set_target_properties(abstraction_aggregate_sum PROPERTIES
  CXX_STANDARD ${TUTORIAL_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON 
  RUNTIME_OUTPUT_DIRECTORY ${TURORIAL_BIN_ROOT}/02-abstraction
  OUTPUT_NAME aggregate_sum
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)
target_include_directories(abstraction_aggregate_sum PRIVATE ${TUTORIAL_INCLUDE_DIRECTORIES})
target_compile_options(abstraction_aggregate_sum PRIVATE -mavx2)

