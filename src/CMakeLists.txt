cmake_minimum_required(VERSION 3.28.3)
project(qucomp VERSION 0.3.0 DESCRIPTION "Quantum computing")

 # GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
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
add_executable(qucomp
  main.cpp
  ket.cpp
  bra.cpp
  matrix.cpp
  matrix1.cpp
  parser.cpp
  vectutils.cpp
  qugates.cpp
  tokenizer.cpp

  sourceContext.cpp
  token.cpp
  tokenizer1.cpp

  syntaxRules.cpp
  syntaxBuilder.cpp
  qusyntax.cpp

  commands.cpp
  compiler.cpp

  values.cpp
  operators.cpp
  processor.cpp
)

 target_include_directories(qucomp PUBLIC "../include" "${PROJECT_BINARY_DIR}")

 enable_testing()

add_executable(
  run_tests

  sourceContext.cpp
  token.cpp
  tokenizer1.cpp

  syntaxRules.cpp
  syntaxBuilder.cpp
  qusyntax.cpp
  
  commands.cpp
  compiler.cpp

  values.cpp
  operators.cpp
  processor.cpp

  matrix1.cpp
  vectutils.cpp

  testMatrix.cpp
  testTokenizer.cpp
  testToken.cpp
  testSyntaxBuilder.cpp
  testQusyntax.cpp
  testProcessor.cpp
  testCompiler.cpp
  testOperators.cpp
)
target_include_directories(run_tests PUBLIC "../include" "${PROJECT_BINARY_DIR}")
target_link_libraries(
  run_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)