#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

#include "additional_functions.h"


bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

std::vector<std::string> split(std::string s, const std::string& delimiter) {
    size_t pos = 0;
    std::string token;
    std::vector<std::string> words;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if (!token.empty()) {
            words.push_back(token);
        }
        s.erase(0, pos + delimiter.length());
    }
    if (!s.empty()){
        words.push_back(s);
    }
    return words;
}

std::string double_to_string_precision(double number, int percision){
    std::string s;
    std::stringstream sstream;
    sstream.setf(std::ios::fixed);
    sstream.precision(percision);
    sstream << number;
    s = sstream.str();
    return s;
}


std::string memory_suitable_view(long mem_size){
    long double mem_size_d = (long double) mem_size;
    int div_count = 0;
    while (mem_size_d > 1000){
        if(div_count == 2){
            break;
        }
        div_count++;
        mem_size_d /= 1024;
    }
    std::string s = double_to_string_precision(mem_size_d, 1);
    if(div_count == 0){
       s += " Kb";
    } else if(div_count == 1){
       s += " Mb";
    } else if(div_count == 2){
        s += " Gb";
    }
    return s;
}


