#include <QVector>

#include "mainwindow.h"
#include "ui_mainwindow.h"


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


QVector<double> MainWindow::load_points(QString query, int div){
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
       std::string y_string = y.toStdString();
       double y_double = stod(y_string);
       if (div){
           y_double = y_double/constinf.cpu_cores;
       }
       y_ax.append(y_double);
    }
    return y_ax;
}
