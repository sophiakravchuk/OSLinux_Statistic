#include <string>

#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include "headers/structures.h"
#include "headers/reading_from_file.h"


void MainWindow::on_Processes_Button_clicked() {
    /**
    * @brief This function is called when user clicks on button "Processes_Button".
    * This function leads to processes page and highlights the button.
    */
    actv_wind.processes = 1;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 0;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(1);

    for(auto btn: btns){
        if(btn == ui->Processes_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }


    render_window();
}


void MainWindow::on_CPU_Button_clicked() {
    /**
    * @brief This function is called when user clicks on button "CPU_Button".
    * This function leads to the page with CPUusage graph and highlights the button.
    */
    actv_wind.processes = 0;
    actv_wind.cpu = 1;
    actv_wind.memory = 0;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 0;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(2);

    for(auto btn: btns){
        if(btn == ui->CPU_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }

    render_window();
}


void MainWindow::on_Memory_Button_clicked() {
    /**
    * @brief This function is called when user clicks on button "Memory_Button".
    * This function leads to the page with Memory usage graph and highlights the button.
    */
    actv_wind.processes = 0;
    actv_wind.cpu = 0;
    actv_wind.memory = 1;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 0;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(3);

    for(auto btn: btns){
        if(btn == ui->Memory_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }

    render_window();
}


void MainWindow::on_AboutUs_Button_clicked() {
    /**
    * @brief This function is called when user clicks on button "AboutUs_Button".
    * This function leads to the page with text about us and highlights the button.
    */
    actv_wind.processes = 0;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.graph_proc = 0;
    actv_wind.about_us = 1;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(5);

    for(auto btn: btns){
        if(btn == ui->AboutUs_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }

    render_window();
}


void MainWindow::on_kill_this_proc_clicked() {
    /**
    * @brief This function is called when user clicks on button "kill_this_proc".
    * This function leads to processes page and calls slotKill that shows the 
    * QMessageBox that warns you about killing the process and if you confirm the action
    * it kills the process.
    */
    actv_wind.processes = 1;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 0;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(1);
    render_window();
    slotKill();
}


void MainWindow::on_Help_Button_clicked() {
    /**
    * @brief This function is called when user clicks on button "Help_Button".
    * This function leads to the page with help text and highlights the button.
    */
    actv_wind.processes = 0;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.graph_proc = 0;
    actv_wind.about_us = 0;
    actv_wind.help = 1;
    ui->stackedWidget->setCurrentIndex(6);
    for(auto btn: btns){
        if(btn == ui->Help_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + "; "+ "color: " + theme_text_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }

    render_window();
}


void MainWindow::on_DarkTheme_clicked() {
    /**
    * @brief This function is called when user clicks on button "DarkTheme".
    * This function changes the color theme of a whole application.
    */
    if (darktheme){
        darktheme = 0;
        theme_highlight_color = "#90e0ef";
        theme_default_color = "#efefef";
        theme_text_color = "#000000";
    } else {
        darktheme = 1;
        theme_highlight_color = "#1985a1";
        theme_default_color = "#46494c";
        theme_text_color = "#dcdcdd";

    }
    std::string text_button_c = "QPushButton {color: " + theme_text_color + ";}";
    for(auto btn: btns){
        btn->setStyleSheet(QString::fromStdString(text_button_c));
    }

    std::string text_c = "color: " + theme_text_color + ";";
    ui->tabWidget->setStyleSheet(QString::fromStdString(text_c));
    ui->tabWidget_2->setStyleSheet(QString::fromStdString(text_c));
    std::string background_color = "background-color:" + theme_default_color + ";";

    this->setStyleSheet(QString::fromStdString(background_color));

    if(actv_wind.processes){
        on_Processes_Button_clicked();
    } else if (actv_wind.cpu){
        on_CPU_Button_clicked();
    } else if (actv_wind.memory){
        on_Memory_Button_clicked();
    } else if (actv_wind.graph_proc){
        slotGraph();
    } else if (actv_wind.about_us){
        on_AboutUs_Button_clicked();
    } else if (actv_wind.help){
        on_Help_Button_clicked();
    }
    render_window();
}
