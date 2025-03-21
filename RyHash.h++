//
// Created by ryanseavey on 9/12/24.
//
//export module RyHash;
#ifndef HASHMA_RYHASH
#define HASHMA_RYHASH

#include <string>
#include <cmath>
#include <bit>
#include <numbers>
#include <random>
#include <array>
#include <concepts>
#include <boost/math/special_functions/fibonacci.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/random.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <climits>
#include <condition_variable>
#include <span>
#include <filesystem>
#include <fstream>
#include <queue>

#include "evilDictionary.c++"
#include <thread>

using cstring = const std::string;
using cvec = const std::vector<std::vector<std::string>>;
using cvfloat = boost::multiprecision::cpp_bin_float<200>;
using hashVal = uint32_t;
#define ss static cstring


namespace RyHash{
        //MAX_SENTENCE_BANK_SIZE
        extern inline const int MSBS;
        inline constexpr std::span dictionary{evil::dictionary, std::size(evil::dictionary)};
        inline constexpr size_t BLOCK_SIZE{128 * 1024};

        //Yeah, we're Advanced Computer Security guys, of course we violate SOLID!
        extern inline std::vector<uint8_t> string_to_bytes(const std::string&);
        extern inline std::vector<uint8_t> file_to_bytes(std::filesystem::path);

        /**
         * Doesn't hash with respect to time, rather: IT'S HASHING TIME.
         * @param hashMe the raw void * crap to be hashed
         * @param bits the size of the resultant hash
         * @return the spectacular hash!
         */
        extern inline u_int32_t hashTime(std::vector<uint8_t>&& hashMe, size_t bits);
        /**
         * This is where the magic happens!
         * @param block what do you think?
         * @note block's size does not matter.
         */
        extern inline void processBlock(std::span<uint8_t> block);
}


//REMEMBER: OUTPUT SHOULD BE A HEX NUMBER OF LENGTH 8! (40320)
extern inline u_int32_t RyHash::hashTime(std::vector<uint8_t>&& theWholeEnchilada, size_t bits)
{
        //split cocksucker into parts.
        size_t bin_size = theWholeEnchilada.size();
        //check divisibility
        float quotient = static_cast<float>(bin_size) / BLOCK_SIZE;
        size_t numberOfBlocksWeWillNeed = std::ceil(quotient);
        bool paddingNeeded = (numberOfBlocksWeWillNeed - static_cast<int>(quotient));

        //reserve the space we will need.
        theWholeEnchilada.reserve(numberOfBlocksWeWillNeed * BLOCK_SIZE);
        //slap the entirety of hashMe into the true vector
        if (paddingNeeded) theWholeEnchilada.resize(numberOfBlocksWeWillNeed * BLOCK_SIZE, 0);
        const unsigned threadCount = std::thread::hardware_concurrency();
        boost::asio::thread_pool threads(threadCount);





        std::atomic<u_int32_t> result = 0x00000000;

        const std::function THEJOB{[&](size_t offset)
                {
                std::span currBlock{theWholeEnchilada.data() + offset, BLOCK_SIZE};
                processBlock(currBlock);
                u_int32_t babyBoy{~result};
                for (unsigned char z : currBlock)
                {
                        babyBoy ^= std::rotl(babyBoy, z) + ~(z * 0x1337u);
                }
                result ^= babyBoy;
        }
        };

        //fill the queue
        for (size_t i = 0; i < numberOfBlocksWeWillNeed; ++i)
        {
                size_t beginOffset = BLOCK_SIZE * i;
                boost::asio::post(threads, [THEJOB, beginOffset]{THEJOB(beginOffset);});
        }

        threads.join();

        return result;
}

inline void RyHash::processBlock(std::span<uint8_t> block)
{
        const std::string_view spice = dictionary[block[BLOCK_SIZE/4] % dictionary.size()];
        const size_t spicelen = spice.length();

        using MOD_TYPE = uint64_t;
        constexpr size_t FIT = sizeof(MOD_TYPE) / sizeof(uint8_t);
        for (int i = 0; i < BLOCK_SIZE; i += FIT)
        {
                MOD_TYPE full{};
                //stuffing time
                for (uint8_t index = 0; index < FIT; index++) (full <<= 8) |= (block[i + index] ^ spice[index % spicelen]);
                full = std::rotr(full, spice[full % spicelen]);
                std::memcpy(&block[i], &full, sizeof(full));
        }
}




















inline std::vector<uint8_t> RyHash::string_to_bytes(const std::string& str) {
        return {str.begin(), str.end()};
}

inline std::vector<uint8_t> RyHash::file_to_bytes(std::filesystem::path filename){
        std::ifstream file{filename, std::ios::binary | std::ios::ate};
        if (not file) throw std::runtime_error("File not found, hombre.");

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<uint8_t> buffer(size);
        file.read(reinterpret_cast<char *>(buffer.data()), size);
        return buffer;
}

#endif