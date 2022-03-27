project(m3i)

set(CMAKE_CXX_STANDARD 14)

add_library(m3i m3i.h m3i.cpp)

add_executable(test_m3i test_m3i.cpp)

target_link_libraries(test_m3i m3i)
