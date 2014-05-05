#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>

QT_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSerialPort *serialPort, QWidget *parent = 0);
    ~MainWindow();
    void write(const QByteArray &writeData);

private slots:
    void handleBytesWritten(qint64 bytes);
    void handleReadyRead();
    void handleWriteTimeout();
    void handleReadTimeout();
    void handleError(QSerialPort::SerialPortError error);

private:
    void processReadData();
    void processLine(QByteArray *a);
    void updateValues();

    Ui::MainWindow *ui;
    QSerialPort     *m_serialPort;
    QByteArray      m_writeData;
    QByteArray  m_readData;
    QByteArray outputBuffer;
    QTextStream     m_standardOutput;
    qint64          m_bytesWritten;
    QTimer          m_readtimer;
    QTimer          m_writetimer;

    QString mAmbient;
    QString mHeading;
    QString mAccX;
    QString mAccY;
    QString mAccZ;
};
#endif // MAINWINDOW_H
