#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog_com_port.h"
#include <QMainWindow>
#include <QSerialPort>
#include <QtCharts>
#include <QtCharts/QChartGlobal>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
    class Dialog_COM_Port;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void set_serialport(QSerialPort *sp);
    Dialog_COM_Port dw;
    ~MainWindow();



public slots:
    void showMsg(QString, int t);
    void open_com_dialog();
    void new_data_available();


private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *sp;
    QChart *createLineChart() const;
    QSplineSeries *series1,*series2;
    QChart *chart1 , *chart2;




};
#endif // MAINWINDOW_H
