#include <iostream>
#include <cassert>
#include "RyHash.hpp"
#define quick(T,arg) std::cout << RyHash::hash<T>(arg) << '\n'
#define sassert(T,arg) quick(T,arg); assert((RyHash::hash<T>(arg) == RyHash::hash<T>(arg)))

int main() {
    std::cout << RyHash::hash<std::string>("burrito") << '\n';
    std::cout << RyHash::hash<std::string>("burrito") << '\n';
    std::cout << RyHash::hash<std::string>("burrita") << '\n';
//    sassert(std::string, "bog");
//    sassert(std::string, "fog");
//    sassert(std::string, "bog");
//    sassert(const char*,"owie");
//    sassert(const char*,"owie");
//    sassert(int, 1);
//    sassert(bool, true);
//    sassert(int, 1);
//    sassert(char, '\1');
    return 0;
}
