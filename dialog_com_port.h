#ifndef DIALOG_COM_PORT_H
#define DIALOG_COM_PORT_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>

namespace Ui {
    class Dialog_COM_Port;
    class MainWindow;
}

class Dialog_COM_Port : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_COM_Port(QWidget *parent = nullptr);
    void set_serialport(QSerialPort *sp);
    void port_setings_init();

    ~Dialog_COM_Port();

signals:
    void sendMsg(QString msg);

private slots:
    void on_toggle_open_button_clicked();
    void on_combo_comList_currentIndexChanged(const QString &arg1);
    void new_data_available();


private:
    Ui::Dialog_COM_Port *ui;
    QList<QSerialPortInfo> ap;      // available Ports
    QSerialPort *sp1;               // serial Port
    struct currentComParam{
        QString comPort;
        QString Baudrat;
        QString Databits;
        QString Parity;
        QString Stopbits;
    };
};

#endif // DIALOG_COM_PORT_H
