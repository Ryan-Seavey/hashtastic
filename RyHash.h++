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

using cstring = const std::string;
using cvec = const std::vector<std::vector<std::string>>;
using cvfloat = boost::multiprecision::cpp_bin_float<200>;
using hashVal = uint32_t;
#define ss static cstring


namespace RyHash{
        //MAX_SENTENCE_BANK_SIZE
        extern inline const int MSBS;
        inline constexpr std::span dictionary{evil::dictionary, std::size(evil::dictionary)};
        inline constexpr size_t BLOCK_SIZE{64 * 1024}; //64k

        //Yeah, we're Advanced Computer Security guys, of course we violate SOLID!
        template<class T>
        std::enable_if_t<std::is_arithmetic_v<T>, T> getRandom(unsigned seed);
        extern inline std::vector<uint8_t> string_to_bytes(const std::string&);
        extern inline std::vector<uint8_t> file_to_bytes(const std::filesystem::path&);

        /**
         * Doesn't hash with respect to time, rather: IT'S HASHING TIME.
         * @param hashMe the raw void * crap to be hashed
         * @param bits the size of the resultant hash
         * @return the spectacular hash!
         */
        extern inline u_int32_t hashTime(std::span<const uint8_t> hashMe, size_t bits);
        /**
         * This is where the magic happens!
         * @param block what do you think?
         * @note block's size does not matter.
         */
        extern inline void processBlock(std::span<uint8_t> block);
}


//REMEMBER: OUTPUT SHOULD BE A HEX NUMBER OF LENGTH 8! (40320)
extern inline u_int32_t RyHash::hashTime(std::span<const uint8_t> hashMe, size_t bits)
{
        std::vector<uint8_t> theWholeEnchilada;
        //split cocksucker into parts.
        size_t bin_size = hashMe.size();
        //check divisibility
        float quotient = static_cast<float>(bin_size) / BLOCK_SIZE;
        size_t numberOfBlocksWeWillNeed = std::ceil(quotient);
        bool paddingNeeded = (numberOfBlocksWeWillNeed - static_cast<int>(quotient));

        //reserve the space we will need.
        theWholeEnchilada.reserve(numberOfBlocksWeWillNeed * BLOCK_SIZE);
        //slap the entirety of hashMe into the true vector
        std::ranges::copy(std::as_const(hashMe), std::insert_iterator(theWholeEnchilada, theWholeEnchilada.begin()));
        if (paddingNeeded) theWholeEnchilada.resize(numberOfBlocksWeWillNeed * BLOCK_SIZE, 0);

        processBlock(theWholeEnchilada);
        u_int32_t result{};
        for (const auto& z : theWholeEnchilada)
        {
                result += z;
                result *= z;
        }
        return result;
}

inline void RyHash::processBlock(std::span<uint8_t> block)
{
        auto half = block.begin() + block.size()/2;
        auto bend = block.rend();
        for (int i = 0; i < block.size(); ++i)
        {
                block[i] ^= getRandom<uint8_t>(block[i % block.size()]);
                ++bend, ++half;
                if (half == block.end()) half = block.begin();
        }
}

template<class T>
std::enable_if_t<std::is_arithmetic_v<T>, T>
RyHash::getRandom(const unsigned seed)
{
      thread_local boost::random::mt11213b engine;
        if constexpr(std::is_integral_v<T>)
        {
                engine.seed(seed);
                boost::random::uniform_int_distribution<T> dist;
                //if (nobodyIsNull) dist.param(boost::random::uniform_int_distribution<T>(a,b));
                return dist(engine);
        } else if constexpr (std::is_floating_point_v<T>)
        {
                engine.seed(seed);
                boost::random::uniform_real_distribution<T> dist;
                //if (nobodyIsNull) dist.param(typename boost::random::uniform_real_distribution<T>::param_type(a,b));
                return dist(engine);
        }
        return{};
}



















inline std::vector<uint8_t> RyHash::string_to_bytes(const std::string& str) {
        return std::vector<uint8_t>(str.begin(), str.end());
}

inline std::vector<uint8_t> RyHash::file_to_bytes(const std::filesystem::path& filename){
        std::ifstream file{filename, std::ios::binary | std::ios::ate};
        if (not file) throw std::runtime_error("File not found, hombre.");

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<uint8_t> buffer(size);
        file.read(reinterpret_cast<char *>(buffer.data()), size);
        return buffer;
}

#endif