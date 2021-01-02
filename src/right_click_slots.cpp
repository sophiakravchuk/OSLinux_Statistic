#include "iostream"
#include "signal.h"

#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/structures.h"


void MainWindow::slotKill(){
    /**
    * @brief This function show window of QMessageBox to the screen
    * with warning or clarification that the process will be killed
    * or cannot be killed.
    */
    std::string delete_message = "Do you really want to kill process: ";
    delete_message += process_right_clicked + " ?";
    if (QMessageBox::warning(this,
                             "Killing process",
                             delete_message.c_str(),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        /* If the answer is no* */
        return;
    } else {
        for(auto finfo : all_tasks_info){
            if (finfo.pid == pid_right_clicked){
                if (finfo.process_name != process_right_clicked){
                    QMessageBox::warning(this,
                                                 "Killing process",
                                                 "No such process found",
                                                 QMessageBox::Ok);
                    std::cout << "No such process found" << std::endl;
                }
                break;
            }
        }
        if (pid_right_clicked < 0){
            QMessageBox::warning(this,
                                         "Killing process",
                                         "No such process found",
                                         QMessageBox::Ok);
            std::cout << "No such process found" << std::endl;
        }
        if(kill(pid_right_clicked, SIGTERM)!=0){
            QMessageBox::warning(this,
                                         "Killing process",
                                         "Cannot kill process",
                                         QMessageBox::Ok);
            std::cout << "Cannot kill process" << std::endl;
        }


    }
}


void MainWindow::slotGraph(){
    /**
    * @brief This function is called when user clicks on button "Graph process"
    * in right click menu.
    * This function leads to process individual page.
    */
    update_proc_graph();
    actv_wind.processes = 0;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 1;
    ui->stackedWidget->setCurrentIndex(4);
    render_window();
}
