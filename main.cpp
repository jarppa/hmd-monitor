#include "mainwindow.h"
#include <QApplication>

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSerialPort serialPort;
    QString serialPortName = "COM12";
    serialPort.setPortName(serialPortName);

    if (!serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }

    int serialPortBaudRate = QSerialPort::Baud9600;
    if (!serialPort.setBaudRate(serialPortBaudRate)) {
        qDebug() << QObject::tr("Failed to set 9600 baud for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }

    if (!serialPort.setDataBits(QSerialPort::Data8)) {
        qDebug() << QObject::tr("Failed to set 8 data bits for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }

    if (!serialPort.setParity(QSerialPort::NoParity)) {
        qDebug() << QObject::tr("Failed to set no parity for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }

    if (!serialPort.setStopBits(QSerialPort::OneStop)) {
        qDebug() << QObject::tr("Failed to set 1 stop bit for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }

    if (!serialPort.setFlowControl(QSerialPort::NoFlowControl)) {
        qDebug() << QObject::tr("Failed to set no flow control for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }

    /*QFile dataFile;
    if (!dataFile.open(stdin, QIODevice::ReadOnly)) {
        qDebug() << QObject::tr("Failed to open stdin for reading") << endl;
        return 1;
    }

    QByteArray writeData(dataFile.readAll());
    dataFile.close();

    if (writeData.isEmpty()) {
        qDebug() << QObject::tr("Either no data was currently available on the standard input for reading, or an error occurred for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }*/

    //SerialPortWriter serialPortWriter(&serialPort);
    //serialPortWriter.write(writeData);

    MainWindow w(&serialPort);
    w.show();

    return a.exec();
}
