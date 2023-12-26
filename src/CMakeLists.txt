include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-but-set-variable)

add_executable(verif-static-analyzer main.cpp)

function(add_verif_module module_name)
  add_library(${module_name} ${ARGN})
  target_link_libraries(
    ${module_name}
    PUBLIC
    clangAST
    clangASTMatchers
    clangFrontend
  )
  target_include_directories(${module_name} PUBLIC "${CMAKE_SOURCE_DIR}/src")
  llvm_config(${module_name} USE_SHARED core)
  set_property(GLOBAL APPEND PROPERTY VERIF_MODULES ${module_name})
endfunction()

target_link_libraries(verif-static-analyzer
  PRIVATE
  clangAST
  clangASTMatchers
  clangFrontend
  clangTooling
)

target_include_directories(verif-static-analyzer PUBLIC "${CMAKE_SOURCE_DIR}/src")
llvm_config(verif-static-analyzer USE_SHARED core)

set_target_properties(verif-static-analyzer
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_subdirectory(ESCA)
add_subdirectory(GoToLater)

get_property(verif_modules GLOBAL PROPERTY VERIF_MODULES)
target_link_libraries(verif-static-analyzer
  PRIVATE
  ${verif_modules})
