cmake_minimum_required(VERSION 2.8)

project(rational)

include_directories(../../prj.parts/doctest)

add_library(rational rational.h rational.cpp)

add_executable(test test.cpp)

target_link_libraries(test rational)
