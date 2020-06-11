#include "dialog_com_port.h"
#include "ui_dialog_com_port.h"

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QtCharts>
#include <QSerialPortInfo>

Dialog_COM_Port::Dialog_COM_Port(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_COM_Port)

{
    ui->setupUi(this);
    sp1 = new QSerialPort;
    ap = QSerialPortInfo::availablePorts();
    ui->combo_comList->addItem(" " );
    for (int i = 0 ; i<ap.length() ; i++){
        qDebug() << ap.at(i).portName();
        ui->combo_comList->addItem(ap.at(i).portName());
    }

    port_setings_init();

}
void Dialog_COM_Port::port_setings_init(){
    ui->combo_baudrate->addItem(" ");
    ui->combo_baudrate->addItem("1200");
    ui->combo_baudrate->addItem("2400");
    ui->combo_baudrate->addItem("4800");
    ui->combo_baudrate->addItem("9600");
    ui->combo_baudrate->addItem("19200");
    ui->combo_baudrate->addItem("38400");
    ui->combo_baudrate->addItem("57600");
    ui->combo_baudrate->addItem("115200");

    ui->combo_databits->addItem(" ");
    ui->combo_databits->addItem("5");
    ui->combo_databits->addItem("6");
    ui->combo_databits->addItem("7");
    ui->combo_databits->addItem("8");

    ui->combo_stopbits->addItem(" ");
    ui->combo_stopbits->addItem("1 stop bit");
    ui->combo_stopbits->addItem("1.5 stop bits");
    ui->combo_stopbits->addItem("2 stop bits");

    ui->combo_parity->addItem(" ");
    ui->combo_parity->addItem("No");
    ui->combo_parity->addItem("Even");
    ui->combo_parity->addItem("Odd");
    ui->combo_parity->addItem("Space");
    ui->combo_parity->addItem("Mark");
};

Dialog_COM_Port::~Dialog_COM_Port()
{
    delete ui;
}

void Dialog_COM_Port::set_serialport(QSerialPort *p){
    sp1 =  p ;
}

void Dialog_COM_Port::on_toggle_open_button_clicked()
{
    if(ui->toggle_open_button->text() == "Open Port"){
                qDebug() << "Connect Clicked !" ;
                if (ui->combo_comList->currentIndex() != 0){
                    sp1->setPort(ap.at(ui->combo_comList->currentIndex()-1));
                    qDebug() << " opening "<< sp1->portName() <<" Port..... result:" <<sp1->open(QIODevice::ReadWrite);
                    sp1->setBaudRate(ui->combo_baudrate->currentText().toInt(),QSerialPort::AllDirections);
                    if ( sp1->isOpen()){
                       ui->toggle_open_button->setText("Close Port");

                        qDebug()<< sp1->baudRate();
                        qDebug()<< sp1->parity();
                        qDebug()<< sp1->dataBits();
                        qDebug()<< sp1->stopBits();

                    }else{
                        // do nothing !
                    }
                }else{
                    qDebug() << "set a COM Port to Open !";
                }

        }else if (ui->toggle_open_button->text() == "Close Port"){
            qDebug() << " closing "<< sp1->portName() <<" Port";
            sp1->close();
            if ( !sp1->isOpen()){
               disconnect(sp1,SIGNAL(readyRead()),this, SLOT(new_data_available()));
               ui->toggle_open_button->setText("Open Port");
            }else{
                // do nothing !
            }
        }

}

void Dialog_COM_Port::on_combo_comList_currentIndexChanged(const QString &arg1)
{
    qDebug()<< ap.length();
    qDebug()<< ui->combo_comList->currentIndex();

}
void Dialog_COM_Port::new_data_available(){
//    rxData.append(sp1->readAll());
//    qDebug() << rxData;
}
