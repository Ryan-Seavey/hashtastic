cmake_minimum_required(VERSION 3.29)
project(hashma)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(hashma main.cpp
        RyHash.hpp
        SortUtils.hpp)
