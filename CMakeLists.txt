cmake_minimum_required(VERSION 3.13)
project(linked_ptr)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -fno-omit-frame-pointer")
set(LSAN_OPTIONS=verbosity=1:log_threads=1)

add_executable(test test.cpp)
add_executable(smoke smoke_test.cpp)
add_executable(smoke_gen gen_smoke_test.cpp)
add_executable(RASDN run_assign_self_derived_nempty.cpp)
add_executable(RSA1 run_swap_adjacent1.cpp)
