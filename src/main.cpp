#include "headers/mainwindow.h"
#include <unistd.h>
#include <filesystem>
#include <QMessageBox>
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    a.setApplicationName("SOrrel JUice");
    MainWindow w;

    w.show();
    w.setWindowIcon(QIcon(":/photos/icon.png"));
    w.setWindowTitle("SOrrel JUice");

    return a.exec();
}
