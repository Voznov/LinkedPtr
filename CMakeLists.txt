cmake_minimum_required(VERSION 3.13)
project(linked_ptr)

set(CMAKE_CXX_STANDARD 11)

#add_executable(linked_ptr test.cpp)
#add_executable(linked_ptr smoke_test.cpp)
add_executable(linked_ptr gen_smoke_test.cpp)
