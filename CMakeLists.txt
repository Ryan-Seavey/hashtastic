cmake_minimum_required(VERSION 3.29)
project(hashma)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER clang++-18)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion")

find_package(Boost 1.87.0 REQUIRED COMPONENTS
        system
        filesystem
        math_tr1
        math_c99
)

add_executable(hashma main.cpp
        RyHash.h++)

target_link_libraries(hashma
        ${Boost_LIBRARIES}
)