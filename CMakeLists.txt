cmake_minimum_required(VERSION 3.30)
project(montg_mul_algorithms)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(montg_mul_algorithms main.cpp
        src/util/binary_helper.cpp
        src/util/binary_helper.h
        src/algorithms/montgomery_algorithm.cpp
        src/algorithms/montgomery_algorithm.h
        src/util/number_generator.cpp
        src/util/number_generator.h
        src/util/result_parser.cpp
        src/util/result_parser.h
        src/algorithms/sos_algorithm.cpp
        src/algorithms/sos_algorithm.h
        src/algorithms/fips_algorithm.cpp
        src/algorithms/fips_algorithm.h
)
