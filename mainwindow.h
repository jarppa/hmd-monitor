#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>

#include <QGraphicsScene>

#include "indicatorellipse.h"

QT_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void write(const QByteArray &writeData);

private slots:
    void handleBytesWritten(qint64 bytes);
    void handleReadyRead();
    void handleWriteTimeout();
    void handleReadTimeout();
    void handleError(QSerialPort::SerialPortError error);

    void on_actionConnect_triggered();

private:
    void processReadData();
    void processLine(QByteArray *a);
    void updateValues();
    void openSerialport(QString name);

    Ui::MainWindow *ui;
    QSerialPort     *m_serialPort;
    QByteArray      m_writeData;
    QByteArray  m_readData;
    QByteArray outputBuffer;
    qint64          m_bytesWritten;
    QTimer          m_readtimer;
    QTimer          m_writetimer;

    QString mAmbient;
    QString mHeading;
    QString mAccX;
    QString mAccY;
    QString mAccZ;

    QGraphicsScene *scene;
    IndicatorEllipse *ellipse;
};
#endif // MAINWINDOW_H
