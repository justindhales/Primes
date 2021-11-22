cmake_minimum_required(VERSION 3.19)
project(CPP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp")

include_directories(.)

add_executable(CPP
        main.cpp)