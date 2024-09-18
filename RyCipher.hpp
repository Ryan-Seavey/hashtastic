//
// Created by ryanseavey on 9/17/24.
//

#ifndef HASHMA_RYCIPHER_HPP
#define HASHMA_RYCIPHER_HPP

#include <string>


class RyCipher {
    static constexpr unsigned char VIG_OFFSET = 'a';




    static unsigned char *
    Vigenere(unsigned char *evil_type, const unsigned char *evil_key, const size_t decSize, const size_t keySize, bool reverse);
public:


    template <class encType, class keyType>
    [[nodiscard]] static std::string encode(encType encrypt_me, keyType key);


    template <class decType, class keyType>
    [[nodiscard]] static decType decode(std::string cipherText, keyType key);
};

template<class encType, class keyType>
std::string RyCipher::encode(encType encrypt_me, keyType key) {
    auto evil_type = static_cast<unsigned char*>(static_cast<void *>(encrypt_me.data())),
           evil_key = static_cast<unsigned char*>(static_cast<void *>(key.data()));

    const size_t decSize = encrypt_me.size(), keySize = sizeof(keyType);

    evil_type = Vigenere(evil_type, evil_key, decSize, keySize, false);


    return {reinterpret_cast<const char*>(evil_type), decSize};
}

unsigned char * RyCipher::Vigenere(unsigned char *evil_type, const unsigned char *evil_key, const size_t decSize,
                   const size_t keySize, bool reverse) {
    for(size_t d = 0, k = 0; d < decSize; d++, ++k){
        if(k >= keySize) k = 0;
        if(not reverse) evil_type[d] -= evil_key[k] - VIG_OFFSET;
        else evil_type[d] += evil_key[k] - VIG_OFFSET;
    }
    return evil_type;
}

template<class decType, class keyType>
decType RyCipher::decode(std::string cipherText, keyType key) {
    auto evil_type = static_cast<unsigned char*>(static_cast<void *>(cipherText.data())),
            evil_key = static_cast<unsigned char*>(static_cast<void *>(key.data()));
    const size_t decSize = cipherText.size(), keySize = sizeof(keyType);

    evil_type = Vigenere(evil_type, evil_key, decSize, keySize, true);
    return reinterpret_cast<decType>(reinterpret_cast<void*>(evil_type));
}


#endif //HASHMA_RYCIPHER_HPP
