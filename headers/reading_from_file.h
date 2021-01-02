#ifndef OS_STATISTIC_READING_FROM_FILE_H
#define OS_STATISTIC_READING_FROM_FILE_H

#include <iostream>
#include <vector>
#include <map>

void get_values_from_file(std::map<std::string, std::string>* values,std::vector<std::string> key_words,
                          const std::string& path_to_file);

std::vector<std::string> read_file_to_vector(const std::string& path_to_file);

#endif //OS_STATISTIC_READING_FROM_FILE_H
