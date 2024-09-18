//
// Created by ryanseavey on 9/17/24.
//

#ifndef HASHMA_RYCIPHER_HPP
#define HASHMA_RYCIPHER_HPP

#include <string>


class RyCipher {
    static constexpr unsigned char VIG_OFFSET = 'a';




public:
    template <class decType, class keyType>
    [[nodiscard]] static std::string encode(decType encrypt_me, keyType key);


    template <class decType, class keyType>
    [[nodiscard]] static decType decode(std::string cipherText, keyType key);



};

template<class decType, class keyType>
decType RyCipher::decode(std::string cipherText, keyType key) {
    auto * evil_key = static_cast<unsigned char*>(static_cast<void *>(key));

    return nullptr;
}

template<class decType, class keyType>
std::string RyCipher::encode(decType encrypt_me, keyType key) {
    return {};
}


#endif //HASHMA_RYCIPHER_HPP
