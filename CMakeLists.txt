cmake_minimum_required(VERSION 3.29)
project(hashma)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werr \
    -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow \
    -Wformat=2 -Wdouble-promotion -Wimplicit-fallthrough -Woverloaded-virtual \
    -Wcast-align -Wcast-qual -Wpointer-arith -Wno-unused-parameter -Wnull-dereference \
    -Wstrict-overflow=5 -Wstrict-aliasing -Wold-style-cast -Weffc++ -Wdocumentation \
    -Wnon-virtual-dtor -Wctor-dtor-privacy -Wswitch-enum -Winit-self -Wuninitialized \
    -Wconditionally-supported -Wundefined-inline -Wodr -Wredundant-decls -Wmissing-declarations \
    -Wmissing-noreturn -Winline -Wstack-protector -Wpadded \
    -fstack-protector-strong -fno-omit-frame-pointer -fno-optimize-sibling-calls \
    -O2 -g -march=native -fno-omit-frame-pointer \
    -std=c++2b -fstrict-aliasing -fno-rtti -flto")

add_executable(hashma main.cpp
        RyHash.hpp
        RyCipher.hpp)

add_executable(RyCipher
        command_line.cpp
        RyCipher.hpp)
