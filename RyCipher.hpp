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

class RyCipher {
    static constexpr unsigned char VIG_OFFSET = 'a';

    template <class Type, class keyType>
    [[nodiscard]] static Type code(Type encrypt_me, keyType key, bool decode);

    static unsigned char * Vigenere(unsigned char *evil_type, const unsigned char *evil_key, const size_t decSize, const size_t keySize, bool reverse);

    static unsigned char * transform(unsigned char *evil_type, const size_t decSize, bool removePadding);

    template <class Type, class keyType>
    static std::tuple<void*,size_t,void*,size_t> makeSenseOfThis(Type encrypt_me, keyType key);


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

unsigned char *RyCipher::transform(unsigned char *evil_type, const size_t decSize, bool removePadding) {
    const int true_N = std::ceil(std::sqrt(decSize));
    const int true_N_sq = true_N * true_N;

    return nullptr;
}

template<class encType, class keyType>
encType RyCipher::code(encType encrypt_me, keyType key, bool decode) {
    unsigned char *evil_type{};
    unsigned char *evil_key{};
    auto carrier_of_evil = makeSenseOfThis(encrypt_me, key);

    evil_type = static_cast<unsigned char *>(get<0>(carrier_of_evil));
    evil_key = static_cast<unsigned char *>(get<2>(carrier_of_evil));
    const size_t decSize = get<1>(carrier_of_evil), keySize = get<3>(carrier_of_evil);

    evil_type = Vigenere(evil_type, evil_key, decSize, keySize, decode);

    //evil_type = transform(evil_type, decSize);

    if constexpr (PODType<encType> || NoneOfTheAbove<encType>){
        return reinterpret_cast<encType>(evil_type);
    } else if constexpr(std::is_same<encType, std::string>()) {
        return {reinterpret_cast<const char *>(evil_type), decSize};
    } else if constexpr(HasDataFunction<encType>) {
        return {reinterpret_cast<encType>(evil_type), decSize};
    }
}

template<class Type, class keyType>
std::tuple<void*,size_t,void*,size_t> RyCipher::makeSenseOfThis(Type encrypt_me, keyType key) {
    std::tuple<void*,size_t,void*,size_t> true_evil{};
    //Handle the Text Type first
    if constexpr (PODType<Type> || NoneOfTheAbove<Type>){
        std::get<0>(true_evil) = static_cast<void *>(encrypt_me);
        std::get<1>(true_evil) = sizeof(encrypt_me);
    } else if constexpr (HasDataFunction<Type>) {
        std::get<0>(true_evil) = static_cast<void *>(encrypt_me.data());
        std::get<1>(true_evil) = encrypt_me.size();
    }
    //Handle the Key Type
    if constexpr (PODType<Type> || NoneOfTheAbove<Type>){
        std::get<2>(true_evil) = static_cast<void *>(key);
        std::get<3>(true_evil) = sizeof(encrypt_me);
    } else if constexpr (HasDataFunction<Type>) {
        std::get<2>(true_evil) = static_cast<void *>(key.data());
        std::get<3>(true_evil) = encrypt_me.size();
    }

    return true_evil;
}


#endif //HASHMA_RYCIPHER_HPP
