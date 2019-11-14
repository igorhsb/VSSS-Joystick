#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <time.h>
#include "math.h"
#include "iostream"
#include "fstream"

#include <QFile>
#include <QTextStream>
#include <QTextBrowser>
#include <QStandardItem>
#include <QString>
#include <QApplication>
#include <QMessageBox>


#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <unistd.h>

#include <QDebug>
#include <QDateTime>
#include <QDir>


using namespace std;

typedef union
{
  float  Float;
 unsigned char Bytes[4];
} Float2Char;

typedef union
{
  unsigned short Short;
 unsigned char Bytes[2];
}Short2Char;

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial();
    ~Serial();
    bool connectSerial(QString);
    void sendMsg(int, int, int);
    bool isOpen();
    void close();

private:
    QSerialPort *serial;
};

#endif // SERIAL_H
