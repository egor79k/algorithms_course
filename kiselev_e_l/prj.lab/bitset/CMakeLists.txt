project(bitset)

include_directories(../../prj.parts)

add_library(bitset bitset.h bitset.cpp)

add_executable(test_bitset test.cpp)

target_link_libraries(test_bitset bitset)
