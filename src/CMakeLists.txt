cmake_minimum_required(VERSION 3.28.3)
project(qucomp VERSION 0.2.1 DESCRIPTION "Quantum computing")
configure_file(../include/version.h.in version.h)
add_executable(qucomp
 main.cpp
 ket.cpp
 bra.cpp
 matrix.cpp
 parser.cpp
 vectutils.cpp
 qugates.cpp
 tokenizer.cpp
 tests.cpp)
target_include_directories(qucomp PUBLIC "../include" "${PROJECT_BINARY_DIR}")
