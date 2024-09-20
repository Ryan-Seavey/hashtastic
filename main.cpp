#include <iostream>
#include <cassert>
#include "RyHash.hpp"
#include "RyCipher.hpp"

#define quick(A) std::cout << RyCipher::decode<std::string, std::string>(RyCipher::encode<std::string,std::string>(A,"key"), "key") << '\n';
#define sassert(T,arg) quick(T,arg); assert((RyHash::hash<T>(arg) == RyHash::hash<T>(arg)))

int main() {
    std::string secretText = RyCipher::encode<std::string,std::string>("The quick fox jumped over the lazy brown dog.","key");
    std::cout << secretText << '\n';
    std::cout << RyCipher::decode<std::string, std::string>(secretText, "key") << '\n';

    quick("super_secret")
    quick("small_test")
    quick("tiny")
    quick("super duper mega extra big, long string for information; I think, therefore I am. Always? Always.")
    quick("welcometothebar")
    quick("welcometothebarn")
    quick("welcometothebarnYes")

    return 0;
}
