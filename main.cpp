#include "mainwindow.h"
#include <unistd.h>
#include <filesystem>
#include <QMessageBox>
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    w.render_window();

    return a.exec();
}
