cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME "primes")
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
        main.cpp)

set(CMAKE_CXX_STANDARD 20)
target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall -Wextra -march=native)
