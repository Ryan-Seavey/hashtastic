//
// Created by ryanseavey on 9/12/24.
//
export module RyHash;

import <string>;

using cstring = const std::string;
#define ss static cstring

export class RyHash{
    ss places[];
    ss people[];
    ss adjectives[];
    ss prepositions[];
    ss punctuation[];
    ss adverbs[];


};

cstring RyHash::places[] = {
        "school",
        "university",
        ""
};
cstring RyHash::people[] = {};
cstring RyHash::adjectives[] ={};
cstring RyHash::prepositions[] = {};
cstring RyHash::punctuation[] = {};
cstring RyHash::adverbs[] = {};