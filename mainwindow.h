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
    struct active_window actv_wind { 1, 0, 0, 0};

    std::vector<struct task_manager_file_info> all_tasks_info;
    void update_table();
    QTimer* wind_timer;
    void contextMenuEvent( QContextMenuEvent * e );
    void render_window();
    void update_cpu_graph();
    struct constant_info constinf;

private slots:

    void slotKill();

    void on_myTable_sectionClicked(int index);

    void sort_table(std::vector<struct task_manager_file_info>* all_tasks_info);

    void on_Processes_Button_clicked();

    void on_CPU_Button_clicked();

private:
    std::string process_to_kill;
    int pid_to_kill;
    int updating_t;
    Ui::MainWindow *ui;
    int pushed_time_checkpoints = 0;
    void draw_graph(QVector<double> x_ax, QVector<double> y_ax);
    QVector<double> load_points_cpu();

};
#endif // MAINWINDOW_H
