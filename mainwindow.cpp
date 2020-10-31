#include <QTimer>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>

#include <boost/algorithm/string/trim.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additional_functions.h"
#include "parse_proc_dir.h"
#include "structures.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),

    ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        wind_timer = new QTimer(this);
        connect(wind_timer, &QTimer::timeout, this, &MainWindow::update_table);

        ui->tableWidget->verticalHeader()->setVisible(false);

        QObject::connect( ui->tableWidget->horizontalHeader(), SIGNAL( sectionClicked( int ) ), this, SLOT( on_myTable_sectionClicked( int ) ) );

    }


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_table() {
    int in = 0;

        all_tasks_info.clear();
        long mem_total = get_mem_total();
        for(auto &p : std::filesystem::directory_iterator("/proc/")){
            std::string file_path =p.path();
            struct task_manager_file_info finfo;
            std::vector<std::string> vec = split(file_path, "/");
            std::string filename = vec[vec.size()-1];
            if(!is_number(filename)){
                continue;
            }
            finfo.pid = stoi(filename);
            get_cpu_and_name(&finfo, file_path);
            get_memory_info(&finfo, file_path, mem_total);
            all_tasks_info.push_back(finfo);

        }


        /*add rows and columns*/
        ui->tableWidget->setRowCount(all_tasks_info.size());

        sort_table(&all_tasks_info);

        ui->tableWidget->clearContents();
        ui->tableWidget->update();
        /*add stuff inside the table view*/
            for(int i=0; i<ui->tableWidget->rowCount(); i++)
            {
                for(int j=0; j<6; j++)
                {
                    QTableWidgetItem *pCell = ui->tableWidget->item(i, j);
                    if(!pCell)
                    {
                        pCell = new QTableWidgetItem;
                        ui->tableWidget->setItem(i, j, pCell);
                    }
                }
                QTableWidgetItem *pCell0 = ui->tableWidget->item(i, 0);
                pCell0->setText(all_tasks_info[i].process_name.c_str());

                QTableWidgetItem *pCell1 = ui->tableWidget->item(i, 1);
                pCell1->setText(std::to_string(all_tasks_info[i].pid).c_str());

                QTableWidgetItem *pCell2 = ui->tableWidget->item(i, 2);
                std::string s = double_to_string_precision(all_tasks_info[i].cpu_usage, 1);
                pCell2->setText((s + '%').c_str());

                QTableWidgetItem *pCell3 = ui->tableWidget->item(i, 3);
                std::string vm_s = memory_suitable_view(all_tasks_info[i].vm_size);
                pCell3->setText(vm_s.c_str());

                QTableWidgetItem *pCell4 = ui->tableWidget->item(i, 4);
                std::string pss_s = memory_suitable_view(all_tasks_info[i].pss);
                pCell4->setText(pss_s.c_str());

                QTableWidgetItem *pCell5 = ui->tableWidget->item(i, 5);
                pCell5->setText((std::to_string(all_tasks_info[i].mem_percentage) + " %").c_str());

            }
            ui->tableWidget->update();
            wind_timer->start(1000);
            in++;

}

void MainWindow::on_myTable_sectionClicked(int index) {
    for (int i = 0; i < static_cast<int>(sort_flags.flags.size()); i++){
        if(i == index){
            sort_flags.flags[i] += 1;
            sort_flags.flags[i] %= 2;
        } else {
            sort_flags.flags[i] = -1;
        }
    }
}

void MainWindow::sort_table(std::vector<struct task_manager_file_info>* all_tasks_info) {


    if (sort_flags.flags[0] != -1) {
        if (!sort_flags.flags[0]) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    int size_of_words = finfo1.process_name.size() < finfo2.process_name.size() ? finfo1.process_name.size(): finfo2.process_name.size();
                    for(int i = 0; i < size_of_words; i++){
                        bool not_lowered = finfo1.process_name[i] < finfo2.process_name[i];
                        bool lowered1 = std::tolower(finfo1.process_name[i]) < std::tolower(finfo2.process_name[i]);
                        bool lowered2 = std::tolower(finfo2.process_name[i]) < std::tolower(finfo1.process_name[i]);
                        if (finfo1.process_name[i] == finfo2.process_name[i]){
                            continue;
                        } else if (lowered1 == lowered2){
                            return !not_lowered;
                        }else if (lowered1 != lowered2){
                            return lowered1;
                        }
                    }
                    return finfo1.process_name < finfo2.process_name;
                });
        }
        else {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    int size_of_words = finfo1.process_name.size() < finfo2.process_name.size() ? finfo1.process_name.size(): finfo2.process_name.size();
                    for(int i = 0; i < size_of_words; i++){
                        bool not_lowered = finfo1.process_name[i] > finfo2.process_name[i];
                        bool lowered1 = std::tolower(finfo1.process_name[i]) > std::tolower(finfo2.process_name[i]);
                        bool lowered2 = std::tolower(finfo2.process_name[i]) > std::tolower(finfo1.process_name[i]);
                        if (finfo1.process_name[i] == finfo2.process_name[i]){
                            continue;
                        } else if (lowered1 == lowered2){
                            return !not_lowered;
                        }else if (lowered1 != lowered2){
                            return lowered1;
                        }
                    }
                    return finfo1.process_name > finfo2.process_name;
                });
        }
    }

    else if (sort_flags.flags[1] != -1) {
        if (sort_flags.flags[1]) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.pid > finfo2.pid;
                });
            return;
        } else {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.pid < finfo2.pid;
            });
        }
    }

    else if (sort_flags.flags[2] != -1) {
        if (!sort_flags.flags[2]) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.cpu_usage > finfo2.cpu_usage;
                });
            return;
        }
        else {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.cpu_usage < finfo2.cpu_usage;
            });
        }
    }

    else if (sort_flags.flags[3]  != -1) {
        if (!sort_flags.flags[3]) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.vm_size > finfo2.vm_size;
                });
            return;
        } else {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.vm_size < finfo2.vm_size;
            });
        }
    }

    else if (sort_flags.flags[4]  != -1) {
        if (!sort_flags.flags[4] ) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.pss > finfo2.pss;
                });
            return;
        } else {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.pss < finfo2.pss;
            });
        }
    }

    else if (sort_flags.flags[5] != -1) {
        if (!sort_flags.flags[5]) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.mem_percentage > finfo2.mem_percentage;
                });
            return;
        } else {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.mem_percentage < finfo2.mem_percentage;
            });
        }
    }

    else {
        std::cerr << "Error in sort detection!!!" << std::endl;
    }
}


