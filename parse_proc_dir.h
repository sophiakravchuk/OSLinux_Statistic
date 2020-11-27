#ifndef OS_STATISTIC_PARSE_PROC_DIR_H
#define OS_STATISTIC_PARSE_PROC_DIR_H

#include <string>

double get_cpu_Hz();

long get_mem_total();

void get_cpu_and_name(struct task_manager_file_info* finfo,std::string file_path);

void get_memory_info(struct task_manager_file_info* finfo, std::string file_path, long mem_total);

void get_proc_state(struct task_manager_file_info* finfo, std::string file_path);

#endif //OS_STATISTIC_PARSE_PROC_DIR_H
