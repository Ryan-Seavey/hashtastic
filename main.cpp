#include <iostream>

#include "RyHash.hpp"

int main() {
    std::cout << RyHash::hashNormal("dog") << '\n';
    std::cout << RyHash::hashNormal("bog") << '\n';
    std::cout << RyHash::hashNormal("zog") << '\n';
    std::cout << RyHash::hashNormal("dob") << '\n';
    std::cout << RyHash::hashNormal("Mikey") << '\n';
    std::cout << RyHash::hashNormal("dog") << '\n';
//    std::cout << "-- Divide --\n";
//    std::cout << RyHash::hash<std::string>("dog")<< '\n';
//    std::cout << RyHash::hash<std::string>("bog")<< '\n';
//    std::cout << RyHash::hash<std::string>("dog") << '\n';
    return 0;
}
