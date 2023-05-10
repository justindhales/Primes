cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME "primes")
project(${PROJECT_NAME})

add_executable("${PROJECT_NAME}" main.c primes.c bitmap.c)

set(CMAKE_C_STANDARD 17)

target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall -Wextra -march=native)

target_link_libraries("${PROJECT_NAME}" -lm)
