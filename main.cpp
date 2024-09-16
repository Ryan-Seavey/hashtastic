#include <iostream>

#include "RyHash.hpp"
#define quick(T,arg) std::cout << RyHash::hash<T>(arg) << '\n'

int main() {
    std::cout << RyHash::hash<std::string>("dog")<< '\n';
    std::cout << RyHash::hash<std::string>("bog")<< '\n';
    std::cout << RyHash::hash<std::string>("dog") << '\n';
    std::cout << RyHash::hash<std::string>("fuck") << '\n';
    std::cout << RyHash::hash<char*>((char*)"fuck") << '\n';
    std::cout << RyHash::hash<char*>((char*)"fuck") << '\n';

    std::cout << RyHash::hash<int>(1)<< '\n';
    std::cout << RyHash::hash<bool>(true)<< '\n';
    std::cout << RyHash::hash<int>(1) << '\n';
    std::cout << RyHash::hash<char>('\1') << '\n';
    quick(int,4);
//    std::cout << RyHash::hashNormal("dog") << '\n';
//    std::cout << RyHash::hashNormal("bog") << '\n';
//    std::cout << RyHash::hashNormal("zog") << '\n';
//    std::cout << RyHash::hashNormal("dob") << '\n';
//    std::cout << RyHash::hashNormal("Mikey") << '\n';
//    std::cout << RyHash::hashNormal("dog") << '\n';
    return 0;
}
