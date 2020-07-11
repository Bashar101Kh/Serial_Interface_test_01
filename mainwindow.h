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
    void openComDialog();
    void newDataAvailable();
    void on_pushButton_clicked();
    void process_rxData();
    void plottData(int panel_id);


signals:
    void rxData_changed();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *sp;
    QByteArray rxdataRow,messureSeries,S1_voltages,S1_currents,S2_currents,S2_voltages,S1_powers,S2_powers;
    int iX;
    QChart *createLineChart() const;
    QChart *chart1 , *chart2;
    QSplineSeries *series1,*series2,*series3,*series4;

};




#endif // MAINWINDOW_H
