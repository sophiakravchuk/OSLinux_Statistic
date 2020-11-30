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

std::string seconds_to_time(int seconds){
    int m = 0;
    int h = 0;
    int d = 0;
    if(seconds >= 60){
        m = seconds / 60;
        seconds -= m*60;
    }
    if(m >= 60){
        h = m / 60;
        m -= h*60;
    }
    if(h >= 24){
        d = h / 24;
        h -= d*24;
    }
    std::string days = std::to_string(d);

    std::string hours = "";
    if (h < 10){
        hours = "0";
    }
    hours += std::to_string(h);

    std::string minutes = "";
    if (m < 10){
        minutes = "0";
    }
    minutes += std::to_string(m);

    std::string seconds_s = "";
    if (seconds < 10){
        seconds_s = "0";
    }
    seconds_s += std::to_string(seconds);
    std::string time = days + ":" + hours  + ":" + minutes + ":" + seconds_s;
    return time;
}



