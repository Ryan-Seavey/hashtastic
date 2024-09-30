//
// Created by ryanseavey on 9/17/24.
//

#ifndef HASHMA_RYCIPHER_HPP
#define HASHMA_RYCIPHER_HPP

#include <string>
#include <concepts>
#include <tuple>
#include <memory>

template<typename T>
concept PODType = std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>;


template<typename T>
concept NoneOfTheAbove = !PODType<T> && !HasDataFunction<T>;

using std::get;
using evil_container = std::vector<unsigned char>;

class RyCipher {

    static constexpr unsigned short BLOCK_WIDTH = 4;
    static constexpr unsigned short BLOCK_SIZE = BLOCK_WIDTH * BLOCK_WIDTH;
    static constexpr unsigned char PADDING_START = '$';
    static constexpr unsigned char PADDING_CONT = '$';
    static constexpr unsigned char VIG_OFFSET = 255;

    template <class Type, class keyType>
    [[nodiscard]] static Type code(Type encrypt_me, keyType key, bool decode);

    static unsigned char * Vigenere(unsigned char *evil_type, const unsigned char *evil_key, const size_t decSize, const size_t keySize, bool reverse);

    static std::tuple<evil_container, size_t>  transform(unsigned char *evil_type, const size_t vecSize, bool removePadding, unsigned short blockSize = BLOCK_SIZE, unsigned short blockWidth = BLOCK_WIDTH);

    template <class Type, class keyType>
    static std::tuple<evil_container,evil_container> makeSenseOfThis(Type encrypt_me, keyType key);
public:

    template <class encType, class keyType>
    [[nodiscard]] static std::string encode(encType encrypt_me, keyType key){
        return code<encType, keyType>(encrypt_me, key, false);
    }

    template <class decType, class keyType>
    [[nodiscard]] static decType decode(std::string cipherText, keyType key){
        return code<decType, keyType>(cipherText, key, true);
    }
};



unsigned char * RyCipher::Vigenere(unsigned char *evil_type, const unsigned char *evil_key, const size_t decSize, const size_t keySize, bool reverse) {
    for(size_t d = 0, k = 0; d < decSize; d++, ++k){
        if(k >= keySize) k = 0;
        if(not reverse) evil_type[d] -= evil_key[k] - VIG_OFFSET;
        else evil_type[d] += evil_key[k] - VIG_OFFSET;
    }
    return evil_type;
}

std::tuple<evil_container, size_t> RyCipher::transform(unsigned char *evil_type, const size_t decSize, bool removePadding, unsigned short blockSize, unsigned short blockWidth) {
    evil_container evil_within(evil_type, evil_type + decSize);
    size_t vecSize = decSize;
    if(not removePadding && vecSize % blockSize != 0) {
        for (; vecSize % blockSize != 0; ++vecSize);
        //TODO: optimize this garbage, and run speed tests thereon.
        evil_within.emplace_back(PADDING_START);
        evil_within.resize(vecSize, PADDING_CONT);
    }
    evil_container klone(vecSize);
    for(int i = 0, currentBlockMax = blockSize, arrIndex = 0; i < vecSize && arrIndex < vecSize && currentBlockMax <= vecSize; ++i, currentBlockMax += blockSize){
        for(int microTickCounter{}; microTickCounter < blockWidth; microTickCounter++)
        for(int hop = currentBlockMax - blockSize + microTickCounter; hop < currentBlockMax; hop += blockWidth){
            klone[hop] = evil_within[arrIndex];
            arrIndex++;
        }
    }


    return {klone, vecSize};
}

template<class encType, class keyType>
encType RyCipher::code(encType encrypt_me, keyType key, bool decode) {
    evil_container evil_type;
    evil_container evil_key;
    auto carrier_of_evil = makeSenseOfThis(encrypt_me, key);

    evil_type = get<0>(carrier_of_evil);
    evil_key = get<1>(carrier_of_evil);
    size_t decSize = evil_type.size(), keySize = evil_key.size();

    Vigenere(evil_type.data(), evil_key.data(), decSize, keySize, decode);

    std::tie(evil_type , decSize) = transform(evil_type.data(), decSize, decode);

    if constexpr (PODType<encType> || NoneOfTheAbove<encType>){
        return reinterpret_cast<encType>(evil_type.data());
    } else if constexpr(std::is_same<encType, std::string>()) {
        return {reinterpret_cast<const char *>(evil_type.data()), decSize};
    } else if constexpr(HasDataFunction<encType>) {
        return {reinterpret_cast<encType>(evil_type.data()), decSize};
    }
}

template<class Type, class keyType>
std::tuple<evil_container,evil_container> RyCipher::makeSenseOfThis(Type encrypt_me, keyType key) {
    std::tuple<evil_container,evil_container> true_evil{};
    unsigned char * encrypt_me_data{}, * key_data{};
    size_t encrypt_me_size{}, key_size{};
    //Handle the Text Type first
    if constexpr (PODType<Type> || NoneOfTheAbove<Type>){
        encrypt_me_data =  reinterpret_cast<unsigned char *>(encrypt_me);
        encrypt_me_size = sizeof(Type);
    } else if constexpr (HasDataFunction<Type>) {
        encrypt_me_data =  reinterpret_cast<unsigned char *>(encrypt_me.data());
        encrypt_me_size = encrypt_me.size();
    }
    std::get<0>(true_evil) = std::vector(encrypt_me_data, encrypt_me_data + encrypt_me_size);
    //Handle the Key Type
    if constexpr (PODType<Type> || NoneOfTheAbove<Type>){
        key_data =  reinterpret_cast<unsigned char *>(key);
        key_size = sizeof(keyType);
    } else if constexpr (HasDataFunction<Type>) {
        key_data =  reinterpret_cast<unsigned char *>(key.data());
        key_size = key.size();
    }
    std::get<1>(true_evil) = std::vector(key_data, key_data + key_size);
    return true_evil;
}


#endif //HASHMA_RYCIPHER_HPP
