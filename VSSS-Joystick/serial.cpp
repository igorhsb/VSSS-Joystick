#include "serial.h"

Serial::Serial()
{

}

Serial::~Serial()
{
  if(serial->isOpen() && serial->isWritable())
  {
    serial->close();
    delete serial;
    serial = nullptr;
  }
}

void Serial::close()
{
  cout << "Serial closed!" << endl;
  serial->close();
}

bool Serial::connectSerial(QString porta)
{
  serial = new QSerialPort();
  serial->setPortName(porta);
  serial->open(QIODevice::ReadWrite);
  serial->setBaudRate(QSerialPort::Baud115200);
  serial->setDataBits(QSerialPort::Data8);
  serial->setParity(QSerialPort::NoParity);
  serial->setStopBits(QSerialPort::OneStop);
  sleep(0.01);
  if(serial->isOpen() && serial->isWritable())
  {
    cout << "Serial opened!" << endl;
    return true;
  }
  else
  {
    cerr << "Serial could not be opened!" << endl;
    return false;
  }
}

void Serial::sendMsg(int channel, int velLeft, int velRight)
{
    if(serial->isOpen() && serial->isWritable())
    {

        QByteArray bytes(7, 0x0);
        int restoL, restoR, intR, intL;
        intL = velLeft/10;
        restoL = velLeft % 10;
        intR = velRight/10;
        restoR = velRight % 10;

        bytes[0] = (char)channel;
        if(velLeft < 0)
            bytes[1] = 1;
        else
            bytes[1] = 0;

        bytes[2] = (char)fabs(intL);
        bytes[3] = (char)fabs(restoL);

        if(velRight < 0)
            bytes[4] = 1;
        else
            bytes[4] = 0;

        bytes[5] = (char)fabs(intR);
        bytes[6] = (char)fabs(restoR);

//        char *aux = bytes.data();
//            cout << "Inicio" << endl;
//            cout << "l: " << velLeft << endl;
//            cout << "r: " << velRight << endl;
//            for(int k = 0; k < 7; k++)
//                printf("%d\n",aux[k]);

//            cout << "end" << endl;

        serial->write(bytes);
        serial->waitForBytesWritten(5);
        serial->flush();
    }
    else{
        cerr << "Serial could not write wheel speeds" << endl;
    }
}

bool Serial::isOpen()
{
  bool isOpen = serial->isOpen() && serial->isWritable();
  return isOpen;
}
