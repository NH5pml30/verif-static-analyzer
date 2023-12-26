cmake_minimum_required(VERSION 3.0.0)
project(verif-static-analyzer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM 14 REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG
  HINTS "${LLVM_INSTALL_PREFIX}/lib/cmake/clang")
include(AddLLVM)

add_subdirectory(src)

find_package(Python3 REQUIRED)
set(LLVM_DEFAULT_EXTERNAL_LIT)
set(LLVM_LIT_OUTPUT_DIR "${CMAKE_BINARY_DIR}")

add_subdirectory(unittests)
set(VERIF_TEST_DEPS VerifUnitTests)

find_program(LLVM_FILECHECK FileCheck PATHS ${LLVM_TOOLS_BINARY_DIR})
add_subdirectory(test)

add_subdirectory(my-lit)
