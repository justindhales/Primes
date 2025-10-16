cmake_minimum_required(VERSION 4.1)
set(PROJECT_NAME "primes")
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
        main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++23 -O3 -Wall -Wextra -march=native)
