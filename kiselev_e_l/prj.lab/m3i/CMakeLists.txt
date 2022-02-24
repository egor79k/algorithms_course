project(m3i)

include_directories(../../prj.parts/doctest)

add_library(m3i m3i.h m3i.cpp)

add_executable(test_m3i test_m3i.cpp)

target_link_libraries(test_m3i m3i)
