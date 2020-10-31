#ifndef OS_STATISTIC_STRUCTURES_H
#define OS_STATISTIC_STRUCTURES_H

#include <string>
#include <vector>

struct sorting_info {
    std::vector<int> flags;
};

struct task_manager_file_info {
    int pid;
    std::string process_name;
    double cpu_usage;
    long mem_percentage;
    long pss;
    long vm_size;
};

#endif //OS_STATISTIC_STRUCTURES_H
