#include <vector>
#include <map>

#include <boost/algorithm/string/trim.hpp>

#include "parse_proc_dir.h"
#include "reading_from_file.h"
#include "additional_functions.h"
#include "structures.h"

double get_cpu_Hz() {
    std::vector<std::string> key_words_cpu = {"cpu MHz"};
    std::map<std::string, std::string> values;
    get_values_from_file(&values, key_words_cpu, "/proc/cpuinfo");
    std::string cpu_MHz_str = values["cpu MHz"];
    double cpu_MHz = stod(cpu_MHz_str);
    double cpu_Hz = cpu_MHz*1000000;
    return cpu_Hz;
}

long get_mem_total() {
    std::vector<std::string> key_word = {"MemTotal"};
    std::map<std::string, std::string> mem_key_map;
    get_values_from_file(&mem_key_map, key_word, "/proc/meminfo");

    mem_key_map[key_word[0]].pop_back();
    mem_key_map[key_word[0]].pop_back();
    boost::algorithm::trim(mem_key_map[key_word[0]]);
    if(!is_number(mem_key_map[key_word[0]])){
        std::cerr <<"Error!!!";

    }
    return stol(mem_key_map[key_word[0]]);
}

void get_cpu_and_name(struct task_manager_file_info* finfo,std::string file_path){
    std::vector<std::string> uptime_vec = read_file_to_vector("/proc/uptime");
    int uptime = stod(uptime_vec[0]);
    std::vector<std::string> stat_vec = read_file_to_vector(file_path + "/stat");
    double clocks =sysconf(_SC_CLK_TCK);
    int shift = 0;
    std::string process_name = stat_vec[1];
    if (process_name[0] != '('){
        std::cerr << "error";
    }
    while(process_name[process_name.size() - 1] != ')'){
        shift ++;
        process_name += stat_vec[1+shift];
    }
    boost::algorithm::trim(process_name);
    process_name.pop_back();
    process_name.erase(0, 1);
    finfo->process_name = process_name;

    int utime = stoi(stat_vec[13+shift]);
    int stime = stoi(stat_vec[14+shift]);
//    int cutime = stoi(stat_vec[15+shift]);
//    int cstime = stoi(stat_vec[16+shift]);
    int starttime = stoi(stat_vec[21+shift]);
    int total_time = utime + stime;
//    total_time = total_time + cutime + cstime;    adding children's cpu usage
    double seconds = uptime - (starttime / clocks);
    finfo->cpu_usage = 100 * ((total_time / clocks) / seconds);
}


void get_memory_info(struct task_manager_file_info* finfo, std::string file_path, long mem_total){
    std::vector<std::string> key_words = {"Pss", "VmSize"};
    std::vector<std::string> smaps_key_words = {"Pss"};
    std::vector<std::string> status_key_words = {"VmSize"};
    std::map<std::string, std::string> mem_keys_map;
    get_values_from_file(&mem_keys_map, smaps_key_words, file_path + "/smaps_rollup");
    get_values_from_file(&mem_keys_map, status_key_words, file_path + "/status");
    for(std::string key_word: key_words){
        if ( mem_keys_map.find(key_word) == mem_keys_map.end() ) {
            mem_keys_map[key_word] = "0";
        } else {
            mem_keys_map[key_word].pop_back();
            mem_keys_map[key_word].pop_back();
            boost::algorithm::trim(mem_keys_map[key_word]);
            if(!is_number(mem_keys_map[key_word])){
                std::cerr <<"Error!!!";
            }
        }


    }
    finfo->pss = stol(mem_keys_map["Pss"]);
    finfo->vm_size = stol(mem_keys_map["VmSize"]);
    finfo->mem_percentage = 100 * finfo->pss/mem_total;

}


void get_proc_state(struct task_manager_file_info* finfo, std::string file_path){
    std::vector<std::string> key_words = {"State"};
    std::map<std::string, std::string> mem_keys_map;
    get_values_from_file(&mem_keys_map, key_words, file_path + "/status");
   boost::algorithm::trim(mem_keys_map["State"]);
    finfo->proc_state =  mem_keys_map["State"];
}
