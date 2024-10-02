//
// Created by ryanseavey on 9/17/24.
//

#ifndef HASHMA_RYCIPHER_HPP
#define HASHMA_RYCIPHER_HPP

#define NDS [[nodiscard]] static

#include <string>
#include <concepts>
#include <tuple>
#include <memory>
#include <string>

template<typename T>
concept PODType = std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>;


template<typename T>
concept NoneOfTheAbove = !PODType<T> && !HasDataFunction<T>;

using std::get;
using evil_container = std::vector<unsigned char>;
using evil_char = unsigned char;
using ull = unsigned long long;
using namespace std::literals;

class RyCipher {
    static constexpr unsigned long long DEFAULT_PASSES = 3;
    static constexpr unsigned short BLOCK_WIDTH = 4;
    static constexpr unsigned short BLOCK_SIZE = BLOCK_WIDTH * BLOCK_WIDTH;
    static constexpr unsigned short TIMEOUT_MAX = 256;
    static constexpr evil_char PADDING_START = '`';
    static constexpr evil_char PADDING_CONT = '$';
    static constexpr evil_char VIG_OFFSET = 255;

    template <class Type, class keyType>
    NDS Type code(Type encrypt_me, keyType key, bool decode, unsigned long long passes);

    static evil_char * Vigenere(unsigned char *evil_type, const unsigned char *evil_key, const size_t decSize, const size_t keySize, bool reverse);

    static void transform(evil_container &evil_type, size_t & decSize, bool removePadding, unsigned short blockSize = BLOCK_SIZE, unsigned short blockWidth = BLOCK_WIDTH);

    template <class Type, class keyType>
    NDS std::tuple<evil_container,evil_container> makeSenseOfThis(Type encrypt_me, keyType key);

    static void TidyUp(evil_container &evil_type, size_t &size);

public:

    template <class encType, class keyType>
    [[nodiscard]] static std::string encode(encType encrypt_me, keyType key, ull passes = DEFAULT_PASSES){
        return code<encType, keyType>(encrypt_me, key, false, passes);
    }

    template <class decType, class keyType>
    [[nodiscard]] static decType decode(std::string cipherText, keyType key, ull passes = DEFAULT_PASSES){
        return code<decType, keyType>(cipherText, key, true, passes);
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

void RyCipher::transform(evil_container &evil_type, size_t &decSize, bool removePadding, unsigned short blockSize, unsigned short blockWidth) {
    size_t vecSize = decSize;
    if(not removePadding && vecSize % blockSize != 0) {
        for (; vecSize % blockSize != 0; ++vecSize);
        //TODO: optimize this garbage, and run speed tests thereon.
        evil_type.emplace_back(PADDING_START);
        evil_type.resize(vecSize, PADDING_CONT);
    }
    evil_container klone(vecSize);
    for(int i = 0, currentBlockMax = blockSize, arrIndex = 0; i < vecSize && arrIndex < vecSize && currentBlockMax <= vecSize; ++i, currentBlockMax += blockSize){
        for(int microTickCounter{}; microTickCounter < blockWidth; microTickCounter++)
        for(int hop = currentBlockMax - blockSize + microTickCounter; hop < currentBlockMax; hop += blockWidth){
            klone[hop] = evil_type[arrIndex];
            arrIndex++;
        }
    }

}

template<class encType, class keyType>
encType RyCipher::code(encType encrypt_me, keyType key, bool decode, unsigned long long passes) {
    evil_container evil_type;
    evil_container evil_key;
    auto carrier_of_evil = makeSenseOfThis(encrypt_me, key);

    evil_type = get<0>(carrier_of_evil);
    evil_key = get<1>(carrier_of_evil);
    size_t decSize = evil_type.size(), keySize = evil_key.size();

    for (int i = 0; i < passes; ++i) {
    transform(evil_type, decSize, decode);
    Vigenere(evil_type.data(), evil_key.data(), decSize, keySize, decode);
    }
    if(decode) TidyUp(evil_type, decSize);

    if constexpr (PODType<encType> || NoneOfTheAbove<encType>){
        return reinterpret_cast<encType>(evil_type.data());
    } else if constexpr(std::is_same<encType, std::string>()) {
        return {reinterpret_cast<const char *>(evil_type.data()), decSize};
    } else if constexpr(HasDataFunction<encType>) {
        return {reinterpret_cast<encType>(evil_type.data()), decSize};
    }
}

void RyCipher::TidyUp(evil_container &evil_type, size_t &decSize) {
    auto staging_post = evil_type.begin();
    unsigned short timeout{};
    while(staging_post != evil_type.end()) {
        staging_post = std::find_if(staging_post, evil_type.end(), [](unsigned char x) {
            return x == PADDING_START;
        });
        if(staging_post + 1 == evil_type.end()){
            evil_type.pop_back();
            break;
        }
        if( not (staging_post == evil_type.end()) && not (staging_post + 1 == evil_type.end()) && *(staging_post + 1) == PADDING_CONT ) {
            evil_type.erase(staging_post, evil_type.end());
            break;
        }
        if( not (staging_post == evil_type.end()) && not (staging_post + 1 == evil_type.end()) && *(staging_post +1) == PADDING_START) staging_post++;
        ++timeout;
        if(timeout > TIMEOUT_MAX) throw std::runtime_error(("Timeout in TidyUp with "s + reinterpret_cast<char *>(evil_type.data())));
    }
    decSize = evil_type.size();
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
