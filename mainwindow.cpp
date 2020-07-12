#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_com_port.h"
#include "ui_dialog_com_port.h"

#include <iostream>
#include <string>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QSerialPortInfo>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    MainWindow::sp = new QSerialPort(this);
    dw.set_serialport(sp);
    iX = 0;


//       sp = dw.sp1;
//    QChartView *chartView;
//    chart1 = createLineChart();
//    chartView = new QChartView(chart1);
//    chartView->setRenderHint(QPainter::Antialiasing, 1);
//    ui->gridLayout->addWidget(chartView,1,0 );

//    chart2 = createLineChart();
//    chartView = new QChartView(chart2);
//    chartView->setRenderHint(QPainter::Antialiasing, 1);

//    ui->gridLayout->addWidget(chartView,1,1 );

    chart1 = new QChart();
    chart2 = new QChart();
    chart1->setTitle("Solar Modul 1.");
    chart2->setTitle("Solar Modul 2.");
    series1 = new QSplineSeries();
    series2 = new QSplineSeries();
    series3 = new QSplineSeries();
    series4 = new QSplineSeries();
    series1->setName("Spannung");
    series2->setName("Leistung");
    series3->setName("Spannung");
    series4->setName("Leistung");

    //series1 = new QSplineSeries(chart1);
    chart1->addSeries(series1);
    chart1->addSeries(series2);
    chart2->addSeries(series3);
    chart2->addSeries(series4);

    //chart->addSeries(series2);
    chart1->createDefaultAxes();
    QCategoryAxis *axisY1 = new QCategoryAxis;
    chart1->addAxis(axisY1,Qt::AlignRight);
    axisY1->setTitleText("Current");
    chart1->axes(Qt::Horizontal).first()->setTitleText("Voltage");
    chart1->axes(Qt::Vertical).first()->setTitleText("Power");
    chart1->axes(Qt::Horizontal).first()->setRange(0, 24);
    chart1->axes(Qt::Vertical).first()->setRange(0, 12);


    chart2->createDefaultAxes();
    chart2->axes(Qt::Horizontal).first()->setTitleText("Voltage");
    chart2->axes(Qt::Vertical).first()->setTitleText("Power");
    chart2->axes(Qt::Horizontal).first()->setRange(0, 24);
    chart2->axes(Qt::Vertical).first()->setRange(0, 12);
    QChartView *chartView1,*chartView2;
    chartView1 = new QChartView(chart1);
    chartView2 = new QChartView(chart2);
    chartView1->setRenderHint(QPainter::Antialiasing, 1);
    chartView2->setRenderHint(QPainter::Antialiasing, 1);
    ui->gridLayout->addWidget(chartView1,1,0);
    ui->gridLayout->addWidget(chartView2,1,1);


//    QPalette pal = window()->palette();
//    pal.setColor(QPalette::Window, QRgb(0x121218));
//    pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
//    window()->setPalette(pal);
//    chart1->setTheme(QChart::ChartThemeDark);
//    ui->menubar->setPalette(pal);

    connect(ui->actionCOM_Port_Config,&QAction::triggered,this,&MainWindow::openComDialog);
    connect(sp,SIGNAL(readyRead()),this, SLOT(newDataAvailable()));

}


MainWindow::~MainWindow()
{
    sp->close();

    delete ui;
}


