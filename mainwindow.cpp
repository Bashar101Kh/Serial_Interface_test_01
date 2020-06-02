#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_com_port.h"
#include "ui_dialog_com_port.h"

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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::sp = new QSerialPort(this);

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
    chart1->setTitle("Solar Modul 1.");
    series1 = new QSplineSeries(chart1);
//    series1->append(1,1);
    chart1->addSeries(series1);
    chart1->createDefaultAxes();
    chart1->axes(Qt::Horizontal).first()->setRange(0, 120);
    chart1->axes(Qt::Vertical).first()->setRange(0, 12);
    QChartView *chartView;
    chartView = new QChartView(chart1);
    chartView->setRenderHint(QPainter::Antialiasing, 1);
    ui->gridLayout->addWidget(chartView,1,0);


    connect(ui->actionCOM_Port_Config,&QAction::triggered,this,&MainWindow::open_com_dialog);

}


MainWindow::~MainWindow()
{
    sp->close();

    delete ui;
}


void MainWindow::new_data_available(){
    qDebug() << sp->readAll();
}
void MainWindow::open_com_dialog(){
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
    qDebug()<< "\nSeries size befor: "<<series1->points().size();
    qDebug()<< "added Point: ("<<ui->lineEdit->text().toDouble()<<","<<ui->lineEdit_2->text().toDouble() <<")";

    series1->append(ui->lineEdit->text().toDouble(),ui->lineEdit_2->text().toDouble());
    chart1->update();
    qDebug()<<  "Series size after: "<< series1->points().size();
}
