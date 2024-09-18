#include <iostream>
#include <cassert>
#include "RyHash.hpp"
#include "RyCipher.hpp"

#define quick(T,arg) std::cout << RyHash::hash<T>(arg) << '\n'
#define sassert(T,arg) quick(T,arg); assert((RyHash::hash<T>(arg) == RyHash::hash<T>(arg)))

int main() {
    std::string secretText = RyCipher::encode<std::string,std::string>("super_secret","key");
    std::cout << secretText << '\n';
    std::string newText = RyCipher::decode<const char *, std::string>(secretText, "key");
    std::cout << newText;
    return 0;
}
