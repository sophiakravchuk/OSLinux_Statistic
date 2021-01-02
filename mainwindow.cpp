#include <QTimer>
#include <QMenu>
#include <QContextMenuEvent>
#include <QLocale>
#include <QMessageBox>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtGui/QPainter>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include <signal.h>
#include <iostream>
#include <ctime>

#include <boost/algorithm/string/trim.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additional_functions.h"
#include "parse_proc_dir.h"
#include "structures.h"
#include "reading_from_file.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),

    ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        db = QSqlDatabase::addDatabase("QSQLITE");
        // std::string curr_path = std::filesystem::current_path();
        // std::string curr_path = "/run/user/1000";
        std::string curr_path = std::filesystem::temp_directory_path();
        std::string path_to_db = curr_path + "/db.sqlite";
        if(std::filesystem::exists(path_to_db)){
            std::filesystem::remove(path_to_db);
        }

        db.setDatabaseName(path_to_db.c_str());
        if(!db.open()){
            std::cout << "Cannot create database!!!" << std::endl;
            QMessageBox::warning(this,
                                         "Database creation",
                                         "Cannot create database",
                                         QMessageBox::Ok);
            qApp->quit();
        }
        QString query = "CREATE TABLE all_finfos ("
                "time_checkp Integer,"
                "name varchar,"
                "pid integer,"
                "state varchar,"
                "cpu_usage real,"
                "virt Real,"
                "pss Real,"
                "mem_usage real"
              ");"
                "";
        QSqlQuery qry;
        if(!qry.exec(query)){
            QMessageBox::warning(this,
                                         "Database creation",
                                         "Cannot create table",
                                         QMessageBox::Ok);
            qApp->quit();
        }

        std::map<std::string, std::string> consti = get_constant_info();
        constinf.cpu_cores = stoi(consti["cpu cores"]);
        constinf.cpu_name = consti["model name"];

        wind_timer = new QTimer(this);
        connect(wind_timer, &QTimer::timeout, this, &MainWindow::update_database);
        wind_timer->start(1000);

        ui->tableWidget->verticalHeader()->setVisible(false);

        QObject::connect( ui->tableWidget->horizontalHeader(), SIGNAL( sectionClicked( int ) ), this, SLOT( on_myTable_sectionClicked( int ) ) );

        QPixmap pixmap(":/processes.png");
        QIcon ButtonIcon(pixmap);
        ui->Processes_Button->setIcon(ButtonIcon);
        ui->Processes_Button->setIconSize(QSize(20, 20));

        QPixmap pixmap1(":/cpu.png");
        QIcon ButtonIcon1(pixmap1);
        ui->CPU_Button->setIcon(ButtonIcon1);
        ui->CPU_Button->setIconSize(QSize(20, 20));

        QPixmap pixmap2(":/memory.png");
        QIcon ButtonIcon2(pixmap2);
        ui->Memory_Button->setIcon(ButtonIcon2);
        ui->Memory_Button->setIconSize(QSize(20, 20));

        QPixmap pixmap3(":/aboutus.png");
        QIcon ButtonIcon3(pixmap3);
        ui->AboutUs_Button->setIcon(ButtonIcon3);
        ui->AboutUs_Button->setIconSize(QSize(20, 20));

        QPixmap pixmap4(":/help.png");
        QIcon ButtonIcon4(pixmap4);
        ui->Help_Button->setIcon(ButtonIcon4);
        ui->Help_Button->setIconSize(QSize(25, 25));

        QPixmap pixmap5(":/dark_theme.png");
        QIcon ButtonIcon5(pixmap5);
        ui->DarkTheme->setIcon(ButtonIcon5);
        ui->DarkTheme->setIconSize(QSize(25, 25));

        btns.push_back(ui->Processes_Button);
        btns.push_back(ui->CPU_Button);
        btns.push_back(ui->Memory_Button);
        btns.push_back(ui->AboutUs_Button);
        btns.push_back(ui->Help_Button);
        btns.push_back(ui->DarkTheme);

        ui->plot->addGraph();
        ui->plot_2->addGraph();
        ui->plot_3->addGraph();
        ui->plot_4->addGraph();
        plts.push_back(ui->plot);
        plts.push_back(ui->plot_2);
        plts.push_back(ui->plot_3);
        plts.push_back(ui->plot_4);
        update_database();
        style_text_fields();
        theme_highlight_color = "#90e0ef";
        theme_default_color = "#efefef";
        theme_text_color = "#000000";
        std::string background_color = "background-color:" + theme_default_color + ";";
        this->setStyleSheet(QString::fromStdString(background_color));

        on_Processes_Button_clicked();
    }


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::style_text_fields(){
    std::string text_c = "color:'" + theme_text_color + "';";
    std::string cpu_text_c = "background-color:'#289EE37D'; " + text_c;
    std::string mem_text_c = "background-color:'#229D4EDD'; " + text_c;

    ui->NameText->setStyleSheet(QString("background-color:'#8863C132'; font-weight: 900; "
                                        " font-size: 17px;  margin-left: 250px; ;  margin-right: 250px;"));

    ui->CP_usage_text->setStyleSheet(QString(cpu_text_c.c_str()));
    ui->NumbOfCorsText->setStyleSheet(QString(cpu_text_c.c_str()));
    ui->NumbOfProcsText->setStyleSheet(QString(cpu_text_c.c_str()));
    ui->WorkingTimeText->setStyleSheet(QString(cpu_text_c.c_str()));

    ui->CachedMemText->setStyleSheet(QString(mem_text_c.c_str()));
    ui->FreeMemText->setStyleSheet(QString(mem_text_c.c_str()));
    ui->TotalMemText->setStyleSheet(QString(mem_text_c.c_str()));
    ui->UsedMemText->setStyleSheet(QString(mem_text_c.c_str()));


    ui->UsedCpuText_2->setStyleSheet(QString(cpu_text_c.c_str()));
    ui->UsedCpuText_3->setStyleSheet(QString(cpu_text_c.c_str()));
    ui->UsedMemText_2->setStyleSheet(QString(mem_text_c.c_str()));

    ui->Title_2->setStyleSheet(QString("font-weight: 900; font-size: 17px;  margin-left: 250px; ;  margin-right: 250px;"));

};

