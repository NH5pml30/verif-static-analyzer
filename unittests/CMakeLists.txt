include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_custom_target(VerifUnitTests)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

function(add_unittest test_name)
  add_executable(
    ${test_name}
    ${ARGN}
  )
  target_link_libraries(
    ${test_name}
    PRIVATE
    GTest::gtest_main
    clangTooling
  )
  llvm_config(${test_name} USE_SHARED core)
  add_dependencies(VerifUnitTests ${test_name})
endfunction()

add_subdirectory(GoToLater)
