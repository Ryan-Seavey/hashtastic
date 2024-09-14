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

using cstring = const std::string;
#define ss static cstring

class RyHash{
    //MAX_SENTENCE_BANK_SIZE
    static constexpr int MSBS = 9;

    ss places[];
    ss people[];
    ss adjectives[];
    ss prepositions[];
    ss verbs[];
    ss adverbs[];
    ss punctuation[];

    static std::mt19937_64 generator;
    static std::uniform_int_distribution<unsigned long long> distribution;

    static std::string sentence(unsigned long long);
    static std::string hashInt(unsigned long long x[], size_t max);

    template <class sizeCheck>
    static std::string hashObject(sizeCheck);
public:
    static std::string hashNormal(const std::string&);

    template <class arbitraryType>
    static std::string hash(arbitraryType a){return hashObject<arbitraryType>(a);}
};

std::mt19937_64 RyHash::generator{};
std::uniform_int_distribution<unsigned long long> RyHash::distribution{UINT64_MAX+1,UINT64_MAX};

cstring RyHash::places[] = {
        "School",
        "University",
        "Hell",
        "Store",
        "Classroom",
        "Bus",
        "Car",
        "Airplane",
        "JailCell"
};
cstring RyHash::people[] = {
        "Pete",
        "Dr.Sellers",
        "TomPetty",
        "Miguel",
        "Katie",
        "Carrot",
        "Adolf",
        "Johninator",
        "Timmy"
};
cstring RyHash::adjectives[] ={
        "Fat",
        "Ugly",
        "Silly",
        "Radical",
        "Eccentric",
        "Stinky",
        "Godly",
        "Unusual",
        "Suspicious",
};
cstring RyHash::prepositions[] = {
        "To",
        "From",
        "By",
        "ByWayOf",
        "Through",
        "Around",
        "About",
        "Out",
        "OutOf",
};
cstring RyHash::verbs[] = {
        "Walks",
        "Talks",
        "Barks",
        "Screams",
        "Perambulates",
        "PicksUp",
        "Drops",
        "Drives",
        "Lectures"
};
cstring RyHash::adverbs[] = {
        "Gaily",
        "Lethargically",
        "Quietly",
        "Unfortunately",
        "Cleverly",
        "Horribly",
        "Recklessly",
        "Casually",
        "Wonderfully"
};

cstring RyHash::punctuation[] = {
        ".",
        "!",
        "?",
        "!?",
        ";",
        ",",
        "~",
        "...",
        "..?"
};

std::string RyHash::hashNormal(const std::string& x) {
    unsigned long long newOrleans = x.length();
    unsigned long long meat{};
    for(int z = 0; z < x.length(); z++){
        meat += static_cast<unsigned long long>((static_cast<int>(x[z])) * (std::numbers::e * ((z + 0.1)*10)));
        meat ^= static_cast<unsigned long long>(std::tgamma((static_cast<int>(x[z]))));
    }
    for(int i = 10; i != 0; --i){
        if(newOrleans % i == 0) newOrleans ^= static_cast<int>(std::pow<unsigned long long,int>(newOrleans, i));
    }

    return sentence(meat ^ newOrleans);
}

template <class sizeCheck>
std::string RyHash::hashObject(sizeCheck yummy) {
    unsigned char * yes = static_cast<unsigned char *>(static_cast<void *>(&yummy));
    size_t max = sizeof(sizeCheck);
    unsigned long long wrapper[max];
    for(int x = 0; x < max; ++x){
        wrapper[x] = static_cast<unsigned long long>(yes[x]);
    }
    return hashInt(wrapper, max);
}

std::string RyHash::hashInt(unsigned long long x[], size_t max) {
    unsigned long long newOrleans = max;
    unsigned long long meat{};
    for(int z = 0; z < max; z++){
        meat += static_cast<unsigned long long>((static_cast<int>(x[z])) * (std::numbers::e * ((z + 0.1)*10)));
        meat ^= static_cast<unsigned long long>(std::tgamma((static_cast<int>(x[z]))));
    }
    for(int i = 10; i != 0; --i){
        if(newOrleans % i == 0) newOrleans ^= static_cast<int>(std::pow<unsigned long long,int>(newOrleans, i));
    }

    return sentence(meat ^ newOrleans);
}

std::string RyHash::sentence(unsigned long long key) {
    generator.seed(key);
    key = distribution(generator);
#define calc(n) (std::rotl(key, n) % MSBS % 10)
    unsigned long long z = key % 100;
    std::string superString{adjectives[calc(z)]};
    z++;
    superString +=  people[calc(z)];
    z++;
    superString += adverbs[calc(z)];
    z++;
    superString += verbs[calc(z)];
    z-=10;
    superString += prepositions[calc(z)];
    z-=20;
    superString += places[calc(z)];
    z-=30;
    superString += punctuation[calc(z)];
#undef calc
    return superString;
}
