cmake_minimum_required(VERSION 3.30)
project(montg_mul_algorithms)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(montg_mul_algorithms main.cpp
        src/algorithms/cihs_algorithm.cpp
        src/algorithms/cihs_algorithm.h
        src/util/binary_helper.cpp
        src/util/binary_helper.h
        src/algorithms/montgomery_algorithm.cpp
        src/algorithms/montgomery_algorithm.h
        src/util/number_generator.cpp
        src/util/number_generator.h
        src/util/result_parser.cpp
        src/util/result_parser.h
)
