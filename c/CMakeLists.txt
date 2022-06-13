cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME "primes")
project("${PROJECT_NAME}")

add_executable("${PROJECT_NAME}" main.c primes.c bitmap.c)

target_compile_options("${PROJECT_NAME}" PRIVATE -O3 -Wall -Wextra -std=c17)

target_link_libraries("${PROJECT_NAME}" -lm)