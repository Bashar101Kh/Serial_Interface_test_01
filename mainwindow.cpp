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
    chart1->setTitle("Solar Modul 1.");
    series1 = new QSplineSeries();
    series2 = new QSplineSeries();
    series1->setName("Spannung");
    series2->setName("Leistung");
    //series1 = new QSplineSeries(chart1);
    chart1->addSeries(series1);
    chart1->addSeries(series2);

    //chart1->addSeries(series2);
    chart1->createDefaultAxes();
    chart1->axes(Qt::Horizontal).first()->setRange(0, 24);
    chart1->axes(Qt::Vertical).first()->setRange(0, 12);
    QChartView *chartView;
    chartView = new QChartView(chart1);
    chartView->setRenderHint(QPainter::Antialiasing, 1);
    ui->gridLayout->addWidget(chartView,1,0);


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
    if (rxdataRow.size() > 5){
        int id;
        QString telegramm,value;
        telegramm = rxdataRow.left(6);
        id = telegramm.left(1).toInt();
        qDebug()<<id<<" | "<< telegramm.mid(1,5);
        value = rxdataRow.mid(1,5);
        switch(id){
        case 0:         // Voltage Value [V]

            voltages.append(value);
            value += " Volt";
        break;

        case 1:         // Current Value [I]
            currents.append(value);
            value += " Amper";
            plottData();
        break;

        case 2:
            //powers.append(value);
            value += " Watt";
        break;

        default:
            ;
        }
        iX++;

        qDebug()<< "value"<< value;
        rxdataRow.remove(0,6);

    }
}
void MainWindow::plottData()
{
    qDebug()<< "plotting ....."<<voltages.right(5).toDouble() <<"   "<< currents.right(5).toDouble();
    series1->append(voltages.right(5).toDouble(), currents.right(5).toDouble());
    double p = voltages.right(5).toDouble()*currents.right(5).toDouble();
    series2->append(voltages.right(5).toDouble(), p/10.0 /*powers.right(5).toDouble()/10.0*/);
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

}
