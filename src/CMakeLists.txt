cmake_minimum_required(VERSION 3.28.3)
project(qucomp VERSION 1.0)
add_executable(qucomp main.cpp ket.cpp bra.cpp matrix.cpp parser.cpp vectutils.cpp qugates.cpp)
target_include_directories(qucomp PUBLIC
                           "../include"
                           )
