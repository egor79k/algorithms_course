project(rational)

set(CMAKE_CXX_STANDARD 14)

add_library(rational rational.h rational.cpp)

add_executable(test_rational test.cpp)

target_link_libraries(test_rational rational)
