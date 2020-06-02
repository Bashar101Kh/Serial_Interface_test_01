#ifndef PORT_READER_H
#define PORT_READER_H

#include<QByteArray>

 class Port_Reader
{
public:
    Port_Reader();
private:
    QByteArray rx_data;
};

#endif // PORT_READER_H
