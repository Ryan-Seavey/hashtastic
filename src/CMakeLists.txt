# 🛠️ Bump minimum CMake version for modern features
cmake_minimum_required(VERSION 3.20)

set_source_files_properties(StudentHashRunner.cpp PROPERTIES LANGUAGE CXX)

project(hashlab LANGUAGES C CXX)

# 🛠️ Use consistent C++11 ABI (0 = old ABI for compatibility)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-D_GLIBCXX_USE_CXX11_ABI=0)  # 🛠️ FIXED INCONSISTENT ABI

# 🛠️ Verify GCC 11 install path
set(CMAKE_C_COMPILER gcc-11)
set(CMAKE_CXX_COMPILER g++-11)


add_executable(runner StudentHashRunner.cpp Hashes.cpp RyHash.h++ evilDictionary.c++)

add_executable(smasher AvalancheTest.cpp MurmurHash1.cpp Random.cpp Stats.cpp Hashes.cpp Bitslice.cpp MurmurHash2.cpp Bitvec.cpp KeysetTest.cpp sha1.cpp MurmurHash3.cpp City.cpp  lookup3.cpp  Platform.cpp SpeedTest.cpp CityTest.cpp main.cpp SuperFastHash.cpp PMurHash.c Spooky.cpp Types.cpp crc.cpp DifferentialTest.cpp md5.cpp SpookyTest.cpp
        RyHash.h++ evilDictionary.c++)

# 🛠️ PROPER static linking setup
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

# 🛠️ Correct version script format
file(WRITE ${CMAKE_SOURCE_DIR}/glibc.map "GLIBC_2.35 { };\nGLIBC_2.17 { };\n")

# 🛠️ Verified library paths for GCC 11 static libs
link_directories(
        /usr/lib/gcc/x86_64-linux-gnu/11
        /usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu
)

# 🛠️ Unified linker flags
set(CMAKE_EXE_LINKER_FLAGS
        "-Wl,--version-script=${CMAKE_SOURCE_DIR}/glibc.map \
     -static-libstdc++ \
     -static-libgcc \
     -Wl,--start-group \
     -lpthread \
     -ldl \
     -Wl,--end-group"
)

# 🛠️ Boost configuration checks
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.87.0 REQUIRED COMPONENTS
        system filesystem program_options log random thread
)

# 🛠️ Verified static linking sequence
foreach(target runner smasher)
    target_link_libraries(${target} PRIVATE
            ${Boost_LIBRARIES}
            -Wl,--start-group
            /usr/lib/gcc/x86_64-linux-gnu/11/libstdc++.a
            /usr/lib/gcc/x86_64-linux-gnu/11/libgcc_eh.a
            /usr/lib/gcc/x86_64-linux-gnu/11/libgcc.a
            -Wl,--end-group
            -Wl,-Bstatic
            -lc
            -lm
            -lrt
            -Wl,-Bdynamic
    )
endforeach()

