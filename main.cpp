#include <iostream>
#include <cassert>
#include <chrono>
#include <iomanip>
#include "RyHash.hpp"
#include "RyCipher.hpp"

std::string truncateMa(const std::string& str, size_t width) {
    if (str.length() > width) {
        return str.substr(0, width - 3) + "...";
    }
    return str;
}

static constexpr size_t width = 56/2;
static constexpr float offset = 2.0f/3.0f;

inline void quick(const std::string &str, ull y = 2){
    auto start = std::chrono::high_resolution_clock::now();
    auto ciphertext = RyCipher::encode<std::string,std::string>(str, "key", y);
    auto end = std::chrono::high_resolution_clock::now();
    double encodeTime = std::chrono::duration<double>(end - start).count();
    start = std::chrono::high_resolution_clock::now();
    (void) RyCipher::decode<std::string, std::string>(ciphertext, "key", y);
    end = std::chrono::high_resolution_clock::now();
    double decodeTime = std::chrono::duration<double>(end - start).count();
    double time_differential = std::abs(decodeTime - encodeTime);
    std::string color = (encodeTime > decodeTime) ? "\033[31m" : "\033[32m";
    std::cout << std::left << std::setw(width) << truncateMa(str, width)
    << std::right << std::setw(width) << std::setprecision(10) << std::fixed << encodeTime
    << std::right << std::setw(width*offset) << std::setprecision(10) << std::fixed << decodeTime
    << std::right << std::setw(width*offset) << std::setprecision(10) << std::fixed << (decodeTime + encodeTime)
    << std::right << std::setw(width/3) << std::setprecision(10) << std::fixed << color << time_differential
    << "\033[0m" << '\n';
}

int main() {
    for(ull i = 0, j = 0; i < LONG_MAX; ++j, i = static_cast<ull>(std::pow(2, j))) {
        std::cout << std::setfill('-') << std::setw(width << 2) << std::endl;
        std::cout << "\nNow at: " << i << " passes." << std::endl;
        std::cout << std::setfill(' ') << std::left << std::setw(width) << "String name"
                        << std::right << std::setw(width) << "Encode time"
                        << std:: right << std::setw(width*offset) << "Decode time"
                        << std:: right << std::setw(width*offset) << "Total time"
                        << std:: right << std::setw(width*offset) << "Differential"
                        << std::endl;
        quick("super_secret", i);
        quick("small_test", i);
        quick("tiny", i);
        quick("super duper mega extra big, long string for information; I think, therefore I am.", i);
        quick("welcometothebar", i);
        quick("welcometothebarn",i);
        quick("welcometothebarnYes", i);
        quick("welcometothebar````", i);
    }
}
