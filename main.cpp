#include <iostream>
#include <cassert>
#include "RyHash.hpp"
#include "RyCipher.hpp"

#define quick(T,arg) std::cout << RyHash::hash<T>(arg) << '\n'
#define sassert(T,arg) quick(T,arg); assert((RyHash::hash<T>(arg) == RyHash::hash<T>(arg)))

int main(int argc, char* argv[]) {
    std::cout << RyHash::hash<std::string>("burrito") << '\n';
    std::cout << RyHash::hash<std::string>("burrito") << '\n';
    std::cout << RyHash::hash<std::string>("burrita") << '\n';

    RyCipher::encode<std::string,std::string>("secret","key");
    return 0;
}
