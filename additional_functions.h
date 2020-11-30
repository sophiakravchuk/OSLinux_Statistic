#ifndef OS_STATISTIC_ADDITIONAL_FUNCTIONS_H
#define OS_STATISTIC_ADDITIONAL_FUNCTIONS_H

#include <string>
#include <vector>

bool is_number(const std::string& s);

std::vector<std::string> split(std::string s, const std::string& delimiter);

std::string double_to_string_precision(double number, int percision);

std::string memory_suitable_view(long mem_size);

std::string seconds_to_time(int seconds);

#endif //OS_STATISTIC_ADDITIONAL_FUNCTIONS_H
