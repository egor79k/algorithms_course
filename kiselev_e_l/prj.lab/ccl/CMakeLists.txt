project(ccl)

set(CMAKE_CXX_STANDARD 14)

add_executable(ccl ccl.cpp)

target_link_libraries(ccl m3i)
