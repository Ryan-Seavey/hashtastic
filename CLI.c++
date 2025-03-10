//
// Created by diogenes on 3/7/25.
//

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include "RyHash.h++"

namespace opt = boost::program_options;

static constexpr std::string_view help_text{
    "Yeah, I am the best hash to ever exist, what of it?\n"
    "Usage: [OPTION]... [FILE]...\n"
    "Allowed Options:\b"
};

int main(int argc, char ** argv){
    opt::options_description desc(help_text.data());

    std::string strinput;
    std::vector<std::string> fpinput(1);
    unsigned requested_bits;

    desc.add_options()
    ("help,h", "")
    ("file,f", opt::value<std::vector<std::string>>(&fpinput),"The path of the file(s) you wish to hash")
    ("bits,b", opt::value<unsigned>(&requested_bits)->default_value(32), "The number of bits you would like to use.")
    ("string,message,s" ,opt::value<std::string>(&strinput),"A string you would like to hash.")

    ;

    opt::positional_options_description paths;
    paths.add("file", -1);

    opt::variables_map vm;
    store(opt::command_line_parser(argc, argv).options(desc).positional(paths).run(), vm);
    opt::notify(vm);

    if (vm.contains("help") || argc == 1) std::cout << desc;

    if (vm.contains("string")) std::cout << "Here's stringinput!\n" << vm["string"].as<std::string>() << '\n';

    std::cout << std::hex << RyHash::hashTime(RyHash::file_to_bytes(vm["f"].as<std::string>()), requested_bits) << '\n';


    return 0;
}