void MainWindow::update_table() {
    int in = 0;


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

            in++;

}

void MainWindow::update_database(){
    all_tasks_info.clear();
    long mem_total = get_mem_total();
    time_t now = time(0);
    if (pushed_time_checkpoints >= 100){
        QString query = "DELETE FROM all_finfos WHERE time_checkp = (SELECT MIN(time_checkp) FROM all_finfos);";
        QSqlQuery qry(db);
        if(!qry.exec(query)){
            QMessageBox::warning(this,
                                         "Database creation",
                                         "Cannot delete checkpoint",
                                         QMessageBox::Ok);
            qApp->quit();
        }
        pushed_time_checkpoints--;
    }
    QSqlQuery qry(db);
    qry.prepare("INSERT INTO all_finfos VALUES (?, ?, ?, ?, ?, ?, ?, ?);");
    QVariantList checkps;
    QVariantList procnames;
    QVariantList pids;
    QVariantList procstates;
    QVariantList cpuusages;
    QVariantList vm_s;
    QVariantList psss;
    QVariantList memusages;

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
        if (finfo.process_name == ""){
            continue;
        }
        get_memory_info(&finfo, file_path, mem_total);
        get_proc_state(&finfo, file_path);



        checkps << QString::fromStdString(std::to_string(now));
        procnames << QString::fromStdString(finfo.process_name);
        pids << QString::fromStdString(std::to_string(finfo.pid));
        procstates << QString::fromStdString(finfo.proc_state);
        cpuusages << QString::fromStdString(std::to_string(finfo.cpu_usage));
        vm_s << QString::fromStdString(std::to_string(finfo.vm_size));
        psss << QString::fromStdString(std::to_string(finfo.pss));
        memusages << QString::fromStdString(std::to_string(finfo.mem_percentage));

        all_tasks_info.push_back(finfo);

    }
    qry.addBindValue(checkps);
    qry.addBindValue(procnames);
    qry.addBindValue(pids);
    qry.addBindValue(procstates);
    qry.addBindValue(cpuusages);
    qry.addBindValue(vm_s);
    qry.addBindValue(psss);
    qry.addBindValue(memusages);

    if(!qry.execBatch()){
        QMessageBox::warning(this,
                                     "Database creation",
                                     "Cannot add a row",
                                     QMessageBox::Ok);
        qApp->quit();
    }


    pushed_time_checkpoints++;
    render_window();
}

