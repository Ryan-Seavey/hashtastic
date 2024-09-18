cmake_minimum_required(VERSION 3.29)
project(hashma)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wconversion")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wcast-align -Wfloat-equal -Wpointer-arith -Wformat=2 -Wundef -Wcast-qual -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wduplicated-cond -Wlogical-op")

add_executable(hashma main.cpp
        RyHash.hpp
        RyCipher.hpp)

add_executable(RyCipher
        command_line.cpp
        RyCipher.hpp)
