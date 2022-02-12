cmake_minimum_required(VERSION 2.8)
project(rational)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(test
        rational.cpp
        rational.h
        test.cpp)