void MainWindow::newDataAvailable(){
    rxdataRow += sp->readAll();
    qDebug() << "MAINWINDOW RX : "<< rxdataRow;
    process_rxData();
}
void MainWindow::process_rxData(){
    if (rxdataRow.size() > 4){
        int id,i_value;
        double d_value,battery_val_holder;
        QString value,Telegramm;
        qDebug() << "rxdata: " << rxdataRow;
        id = rxdataRow.at(0);
        Telegramm = rxdataRow.mid(0,5).toHex();
        value = rxdataRow.mid(1,2).toHex();
        i_value = value.toUInt(nullptr,16);
        d_value = i_value/100.0;

        qDebug()<<"Telegramm: "<< Telegramm<< "\tsize : "<< Telegramm.length() ;
        qDebug()<<"value: "<< value<< "\tsize : "<< value.length() ;
        qDebug()<<"i_value: "<< i_value ;
        qDebug()<<"d_value: "<< d_value ;
        qDebug() << "id: " << id ;
        switch(id){
        case 0:         // Voltage Value Panel 1 [V]

            S1_voltages.append(value);
            value = QString::number(d_value)+" Volt";

        break;

        case 1:         // Current Value Panel 1 [A]
            S1_currents.append(value);
            value = QString::number(d_value)+" Amper";
            plottData(0);
        break;

        case 2:         // Voltage Value Panel 2 [V]

            S2_voltages.append(value);
            value = QString::number(d_value)+" Volt";
            //plottData();
        break;

        case 3:         // Current Value Panel 2 [A]
            S2_currents.append(value);
            value = QString::number(d_value)+" Amper";
            plottData(1);
        break;

        case 4:         // Valtage Battary [V]
            ui->bat_V->setText(QString::number(d_value));
            break;

        case 5:         // Current Battery[A]
            ui->bat_I->setText(QString::number(d_value));
            break;



        default:
            ;
        }
        iX++;

        qDebug()<< "value: "<< value;
        rxdataRow.remove(0,5);
        if(rxdataRow.size() != 0){
            MainWindow::process_rxData();
        }

    }
}
void MainWindow::plottData(int panel_id)
{
    QString s_Vvalue, s_Ivalue, s_Pvalue ;
    double d_Vvalue, d_Ivalue, d_Pvalue ;
    switch (panel_id) {
    case 0 :
        qDebug()<< "Plot Test Case !";
        qDebug()<<"S1_voltages : " <<S1_voltages<<"\t size : "<< S1_voltages.size();
        qDebug()<<"S1_currents : " <<S1_currents<<"\t size : "<< S1_currents.size();
        s_Vvalue = S1_voltages.left(4);
        s_Ivalue = S1_currents.left(4);
        d_Vvalue = s_Vvalue.toUInt(nullptr,16)/100.0;
        d_Ivalue = s_Ivalue.toUInt(nullptr,16)/100.0;
        d_Pvalue = (d_Ivalue * d_Vvalue)/10.0;

        qDebug () << s_Vvalue.toUInt(nullptr,16);
        qDebug()<< "plotting ....."<<d_Vvalue <<"   "<< d_Ivalue<<"   "<< d_Pvalue;
        series1->append(d_Vvalue, d_Ivalue);
        series2->append(d_Vvalue, d_Pvalue);
        S1_voltages = S1_voltages.mid(5,S1_voltages.size()-1);
        S1_currents = S1_currents.mid(5,S1_voltages.size()-1);
        break;

    case 1 :
        qDebug()<< "Plot Test Case !";
        qDebug()<<"S2_voltages : " <<S1_voltages<<"\t size : "<< S2_voltages.size();
        qDebug()<<"S2_currents : " <<S1_currents<<"\t size : "<< S2_currents.size();
        s_Vvalue = S2_voltages.left(4);
        s_Ivalue = S2_currents.left(4);
        d_Vvalue = s_Vvalue.toUInt(nullptr,16)/100.0;
        d_Ivalue = s_Ivalue.toUInt(nullptr,16)/100.0;
        d_Pvalue = (d_Ivalue * d_Vvalue)/10.0;

        qDebug () << s_Vvalue.toUInt(nullptr,16);
        qDebug()<< "plotting ....."<<d_Vvalue <<"   "<< d_Ivalue<<"   "<< d_Pvalue;
        series3->append(d_Vvalue, d_Ivalue);
        series4->append(d_Vvalue, d_Pvalue);
        S2_voltages = S2_voltages.mid(5,S2_voltages.size()-1);
        S2_currents = S2_currents.mid(5,S2_voltages.size()-1);
//        qDebug()<< "plotting ....."<<S1_voltages.right(5).toDouble() <<"   "<< S1_currents.right(5).toDouble();
//        series1->append(S1_voltages.right(5).toDouble(), S1_currents.right(5).toDouble());
//        double p = S1_voltages.right(5).toDouble()*S1_currents.right(5).toDouble();
//        series2->append(S1_voltages.right(5).toDouble(), p/10.0 /*powers.right(5).toDouble()/10.0*/);
        break;


    }
}

void MainWindow::openComDialog(){
    dw.setModal(true);
    dw.exec();
}

void MainWindow::set_serialport(QSerialPort *p){
    this->sp =  p ;
}
void MainWindow::showMsg(QString msg, int t){
    ui->statusbar->showMessage(msg, t);
}

QChart *MainWindow::createLineChart() const
{
    //![1]
    QChart *chart = new QChart();
    chart->setTitle("Line chart");
    //![1]
    //![2]
    QString name("Series ");
        QSplineSeries *series = new QSplineSeries(chart);
        chart->addSeries(series);

    //![2]
    //![3]
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 12);
    chart->axes(Qt::Vertical).first()->setRange(0, 12);
    //![3]
    //![4]
    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");
    //![4]

    return chart;
}

void MainWindow::on_pushButton_clicked()
{
//    qDebug()<< "\nSeries size befor: "<<series1->points().size();
//    qDebug()<< "added Point: ("<<ui->lineEdit->text().toDouble()<<","<<ui->lineEdit_2->text().toDouble() <<")";

//    series1->append(ui->lineEdit->text().toDouble(),ui->lineEdit_2->text().toDouble());
//    chart1->update();
//    qDebug()<<  "Series size after: "<< series1->points().size();
    series1->clear();
    series2->clear();
    series3->clear();
    series4->clear();

}

void MainWindow::on_pushButton_2_clicked()
{
    double x, y ;
    x = ui->lineEdit_x->text().toDouble(nullptr);
    y = ui->lineEdit_y->text().toDouble(nullptr);
    series1->append(x,y);
}

void MainWindow::on_pushButton_3_clicked()
{
    double x, y ;
    x = ui->lineEdit_x->text().toDouble(nullptr);
    y = ui->lineEdit_y->text().toDouble(nullptr);
    series3->append(x,y);
}
