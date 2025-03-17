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
#include <climits>
#include <span>
#include <filesystem>
#include <fstream>
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
        std::counting_semaphore semaphore(threadCount);

        std::vector<std::thread> threads;
        threads.reserve(numberOfBlocksWeWillNeed);
        std::atomic<u_int32_t> result{};

        for (size_t i = 0; i < numberOfBlocksWeWillNeed; ++i)
        {
                semaphore.acquire();
                size_t beginOffset = BLOCK_SIZE * i;
                threads.emplace_back([&semaphore, &theWholeEnchilada, beginOffset, &result]
                {
                        std::span currBlock{theWholeEnchilada.data() + beginOffset, BLOCK_SIZE};
                        processBlock(currBlock);
                        u_int32_t babyBoy = result;
                        for (unsigned char z : currBlock)
                        {
                                babyBoy = (babyBoy + z) * z;
                        }
                        result += babyBoy;
                        semaphore.release();
                });
        }

        for (auto& t : threads) {
                if (t.joinable()) {
                        t.join();
                }
        }


        return result;
}

inline void RyHash::processBlock(std::span<uint8_t> block)
{
        const size_t bsize = block.size();
        auto half = block.begin() + bsize/2ul;
        auto bend = block.rbegin();
        static constexpr size_t dsize = dictionary.size();
        const auto halfEnd = block.end();
        for (int i = 0; i < bsize; ++i)
        {
                const auto& word = dictionary[(*bend % dsize)];
                const auto& letter = word[*half % word.length()];
                block[i] ^= letter;
                ++bend, ++half;
                if (half == halfEnd) half = block.begin();
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