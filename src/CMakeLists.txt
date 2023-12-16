include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
message(STATUS "${LLVM_INCLUDE_DIRS}")

find_package(Clang REQUIRED CONFIG)

add_executable(verif-static-analyzer main.cpp)

target_link_libraries(verif-static-analyzer
  PRIVATE
  clangAST
  clangASTMatchers
  clangFrontend
)

llvm_config(verif-static-analyzer USE_SHARED core)

set_target_properties(verif-static-analyzer
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
