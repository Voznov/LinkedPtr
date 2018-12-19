cmake_minimum_required(VERSION 3.13)
project(linked_ptr)

set(CMAKE_CXX_STANDARD 17)

add_executable(test test.cpp)
add_executable(smoke smoke_test.cpp)
add_executable(smoke_gen gen_smoke_test.cpp)
add_executable(RASDN run_assign_self_derived_nempty.cpp)
