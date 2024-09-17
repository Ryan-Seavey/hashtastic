//
// Created by ryanseavey on 9/12/24.
//
//export module RyHash;

#include <string>
#include <cmath>
#include <bit>
#include <numbers>
#include <random>
#include <array>
#include <concepts>

using cstring = const std::string;
using cvec = std::vector<std::vector<std::string>>;
#define ss static cstring

template <typename T>
concept HasDataFunction = requires(T t) {
    { t.data() } -> std::convertible_to<const void*>;
    { t.length() } -> std::convertible_to<size_t const>;
};

class RyHash{
    //MAX_SENTENCE_BANK_SIZE
    static const int MSBS;
    enum{
        places,
        people,
        adjectives,
        prepositions,
        verbs,
        adverbs,
        punctuation,
    };
    static cvec dictionary;


    static std::mt19937_64 generator;
    static std::uniform_int_distribution<unsigned long long> distribution;

    static std::string sentence(unsigned long long);
    static std::string hashInt(std::vector<unsigned long long> x, size_t max);

    //Hasher for all plain-old-data structures and atomics and the like.
    template <class basicType>
    static std::string hashObject(basicType) requires std::semiregular<basicType> && (not HasDataFunction<basicType>);

    //Hasher for all complex items which have a .data() function for accessing the POD.
    template <HasDataFunction complexTypeWithData>
    static std::string hashObject(complexTypeWithData) requires HasDataFunction<complexTypeWithData>;

    //Hasher for everything else. Nothing is guaranteed.
    template <class arbitraryType>
    static std::string hashObject(arbitraryType) requires (not (std::semiregular<arbitraryType> || HasDataFunction<arbitraryType>));

    static std::string hashNormal(const std::string&);
public:

    template <class arbitraryType>
    static std::string hash(arbitraryType a){return hashObject<arbitraryType>(a);}
};

std::mt19937_64 RyHash::generator{};
std::uniform_int_distribution<unsigned long long> RyHash::distribution{UINT64_MAX+1,UINT64_MAX};

cvec RyHash::dictionary{
        { //Adjectives
                "Fat",
                "Ugly",
                "Silly",
                "Radical",
                "Eccentric",
                "Stinky",
                "Godly",
                "Unusual",
                "Suspicious",
        },
        { //People
                "Pete",
                "Dr.Sellers",
                "TomPetty",
                "Miguel",
                "Katie",
                "Carrot",
                "Parker",
                "Johninator",
                "Timmy"
        },
        { //Adverbs
                "Gaily",
                "Lethargically",
                "Quietly",
                "Unfortunately",
                "Cleverly",
                "Horribly",
                "Recklessly",
                "Casually",
                "Wonderfully"
        },
        { //Verbs
                "Walks",
                "Talks",
                "Barks",
                "Screams",
                "Perambulates",
                "PicksUp",
                "Drops",
                "Drives",
                "Lectures"
        },
        { //Prepositions
                "To",
                "From",
                "By",
                "ByWayOf",
                "Through",
                "Around",
                "About",
                "Out",
                "OutOf",
        },
        { //Places
                "School",
                "University",
                "Hell",
                "Store",
                "Classroom",
                "Bus",
                "Car",
                "Airplane",
                "JailCell"
        },
        {//Punctuation
                ".",
                "!",
                "?",
                "!?",
                ";",
                ",",
                "~",
                "...",
                "..?"
        }
};


std::string RyHash::hashNormal(const std::string& x) {
    unsigned long long newOrleans = x.length();
    unsigned long long meat{};
    for(size_t z = 0; z < x.length(); z++){
        meat += static_cast<unsigned long long>((static_cast<float>(x[z])) * (std::numbers::e_v<float> * ((static_cast<float>(z) + 0.1f)*10.0f)));
        meat ^= static_cast<unsigned long long>(std::tgamma((static_cast<int>(x[z]))));
    }
    for(int i = 10; i != 0; --i){
        if(newOrleans % static_cast<unsigned long long>(i) == 0) newOrleans ^= static_cast<unsigned long long>(std::pow<unsigned long long,int>(newOrleans, i));
    }

    return sentence(meat ^ newOrleans);
}

template <class basicType>
std::string RyHash::hashObject(basicType yummy) requires std::semiregular<basicType> && (not HasDataFunction<basicType>) {
    unsigned char * yes = static_cast<unsigned char *>(static_cast<void *>(&yummy));
    size_t max = sizeof(basicType);
    std::vector<unsigned long long> wrapper(max);
    for(size_t x = 0; x < max; ++x){
        wrapper[x] = static_cast<unsigned long long>(yes[x]);
    }
    return hashInt(wrapper, max);
}

template <HasDataFunction complexTypeWithData>
std::string RyHash::hashObject(complexTypeWithData yummy) requires HasDataFunction<complexTypeWithData>{
    unsigned char * yes = static_cast<unsigned char *>(static_cast<void *>(yummy.data()));
    size_t max = yummy.size();
    std::vector<unsigned long long> wrapper(max);
    for(size_t x = 0; x < max; ++x){
        wrapper[x] = static_cast<unsigned long long>(yes[x]);
    }
    return hashInt(wrapper, max);
}


template <class arbitraryType>
std::string RyHash::hashObject(arbitraryType yummy) requires (not (std::semiregular<arbitraryType> || HasDataFunction<arbitraryType>)){
    unsigned char * yes = static_cast<unsigned char *>(static_cast<void *>(&yummy));
    size_t max = sizeof(arbitraryType);
    std::vector<unsigned long long> wrapper(max);
    for(size_t x = 0; x < max; ++x){
        wrapper[x] = static_cast<unsigned long long>(yes[x]);
    }
    return hashInt(wrapper, max);
}

std::string RyHash::hashInt(std::vector<unsigned long long> x, size_t max) {
    unsigned long long newOrleans = max;
    unsigned long long meat{};
    for(size_t z = 0; z < max; z++){
        meat += static_cast<unsigned long long>((static_cast<float>(x[z])) * (std::numbers::e_v<float> * ((static_cast<float>(z) + 0.1f)*10.0f)));
        meat ^= static_cast<unsigned long long>(std::tgamma((static_cast<int>(x[z]))));
    }
    for(int i = 10; i != 0; --i){
        if(newOrleans % static_cast<unsigned long long>(i) == 0) newOrleans ^= static_cast<unsigned long long>(std::pow<unsigned long long,int>(newOrleans, i));
    }

    return sentence(meat ^ newOrleans);
}

std::string RyHash::sentence(unsigned long long key) {
    generator.seed(key);
    key = distribution(generator);
#define calc(n,l) (std::rotl(key, static_cast<int>(n)) % l % 10)
    unsigned long long z = key % 100;
    std::string superString;
    for (const auto& chapter : RyHash::dictionary) {
        superString += chapter.at(calc(z,chapter.size()));
    }
#undef calc
    return superString;
}
