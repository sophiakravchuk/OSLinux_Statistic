#include <QContextMenuEvent>
#include <iostream>
#include <stdio.h>

#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/additional_functions.h"


void MainWindow::on_myTable_sectionClicked(int index) {
    /**
    * @brief This function checks index and based on it indicated how to sort
    * table with data
    * The function has parameters as follows:
    *
    * @param index
    * This parameter is a integer that is used to choose sorting
    */
    switch (index) {
        case (0): {
            sort_flags.name += 1;
            sort_flags.name %= 2;
            sort_flags.cpu_u = -1;
            sort_flags.state = -1;
            sort_flags.mem_p = -1;
            sort_flags.pid = -1;
            sort_flags.pss = -1;
            sort_flags.virt = -1;
            break;
        }
        case (1): {
            sort_flags.pid += 1;
            sort_flags.pid %= 2;
            sort_flags.cpu_u = -1;
            sort_flags.state = -1;
            sort_flags.mem_p = -1;
            sort_flags.name = -1;
            sort_flags.pss = -1;
            sort_flags.virt = -1;
            break;
        }

        case (2): {
            sort_flags.state += 1;
            sort_flags.state %= 2;
            sort_flags.pid = -1;
            sort_flags.cpu_u= -1;
            sort_flags.mem_p = -1;
            sort_flags.name = -1;
            sort_flags.pss = -1;
            sort_flags.virt = -1;
            break;
        }

        case (3): {
            sort_flags.cpu_u += 1;
            sort_flags.cpu_u %= 2;
            sort_flags.pid = -1;
            sort_flags.mem_p = -1;
            sort_flags.name = -1;
            sort_flags.pss = -1;
            sort_flags.virt = -1;
            sort_flags.state = -1;
            break;
        }

        case (4): {
            sort_flags.virt += 1;
            sort_flags.virt %= 2;
            sort_flags.pid = -1;
            sort_flags.mem_p = -1;
            sort_flags.name = -1;
            sort_flags.pss = -1;
            sort_flags.cpu_u = -1;
            sort_flags.state = -1;
            break;
        }

        case (5): {
            sort_flags.pss += 1;
            sort_flags.pss %= 2;
            sort_flags.pid = -1;
            sort_flags.mem_p = -1;
            sort_flags.name = -1;
            sort_flags.virt = -1;
            sort_flags.cpu_u = -1;
            sort_flags.state = -1;
            break;
        }
        case (6): {
            sort_flags.mem_p += 1;
            sort_flags.mem_p %= 2;
            sort_flags.pid = -1;
            sort_flags.pss = -1;
            sort_flags.name = -1;
            sort_flags.virt = -1;
            sort_flags.cpu_u = -1;
            sort_flags.state = -1;
            break;
        }
    }
}


void MainWindow::sort_table(std::vector<struct task_manager_file_info>* all_tasks_info) {
    /**
    * @brief This function sorts table with data.
    * The function has parameters as follows:
    *
    * @param all_tasks_info
    * This parameter is a vector of task_manager_file_info that contains
    * all the rows data of the table
    */
    if (sort_flags.name != -1) {
        if (sort_flags.name == 1) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    int size_of_words = finfo1.process_name.size() < finfo2.process_name.size() ? finfo1.process_name.size(): finfo2.process_name.size();
                    for(int i = 0; i < size_of_words; i++){
                        bool not_lowered = finfo1.process_name[i] > finfo2.process_name[i];
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
                    return finfo1.process_name > finfo2.process_name;
                });
        }
        else if (sort_flags.name == 0) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    int size_of_words = finfo1.process_name.size() < finfo2.process_name.size() ? finfo1.process_name.size(): finfo2.process_name.size();
                    for(int i = 0; i < size_of_words; i++){
                        bool not_lowered = finfo1.process_name[i] < finfo2.process_name[i];
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
                    return finfo1.process_name < finfo2.process_name;
                });
        }
    }

    else if (sort_flags.pid != -1) {
        if (sort_flags.pid == 1) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.pid > finfo2.pid;
                });
            return;
        }
        else if (sort_flags.pid == 0) {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.pid < finfo2.pid;
            });
        }
    }

    else if (sort_flags.state != -1) {
        if (sort_flags.state) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.proc_state > finfo2.proc_state;
                });
            return;
        } else {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.proc_state < finfo2.proc_state;
            });
        }
    }

    else if (sort_flags.cpu_u != -1) {
        if (sort_flags.cpu_u == 1) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.cpu_usage < finfo2.cpu_usage;
                });
            return;
        }
        else if (sort_flags.cpu_u == 0) {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.cpu_usage > finfo2.cpu_usage;
            });
        }
    }

    else if (sort_flags.virt != -1) {
        if (sort_flags.virt == 1) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.vm_size < finfo2.vm_size;
                });
            return;
        }
        else if (sort_flags.virt == 0) {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.vm_size > finfo2.vm_size;
            });
        }
    }

    else if (sort_flags.pss != -1) {
        if (sort_flags.pss == 1) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.pss < finfo2.pss;
                });
            return;
        }
        else if (sort_flags.pss == 0) {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.pss > finfo2.pss;
            });
        }
    }

    else if (sort_flags.mem_p != -1) {
        if (sort_flags.mem_p == 1) {
            std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                    return finfo1.mem_percentage < finfo2.mem_percentage;
                });
            return;
        }
        else if (sort_flags.mem_p == 0) {
        std::sort(all_tasks_info->begin(), all_tasks_info->end(), [](struct task_manager_file_info finfo1, struct task_manager_file_info finfo2) {
                return finfo1.mem_percentage > finfo2.mem_percentage;
            });
        }
    }

    else {
        std::cerr << "Error in sort detection!!!" << std::endl;
        return;
    }
}


