cmake_minimum_required(VERSION 3.29)
project(hashma)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER clang++-18)
add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -march=native)
    message("Release time!")
else()
    message("Not release.")
endif()

    find_package(Boost 1.87.0 REQUIRED COMPONENTS
        system
        filesystem
        math_tr1
        math_c99
        program_options
        log
        random
        thread
)

add_executable(hashma main.cpp
        RyHash.h++
        CLI.c++
        evilDictionary.c++)

add_executable(CLI CLI.c++ RyHash.h++)

target_link_libraries(CLI ${Boost_LIBRARIES})

target_link_libraries(hashma
        ${Boost_LIBRARIES}
)