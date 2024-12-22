#include "GetTypes.hpp"
#include <regex>
#include <string>

#define pflag          "--p-type="
#define vflag          "--v-type="
#define vfflag         "--vf-type="
#define INPUT          "--input=(.*)"
#define OUTPUT         "--output=(.*)"
#define INNER          "\\([ ]*(\\d+),[ ]*(\\d+)[ ]*\\)"
#define REG_DOUBLE     "DOUBLE"
#define REG_FLOAT      "FLOAT"
#define REG_FIXED      "FIXED"
#define REG_FAST_FIXED "FAST_FIXED"

bool matchtype(std::string flag_type, std::string& str, int& dest){
    std::regex expr1(flag_type + REG_DOUBLE);
    std::regex expr2(flag_type + REG_FLOAT);
    std::regex expr3(flag_type + REG_FAST_FIXED + INNER);
    std::regex expr4(flag_type + REG_FIXED + INNER);
    std::smatch result;
    bool success = false;

    success += regex_search(str, result, expr1);
    if (success) {
        if (dest != DEFAULT){std::cout << "You cannot use more than one equal flag\n";exit(-1);}
        dest = DOUBLE;
        return true;
    }

    success += regex_search(str, result, expr2);
    if (success) {
        if (dest != DEFAULT){std::cout << "You cannot use more than one equal flag\n";exit(-1);}
        dest = FLOAT;
        return true;
    }

    success += regex_search(str, result, expr3);
    if (success) {
        if (dest != DEFAULT){std::cout << "You cannot use more than one equal flag\n";exit(-1);}
        dest = FAST_FIXED(stoi(result[1]), stoi(result[2]));
        return true;
    }

    success += regex_search(str, result, expr4);
    if (success) {
        if (dest != DEFAULT){std::cout << "You cannot use more than one equal flag\n";exit(-1);}
        dest = FIXED(stoi(result[1]), stoi(result[2]));
        return true;
    }

    return false;
}

bool matchfile(std::string flag_type, std::string& src, std::string& dest){
    std::regex expr(flag_type);
    std::smatch result;
    if (regex_search(src, result, expr)) {
        dest = result[1];
        return true;
    }
    return false;
}

void ParseTypes::parseFlag(std::string& flag){
    if (matchtype(pflag,  flag, p)) {return;}
    if (matchtype(vflag,  flag, v)) {return;}
    if (matchtype(vfflag, flag, vf)) {return;}
    if (matchfile(INPUT,  flag, inputfile)) {return;}
    if (matchfile(OUTPUT, flag, outputfile)) {return;}
    std::cout << flag << ": No such flag\n";
    exit(-1);
}

ParseTypes::ParseTypes(int argc, char *argv[]){
    for (int i = 1; i < argc; ++i){
        std::string tmp(argv[i]);
        parseFlag(tmp);
    }
    if (p == DEFAULT || v == DEFAULT || vf == DEFAULT){
        std::cout << "Not enough types are given" << std::endl;
        exit(-1);
    }
}
