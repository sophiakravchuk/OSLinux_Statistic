//#include <QTimer>
//#include <QMenu>
//#include <QContextMenuEvent>
//#include <QLocale>
//#include <QMessageBox>
//#include <QtCharts/QLineSeries>
//#include <QtCharts/QAreaSeries>
//#include <QtCharts/QChart>
//#include <QtCharts/QChartView>
//#include <QtGui/QPainter>
//#include <iostream>
//#include <vector>
//#include <stdio.h>
//#include <string>
//#include <map>
//#include <fstream>
//#include <filesystem>
//#include <signal.h>
//#include <iostream>
//#include <ctime>

#include <boost/algorithm/string/trim.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additional_functions.h"
//#include "parse_proc_dir.h"
//#include "structures.h"
#include "reading_from_file.h"




void MainWindow::update_proc_graph(){
    std::string a = "SELECT time_checkp, cpu_usage FROM all_finfos "
                              "WHERE pid='" + std::to_string(pid_right_clicked) + "' GROUP BY time_checkp;";
    QString query = QString::fromStdString(a);
    QVector<double> y_ax_cpu = load_points(query, 1);
    QVector<double> x_ax_cpu;

    for(int i = 0; i < y_ax_cpu.size(); i++){
        x_ax_cpu.append(i);
    }

    draw_graph(x_ax_cpu, y_ax_cpu, 2, "3g", "4g");

    ui->UsedCpuText_2->setText(QString::number(y_ax_cpu[y_ax_cpu.size()-1]*constinf.cpu_cores) + "%");
    ui->UsedCpuText_3->setText(QString::number(y_ax_cpu[y_ax_cpu.size()-1]) + "%");
    std::string b = "SELECT time_checkp, mem_usage FROM all_finfos "
                              "WHERE pid='" + std::to_string(pid_right_clicked) + "' GROUP BY time_checkp;";
    QString query1 = QString::fromStdString(b);
    QVector<double> y_ax_mem = load_points(query1, 0);
    QVector<double> x_ax_mem;

    for(int i = 0; i < y_ax_mem.size(); i++){
        x_ax_mem.append(i);
    }
    draw_graph(x_ax_mem, y_ax_mem, 3, "1p", "4p");

    ui->UsedMemText_2->setText(QString::number(y_ax_mem[y_ax_mem.size()-1]) + "%");
    ui->Title_2->setText(QString::fromStdString(process_right_clicked+" ("+std::to_string(pid_right_clicked)+")"));
    style_text_fields();
}


void MainWindow::update_cpu_graph() {
    QString query = "SELECT "
            "time_checkp,"
            "SUM(cpu_usage) "
        "FROM "
            "all_finfos "
        "GROUP BY "
            "time_checkp;"
            "";
    QVector<double> y_ax = load_points(query, 1);
    QVector<double> x_ax;
    for(int i = 0; i < y_ax.size(); i++){
        x_ax.append(i);
    }

    draw_graph(x_ax, y_ax, 0, "3g", "4g");
    ui->CP_usage_text->setText(QString::number(y_ax[y_ax.size()-1]) + "%");
    ui->NameText->setText(QString::fromStdString(constinf.cpu_name));
    ui->NumbOfCorsText->setText(QString::number(constinf.cpu_cores));
    int uptime_int = 0;
    std::vector<std::string> uptime_vec = read_file_to_vector("/proc/uptime");
    if (!uptime_vec.empty()){
        uptime_int = stoi(uptime_vec[0]);
    }
    QString uptime = QString::fromStdString(seconds_to_time(uptime_int));
    ui->WorkingTimeText->setText(uptime);
    ui->NumbOfProcsText->setText(QString::number(all_tasks_info.size()));
    style_text_fields();
}


void MainWindow::update_mem_graph() {
    QString query = "SELECT "
            "time_checkp,"
            "SUM(mem_usage) "
        "FROM "
            "all_finfos "
        "GROUP BY "
            "time_checkp;"
            "";
    QVector<double> y_ax = load_points(query, 0);
    QVector<double> x_ax;
    for(int i = 0; i < y_ax.size(); i++){
        x_ax.append(i);
    }
    draw_graph(x_ax, y_ax, 1, "1p", "4p");

    std::vector<std::string> key_word = {"MemTotal", "MemFree", "MemAvailable", "Cached", "SwapCached"};
    std::map<std::string, std::string> mem_key_map;
    get_values_from_file(&mem_key_map, key_word, "/proc/meminfo");

    for (int ind = 0; ind < (int)key_word.size(); ind++) {
        mem_key_map[key_word[ind]].pop_back();
        mem_key_map[key_word[ind]].pop_back();
        boost::algorithm::trim(mem_key_map[key_word[ind]]);
    }

    long mem_total = stol(mem_key_map["MemTotal"]);
    ui->TotalMemText->setText(QString::fromStdString(memory_suitable_view(mem_total)));

    long mem_used = (y_ax[y_ax.size()-1] * mem_total)/100;
    ui->UsedMemText->setText(QString::fromStdString(memory_suitable_view(mem_used)) +
            "(" + (QString::number(y_ax[y_ax.size()-1]) + "%)"));

    ui->FreeMemText->setText(QString::fromStdString(memory_suitable_view(stol(mem_key_map["MemAvailable"]) +
                                                    stol(mem_key_map["MemFree"]) +
                                                    stol(mem_key_map["Cached"]))));

    ui->CachedMemText->setText(QString::fromStdString(memory_suitable_view(stol(mem_key_map["Cached"]))));
    style_text_fields();
}