void MainWindow::on_myTable_sectionClicked(int index) {
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

void MainWindow::slotKill(){
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
    update_proc_graph();
    actv_wind.processes = 0;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 1;
    ui->stackedWidget->setCurrentIndex(4);
    render_window();
}



QVector<double> MainWindow::load_points_proc(std::string data_type){

    std::string points_type;
    if (data_type == "proc"){
          points_type = "cpu_usage";
    } else {
          points_type = "mem_usage";
    }

    std::string a = "SELECT time_checkp, " + points_type + " FROM all_finfos "
                              "WHERE pid='" + std::to_string(pid_right_clicked) + "' GROUP BY time_checkp;";
    QString query = QString::fromStdString(a);

    QSqlQuery query1;

    if(!query1.exec(query)){
       QMessageBox::warning(this,
                                    "Database usage",
                                   "Cannot select data",
                                    QMessageBox::Ok);
    }
    QVector<double> y_ax;
    while (query1.next()) {
       QString y = query1.value(1).toString();
       std::string y_string = y.toStdString();
       double y_double = stod(y_string);
       if (data_type == "proc") {
           y_double = y_double/constinf.cpu_cores;
       }
       y_ax.append(y_double);
    }
    return y_ax;
}


void MainWindow::update_proc_graph(){
    QVector<double> y_ax_cpu = load_points_proc("proc");
    QVector<double> x_ax_cpu;

    for(int i = 0; i < y_ax_cpu.size(); i++){
        x_ax_cpu.append(i);
    }

    draw_graph(x_ax_cpu, y_ax_cpu, 2, "3g", "4g");

    ui->UsedCpuText_2->setText(QString::number(y_ax_cpu[y_ax_cpu.size()-1]*constinf.cpu_cores) + "%");
    ui->UsedCpuText_3->setText(QString::number(y_ax_cpu[y_ax_cpu.size()-1]) + "%");

    QVector<double> y_ax_mem = load_points_proc("mem");
    QVector<double> x_ax_mem;

    for(int i = 0; i < y_ax_mem.size(); i++){
        x_ax_mem.append(i);
    }
    draw_graph(x_ax_mem, y_ax_mem, 3, "1p", "4p");

    ui->UsedMemText_2->setText(QString::number(y_ax_mem[y_ax_mem.size()-1]) + "%");
    ui->Title_2->setText(QString::fromStdString(process_right_clicked+" ("+std::to_string(pid_right_clicked)+")"));
    style_text_fields();
}


void MainWindow::draw_graph(QVector<double> x_ax, QVector<double> y_ax, int plt_ind, std::string color_brush, std::string color_pen){
    plts[plt_ind]->graph(0)->clearData();
    plts[plt_ind]->graph(0)->setData(x_ax, y_ax);
    plts[plt_ind]->xAxis->setRange(0, x_ax.size());
    plts[plt_ind]->xAxis->setVisible(false);

    plts[plt_ind]->graph(0)->setBrush(QColor(color_map[color_brush].c_str()));
    plts[plt_ind]->graph(0)->setPen(QColor(color_map[color_pen].c_str()));
    plts[plt_ind]->setBackground(QBrush(theme_default_color.c_str()));

    plts[plt_ind]->yAxis->setBasePen(QPen(QColor(theme_text_color.c_str())));
    plts[plt_ind]->yAxis->setTickLabelColor(QColor(theme_text_color.c_str()));
    plts[plt_ind]->yAxis->setLabelColor(QColor(theme_text_color.c_str()));
    plts[plt_ind]->yAxis->setTickPen(QColor(theme_text_color.c_str()));
    plts[plt_ind]->yAxis->setSubTickPen(QColor(theme_text_color.c_str()));

    plts[plt_ind]->yAxis->setRange(0, 100);
    plts[plt_ind]->replot();
    plts[plt_ind]->update();
}


QVector<double> MainWindow::load_points_cpu(){
    QString query = "SELECT "
            "time_checkp,"
            "SUM(cpu_usage) "
        "FROM "
            "all_finfos "
        "GROUP BY "
            "time_checkp;"
            "";
    QSqlQuery qry;

    if(!qry.exec(query)){
       QMessageBox::warning(this,
                                    "Database usage",
                                    "Cannot select data",
                                    QMessageBox::Ok);
    }
    QVector<double> y_ax;
    while (qry.next()) {
       QString y = qry.value(1).toString();
       y_ax.append(y.toDouble()/constinf.cpu_cores);
    }
    return y_ax;
}

void MainWindow::update_cpu_graph() {
    QVector<double> y_ax = load_points_cpu();
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


QVector<double> MainWindow::load_points_mem(){
    QString query = "SELECT "
            "time_checkp,"
            "SUM(mem_usage) "
        "FROM "
            "all_finfos "
        "GROUP BY "
            "time_checkp;"
            "";
    QSqlQuery qry;

    if(!qry.exec(query)){
       QMessageBox::warning(this,
                                    "Database usage",
                                    "Cannot select data",
                                    QMessageBox::Ok);
    }
    QVector<double> y_ax;
    while (qry.next()) {
       QString y = qry.value(1).toString();
       y_ax.append(y.toDouble());
    }
    return y_ax;
}


void MainWindow::update_mem_graph() {
    QVector<double> y_ax = load_points_mem();
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
//    memory_suitable_view();

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

void MainWindow::render_window() {

    if (actv_wind.processes) {
        update_table();
    }
    else if (actv_wind.cpu) {
        update_cpu_graph();

    } else if (actv_wind.memory) {
        update_mem_graph();

    } else if(actv_wind.graph_proc){
        update_proc_graph();
    } else {
        update_table();
    }
}



void MainWindow::on_Processes_Button_clicked()
{
    actv_wind.processes = 1;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 0;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(1);

    for(auto btn: btns){
        if(btn == ui->Processes_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }


    render_window();
}




void MainWindow::on_CPU_Button_clicked()
{
    actv_wind.processes = 0;
    actv_wind.cpu = 1;
    actv_wind.memory = 0;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 0;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(2);

    for(auto btn: btns){
        if(btn == ui->CPU_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }

    render_window();
}

void MainWindow::on_Memory_Button_clicked()
{
    actv_wind.processes = 0;
    actv_wind.cpu = 0;
    actv_wind.memory = 1;
    actv_wind.about_us = 0;
    actv_wind.graph_proc = 0;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(3);

    for(auto btn: btns){
        if(btn == ui->Memory_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }

    render_window();
}

void MainWindow::on_AboutUs_Button_clicked()
{
    actv_wind.processes = 0;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.graph_proc = 0;
    actv_wind.about_us = 1;
    actv_wind.help = 0;
    ui->stackedWidget->setCurrentIndex(5);

    for(auto btn: btns){
        if(btn == ui->AboutUs_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }

    render_window();
}

void MainWindow::on_kill_this_proc_clicked()
{
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

void MainWindow::on_Help_Button_clicked()
{
    actv_wind.processes = 0;
    actv_wind.cpu = 0;
    actv_wind.memory = 0;
    actv_wind.graph_proc = 0;
    actv_wind.about_us = 0;
    actv_wind.help = 1;
    ui->stackedWidget->setCurrentIndex(6);
    for(auto btn: btns){
        if(btn == ui->Help_Button){
            std::string text_button_c = "QPushButton {background-color: " + theme_highlight_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        } else {
            std::string text_button_c = "QPushButton {background-color: " + theme_default_color + ";}";
            btn->setStyleSheet(QString::fromStdString(text_button_c));
        }
        btn->setAutoFillBackground(true);
        btn->update();
    }

    render_window();
}

void MainWindow::on_DarkTheme_clicked()
{
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
