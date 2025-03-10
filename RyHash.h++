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

using cstring = const std::string;
using cvec = const std::vector<std::vector<std::string>>;
using cvfloat = boost::multiprecision::cpp_bin_float<200>;
using hashVal = uint32_t;
#define ss static cstring


namespace RyHash{
        //MAX_SENTENCE_BANK_SIZE
        extern inline const int MSBS;
        extern inline cvec dictionary;
        inline constexpr size_t BLOCK_SIZE{64 * 1024}; //64k


        extern inline boost::random::mt11213b RNG9000;
        extern inline std::uniform_int_distribution<unsigned long long> distribution;

        extern inline std::vector<uint8_t> string_to_bytes(const std::string&);
        extern inline std::vector<uint8_t> file_to_bytes(const std::filesystem::path&);

        /**
         * Doesn't hash with respect to time, rather: IT'S HASHING TIME.
         * @param hashMe the raw void * crap to be hashed
         * @param bits the size of the resultant hash
         * @return the spectacular hash!
         */
        extern inline u_int32_t hashTime(std::span<const uint8_t> hashMe, size_t bits);
}

inline boost::random::mt11213b RyHash::RNG9000{2};
inline std::uniform_int_distribution<unsigned long long> RyHash::distribution{};

//REMEMBER: OUTPUT SHOULD BE A HEX NUMBER OF LENGTH 8! (40320)
extern inline u_int32_t RyHash::hashTime(std::span<const uint8_t> hashMe, size_t bits)
{
        //split cocksucker into parts.
        size_t bin_size = hashMe.size();
        //check divisibility
        if (size_t rem{bin_size % BLOCK_SIZE})
        {

        }



        return 0xDEADBEEF;
}





















inline cvec RyHash::dictionary{
        { //Adjectives
                "Fat",
                "Ugly",
                "Silly",
                "Radical",
                "Eccentric",
                "Stinky",
                "Godly",
                "Unusual",
                "Suspicious",
        },
        { //People
                "Pete",
                "Dr.Sellers",
                "TomPetty",
                "Miguel",
                "Katie",
                "Carrot",
                "Parker",
                "Johninator",
                "Timmy"
        },
        { //Adverbs
                "Gaily",
                "Lethargically",
                "Quietly",
                "Unfortunately",
                "Cleverly",
                "Horribly",
                "Recklessly",
                "Casually",
                "Wonderfully"
        },
        { //Verbs
                "Walks",
                "Talks",
                "Barks",
                "Screams",
                "Perambulates",
                "PicksUp",
                "Drops",
                "Drives",
                "Lectures"
        },
        { //Prepositions
                "To",
                "From",
                "By",
                "ByWayOf",
                "Through",
                "Around",
                "About",
                "Out",
                "OutOf",
        },
        { //Places
                "School",
                "University",
                "Hell",
                "Store",
                "Classroom",
                "Bus",
                "Car",
                "Airplane",
                "JailCell"
        },
        {//Punctuation
                ".",
                "!",
                "?",
                "!?",
                ";",
                ",",
                "~",
                "...",
                "..?"
        }
};

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