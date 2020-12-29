#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector>
#include <QMainWindow>
#include <QTimer>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

#include "structures.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    struct sorting_info sort_flags { 0, -1, -1, -1, -1, -1, -1} ;
    struct active_window actv_wind { 1, 0, 0, 0, 0};

    std::vector<struct task_manager_file_info> all_tasks_info;
    void update_table();
    QTimer* wind_timer;
    void contextMenuEvent( QContextMenuEvent * e );
    void render_window();
    void update_cpu_graph();
    void update_mem_graph();
    struct constant_info constinf;
    void update_database();
    void update_proc_graph();
    QVector<double> load_points_proc(std::string data_type);
    void draw_graph_3(QVector<double> x_ax, QVector<double> y_ax);
    void draw_graph_4(QVector<double> x_ax, QVector<double> y_ax);

private slots:

    void slotKill();
    void slotGraph();

    void on_myTable_sectionClicked(int index);

    void sort_table(std::vector<struct task_manager_file_info>* all_tasks_info);

    void on_Processes_Button_clicked();

    void on_CPU_Button_clicked();

    void on_Memory_Button_clicked();

    void on_AboutUs_Button_clicked();

private:
    std::string process_right_clicked;
    int pid_right_clicked;
    int updating_t;
    Ui::MainWindow *ui;
    int pushed_time_checkpoints = 0;
    void draw_graph(QVector<double> x_ax, QVector<double> y_ax);
    void draw_graph_2(QVector<double> x_ax, QVector<double> y_ax);
    QVector<double> load_points_cpu();
    QVector<double> load_points_mem();
    std::map<std::string, std::string> color_map = {
                                                        {"1g", "#889EE37D"},
                                                        {"2g", "#8863C132"},
                                                        {"3g", "#88358600"},
                                                        {"4g", "#880F6D1C"},
                                                        {"1p", "#889D4EDD"},
                                                        {"2p", "#887B2CBF"},
                                                        {"3p", "#885A189A"},
                                                        {"4p", "#883C096C"},
                                                        {"1r", "#88E03E56"},
                                                        {"2r", "#88CF2642"},
                                                        {"3r", "#88A71E34"},
                                                        {"4r", "#8885182A"},
                                                       };

};
#endif // MAINWINDOW_H
