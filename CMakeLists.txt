cmake_minimum_required(VERSION 3.29)
project(hashma)

set(CMAKE_CXX_STANDARD 23)

add_executable(hashma main.cpp
        RyHash.hpp)
