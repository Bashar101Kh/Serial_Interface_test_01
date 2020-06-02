#include "mainwindow.h"
#include "dialog_com_port.h"
#include <QLoggingCategory>
#include <QApplication>
#include <QtCharts>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

