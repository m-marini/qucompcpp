cmake_minimum_required(VERSION 3.28.3)
project(qucomp VERSION 0.3.0 DESCRIPTION "Quantum computing")

 # GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

configure_file(../include/version.h.in version.h)

enable_testing()

add_executable(
  run_tests

  matrix.cpp
  vectutils.cpp
  testMatrix.cpp

  sourceContext.cpp
  token.cpp
  testToken.cpp

  tokenizer.cpp
  testTokenizer.cpp

  syntaxRules.cpp
  syntaxBuilder.cpp
  testSyntaxBuilder.cpp

  qusyntax.cpp
  testQusyntax.cpp
  
  commands.cpp
  compiler.cpp
  testCompiler.cpp

  values.cpp
  operators.cpp
  testOperators.cpp
  processor.cpp
  testProcessor.cpp
)
target_include_directories(run_tests PUBLIC "../include" "${PROJECT_BINARY_DIR}")
target_link_libraries(
  run_tests
  GTest::gtest_main
)

add_executable(qucomp

  matrix.cpp
  vectutils.cpp

  sourceContext.cpp
  token.cpp

  tokenizer.cpp

  syntaxRules.cpp
  syntaxBuilder.cpp

  qusyntax.cpp
  
  commands.cpp
  compiler.cpp

  values.cpp
  operators.cpp
  processor.cpp

  main.cpp
)

target_include_directories(qucomp PUBLIC "../include" "${PROJECT_BINARY_DIR}")

include(GoogleTest)
gtest_discover_tests(run_tests)