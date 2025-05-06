cmake_minimum_required(VERSION 3.30)
project(montg_mul_algorithms)

set(CMAKE_CXX_STANDARD 20)

add_executable(montg_mul_algorithms main.cpp
        src/algorithms/fips_algorithm.cpp
        src/algorithms/fips_algorithm.h
        src/algorithms/cihs_algorithm.cpp
        src/algorithms/cihs_algorithm.h
        src/util/binary_helper.cpp
        src/util/binary_helper.h
)
