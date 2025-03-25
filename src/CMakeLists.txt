cmake_minimum_required(VERSION 3.20)

project(anthony LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(ASIO_STANDALONE)

include(FetchContent)
FetchContent_Declare(asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG        asio-1-28-0
)
FetchContent_MakeAvailable(asio)
find_package(Threads REQUIRED)


add_executable(runner StudentHashRunner.cpp Hashes.cpp RyHash.h++ evilDictionary.c++)

add_executable(smasher AvalancheTest.cpp MurmurHash1.cpp Random.cpp Stats.cpp Hashes.cpp Bitslice.cpp MurmurHash2.cpp Bitvec.cpp KeysetTest.cpp sha1.cpp MurmurHash3.cpp City.cpp  lookup3.cpp  Platform.cpp SpeedTest.cpp CityTest.cpp main.cpp SuperFastHash.cpp PMurHash.c Spooky.cpp Types.cpp crc.cpp DifferentialTest.cpp md5.cpp SpookyTest.cpp
        RyHash.h++ evilDictionary.c++)

target_include_directories(runner PRIVATE
        ${asio_SOURCE_DIR}/asio/include
)
target_include_directories(smasher PRIVATE
        ${asio_SOURCE_DIR}/asio/include
)