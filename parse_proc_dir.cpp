#include <vector>
#include <map>

#include <boost/algorithm/string/trim.hpp>

#include "headers/parse_proc_dir.h"
#include "headers/reading_from_file.h"
#include "headers/additional_functions.h"
#include "headers/structures.h"

double get_cpu_Hz() {
    /**
    * @brief This function reads from /proc and returns CPU frequency in Hz 
    */
    std::vector<std::string> key_words_cpu = {"cpu MHz"};
    std::map<std::string, std::string> values;
    get_values_from_file(&values, key_words_cpu, "/proc/cpuinfo");
    std::string cpu_MHz_str = values["cpu MHz"];
    double cpu_MHz = stod(cpu_MHz_str);
    double cpu_Hz = cpu_MHz*1000000;
    return cpu_Hz;
}

long get_mem_total() {
    /**
    * @brief This function reads from /proc and returns total amount of memory 
    */
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
    /**
    * @brief This function reads from file in /proc dir and save cpu and name
    * to task_manager_file_info structure
    * The function has parameters as follows:
    *
    * @param finfo
    * This parameter is task_manager_file_info structure instance where the data will be stored
    * 
    * @param file_path
    * This parameter is a std::string of file from which data will be taken.
    */
    std::vector<std::string> uptime_vec = read_file_to_vector("/proc/uptime");
    int uptime = 0;
    if (!uptime_vec.empty()){
        uptime = stoi(uptime_vec[0]);
    }
    std::vector<std::string> stat_vec = read_file_to_vector(file_path + "/stat");
    double clocks =sysconf(_SC_CLK_TCK);
    int shift = 0;
    if (stat_vec.empty()){
        std::string process_name = "";
        return;
    }
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

std::map<std::string, std::string> get_constant_info(){
    /**
    * @brief This function reads from /proc/cpuinfo and returns a map with data 
    * about cpu cores and cpu model
    */
    std::vector<std::string> key_words = {"cpu cores", "model name"};
    std::map<std::string, std::string> cpuinfo_key_map;
    get_values_from_file(&cpuinfo_key_map, key_words, "/proc/cpuinfo");
    return cpuinfo_key_map;
}


void get_memory_info(struct task_manager_file_info* finfo, std::string file_path, long mem_total){
    /**
    * @brief This function reads from file in /proc dir and save memory data 
    * to task_manager_file_info structure
    * The function has parameters as follows:
    *
    * @param finfo
    * This parameter is task_manager_file_info structure instance where the data will be stored
    * 
    * @param file_path
    * This parameter is a std::string of file from which data will be taken.
    * 
    * @param mem_total
    * This parameter is a long integer of total memory.
    */
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
    /**
    * @brief This function reads from file in /proc dir and save proc state
    * to task_manager_file_info structure
    * The function has parameters as follows:
    *
    * @param finfo
    * This parameter is task_manager_file_info structure instance where the data will be stored
    * 
    * @param file_path
    * This parameter is a std::string of file from which data will be taken.
    */
    std::vector<std::string> key_words = {"State"};
    std::map<std::string, std::string> mem_keys_map;
    get_values_from_file(&mem_keys_map, key_words, file_path + "/status");
   boost::algorithm::trim(mem_keys_map["State"]);
    finfo->proc_state =  mem_keys_map["State"];
}
