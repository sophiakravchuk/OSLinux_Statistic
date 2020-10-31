#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
    struct sorting_info sort_flags { {0, -1, -1, -1, -1, -1}, } ;

    std::vector<struct task_manager_file_info> all_tasks_info;
    void update_table();
    QTimer* wind_timer;


private slots:

    void on_myTable_sectionClicked(int index);

    void sort_table(std::vector<struct task_manager_file_info>* all_tasks_info);


private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
