#include "com_port_config_dialog.h"
#include "ui_com_port_config_dialog.h"

com_port_config_dialog::com_port_config_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::com_port_config_dialog)
{
    ui->setupUi(this);
}

com_port_config_dialog::~com_port_config_dialog()
{
    delete ui;
}