void MainWindow::contextMenuEvent( QContextMenuEvent * e ) {
    /**
    * @brief This function is called on right click and opens
    * a small manu.
    * The function has parameters as follows:
    *
    * @param e
    * This parameter is QContextMenuEvent
    */
    if(updating_t){
        return;
    }
    int row = ui->tableWidget->selectionModel()->currentIndex().row();
    if (row == -1){
        return;
    }
    process_right_clicked = all_tasks_info[row].process_name;
    pid_right_clicked = all_tasks_info[row].pid;
    QAction *pKillProc = new QAction("Kill process", this);
    connect(pKillProc,SIGNAL(triggered()),this,SLOT(slotKill()));

    QAction *pGraphProc = new QAction("Graph process", this);
    connect(pGraphProc,SIGNAL(triggered()),this,SLOT(slotGraph()));

    QMenu *pContextMenu = new QMenu( this);
    pContextMenu->addAction(pKillProc);
    pContextMenu->addAction(pGraphProc);

    if(updating_t){
        delete pContextMenu;
        pContextMenu = NULL;
        return;
    }
    pContextMenu->exec( e->globalPos() );

    delete pContextMenu;
    pContextMenu = NULL;
}


void MainWindow::update_table() {
    /**
    * @brief This function updates table with data.
    * The function has parameters as follows:
    */
        /*add rows and columns*/
        ui->tableWidget->setRowCount(all_tasks_info.size());

        sort_table(&all_tasks_info);

        ui->tableWidget->clearContents();
        ui->tableWidget->update();
        /*add stuff inside the table view*/
            for(int i=0; i<ui->tableWidget->rowCount(); i++)
            {
                for(int j=0; j<7; j++)
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
                pCell2->setText(all_tasks_info[i].proc_state.c_str());

                QTableWidgetItem *pCell3 = ui->tableWidget->item(i, 3);
                std::string s = double_to_string_precision(all_tasks_info[i].cpu_usage, 1);
                pCell3->setText((s + '%').c_str());

                QTableWidgetItem *pCell4 = ui->tableWidget->item(i, 4);
                std::string vm_s = memory_suitable_view(all_tasks_info[i].vm_size);
                pCell4->setText(vm_s.c_str());

                QTableWidgetItem *pCell5 = ui->tableWidget->item(i, 5);
                std::string pss_s = memory_suitable_view(all_tasks_info[i].pss);
                pCell5->setText(pss_s.c_str());

                QTableWidgetItem *pCell6 = ui->tableWidget->item(i, 6);
                pCell6->setText((std::to_string(all_tasks_info[i].mem_percentage) + " %").c_str());
                for (int j = 0; j < 7; j++){
                    ui->tableWidget->item(i, j)->setForeground(QBrush(QColor(theme_text_color.c_str())));
                }
                int cpu25 = (all_tasks_info[i].cpu_usage >=25) ? 1 : 0;
                int mem25 = (all_tasks_info[i].mem_percentage >=25) ? 1 : 0;
                if (cpu25 || mem25){
                    double val = cpu25 ? all_tasks_info[i].cpu_usage : all_tasks_info[i].mem_percentage;
                    char color = cpu25 ? 'g' : 'p';
                    if(cpu25 && mem25){
                        val = (all_tasks_info[i].cpu_usage + all_tasks_info[i].mem_percentage) / 2;
                        color = 'r';
                    }
                    int range_percent = 0;
                    if (val >= 90){
                        range_percent = 4;
                    } else {
                        while (val >= 25){
                            val -= 25;
                            range_percent++;
                        }
                    }
                    std::string color_key = std::to_string(range_percent) + color;
                    std::string color_val = color_map[color_key];
                    ui->tableWidget->horizontalHeader()->setStyleSheet(QString::fromStdString("color: " + theme_text_color));
                    ui->tableWidget->verticalHeader()->setStyleSheet(QString::fromStdString("color: " + theme_text_color));
                    for (int j = 0; j < 7; j++){
                        ui->tableWidget->item(i, j)->setBackground(QBrush(QColor(color_val.c_str())));
                    }
                }
            }
            ui->tableWidget->update();
            updating_t = 0;
}
