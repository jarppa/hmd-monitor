#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "indicatorellipse.h"

#include <QInputDialog>
#include <QDir>
#include <QSerialPort>
#include <math.h>

#include <QDebug>

#define Y_ACC_THRESHOLD 0.5f
#define X_ACC_THRESHOLD 0.5f

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bytesWritten(0)
{    
    m_serialPort = new QSerialPort();

    m_writetimer.setSingleShot(true);
    connect(m_serialPort, SIGNAL(bytesWritten(qint64)), SLOT(handleBytesWritten(qint64)));
    connect(m_serialPort, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&m_readtimer, SIGNAL(timeout()), SLOT(handleReadTimeout()));
    connect(&m_writetimer, SIGNAL(timeout()), SLOT(handleWriteTimeout()));

    //m_readtimer.start(5000);

    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QPen p(Qt::red);
    p.setWidth(2);

    QBrush b(Qt::red);

    //ellipse = scene->addEllipse(10, 10, 30, 30, p, b);
    ellipse = new IndicatorEllipse();
    //ellipse = scene->itemAt(0,0);
    scene->addItem(ellipse);

}

MainWindow::~MainWindow()
{
    if (m_serialPort->isOpen())
        m_serialPort->close();
    delete ui;
}


void MainWindow::handleReadyRead()
{
    m_readData.append(m_serialPort->readAll());
    processReadData();
    /*if (!m_readtimer.isActive())
        m_readtimer.start(5000);*/

}

void MainWindow::handleBytesWritten(qint64 bytes)
{
    m_bytesWritten += bytes;
    if (m_bytesWritten == m_writeData.size()) {
        m_bytesWritten = 0;
        qDebug() << QObject::tr("Data successfully sent to port %1").arg(m_serialPort->portName()) << endl;
        //QCoreApplication::quit();
    }
}

void MainWindow::handleWriteTimeout()
{
    qDebug() << QObject::tr("Operation timed out for port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
    //QCoreApplication::exit(1);
}

void MainWindow::handleReadTimeout()
{
    if (m_readData.isEmpty()) {
        qDebug() << QObject::tr("No data was currently available for reading from port %1").arg(m_serialPort->portName()) << endl;
    } else {
        qDebug() << QObject::tr("Data successfully received from port %1").arg(m_serialPort->portName()) << endl;
        qDebug() << m_readData << endl;
    }
}

void MainWindow::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::WriteError) {
        qDebug() << QObject::tr("An I/O error occurred while writing the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        //QCoreApplication::exit(1);
    }

    if (serialPortError == QSerialPort::ReadError) {
           qDebug() << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
           //QCoreApplication::exit(1);
       }

}

void MainWindow::write(const QByteArray &writeData)
{
    m_writeData = writeData;

    qint64 bytesWritten = m_serialPort->write(writeData);

    if (bytesWritten == -1) {
        qDebug() << QObject::tr("Failed to write the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        //QCoreApplication::exit(1);
    } else if (bytesWritten != m_writeData.size()) {
        qDebug() << QObject::tr("Failed to write all the data to port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        //QCoreApplication::exit(1);
    }

    m_writetimer.start(5000);
}

void MainWindow::processReadData() {
    QByteArray last;
    QList<QByteArray> lines = m_readData.split('\n');
    foreach (QByteArray a, lines) {
        //qDebug() << a;
        if (a.endsWith('\r')) {
            processLine(&a);
        }else if (!a.endsWith('\r') && m_readData.endsWith(a)) {
            last = a;
        }else
            qDebug() << "HAAAALP! Data corruption!";

    }
    m_readData.clear();
    m_readData.append(last);
}

void MainWindow::processLine(QByteArray *a) {
    if (a->startsWith("AMBIENT:")) {
        mAmbient = a->split(':')[1];

    }else if (a->startsWith("HEADING:")) {
        mHeading = a->split(':')[1];

    }else if (a->startsWith("ACCELERATION:")) {
        QByteArray vals = a->split(':')[1];
        mAccX = vals.split(';')[0];
        mAccY = vals.split(';')[1];
        mAccZ = vals.split(';')[2];
    }
    else if (a->startsWith("CALIBRATION:")) {
        QString cXmin, cXmax, cYmin, cYmax;
        QByteArray vals = a->split(':')[1];
        cXmin = vals.split(';')[0];
        cXmax = vals.split(';')[1];
        cYmin = vals.split(';')[2];
        cYmax = vals.split(';')[3];
    }
    updateValues();
}


static float meanOf(float *buf, int size) {
    float ret = 0.0f;
    for(int i=0;i<size;i++) {
        ret += *buf++;
    }
    return ret/(float)size;
}

static int x_offset = 0;
static int y_offset = 0;
static const int rb_size = 2;
static float x_acc_rbuf [rb_size];
static float y_acc_rbuf [rb_size];
static int i_yrb;
static int i_xrb;

void MainWindow::updateValues() {
    ui->heading_te->setText(mHeading);
    ui->ambient_te->setText(mAmbient);
    ui->accX_te->setText(mAccX);
    ui->accY_te->setText(mAccY);
    ui->accZ_te->setText(mAccZ);
    ellipse->updateIndicator(mHeading.toDouble(), mAmbient.toUInt(),  mAccZ.toFloat());

    /*int y_offset = (scene->height()/2) + ((mAccY.toFloat()/2.0f)*(scene->height()/2));

    int x_offset = (scene->width()/2) + ((mAccX.toFloat()/1.5f)*(scene->width()/2));*/

    i_yrb = (i_yrb+1) % rb_size;
    i_xrb = (i_xrb+1) % rb_size;

    y_acc_rbuf[i_yrb] = mAccY.toFloat();
    x_acc_rbuf[i_xrb] = mAccX.toFloat();

    y_offset = (scene->height()/2) + ((meanOf(&y_acc_rbuf[0],rb_size)/2.0f)*(scene->height()/2));
    x_offset = (scene->width()/2) + ((meanOf(&x_acc_rbuf[0],rb_size)/2.0f)*(scene->width()/2));

    //qDebug() << x_offset;
    //qDebug() << scene->width();
    scene->setSceneRect(ui->graphicsView->rect());
    ellipse->setPos(x_offset, y_offset);
}

void MainWindow::on_actionConnect_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                            tr("Serial port:"), QLineEdit::Normal,
                                            QDir::home().dirName(), &ok);
    /*if (ok && !text.isEmpty())
        textLabel->setText(text);*/
    openSerialport(text);
}

void MainWindow::openSerialport(QString name) {

    if (m_serialPort->isOpen())
        m_serialPort->close();
    //QString serialPortName = "COM12";
    m_serialPort->setPortName(name);

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << QObject::tr("Failed to open port %1, error: %2").arg(name).arg(m_serialPort->errorString()) << endl;
        //return 1;
    }

    int serialPortBaudRate = QSerialPort::Baud9600;
    if (!m_serialPort->setBaudRate(serialPortBaudRate)) {
        qDebug() << QObject::tr("Failed to set 9600 baud for port %1, error: %2").arg(name).arg(m_serialPort->errorString()) << endl;
        //return 1;
    }

    if (!m_serialPort->setDataBits(QSerialPort::Data8)) {
        qDebug() << QObject::tr("Failed to set 8 data bits for port %1, error: %2").arg(name).arg(m_serialPort->errorString()) << endl;
        //return 1;
    }

    if (!m_serialPort->setParity(QSerialPort::NoParity)) {
        qDebug() << QObject::tr("Failed to set no parity for port %1, error: %2").arg(name).arg(m_serialPort->errorString()) << endl;
        //return 1;
    }

    if (!m_serialPort->setStopBits(QSerialPort::OneStop)) {
        qDebug() << QObject::tr("Failed to set 1 stop bit for port %1, error: %2").arg(name).arg(m_serialPort->errorString()) << endl;
        //return 1;
    }

    if (!m_serialPort->setFlowControl(QSerialPort::NoFlowControl)) {
        qDebug() << QObject::tr("Failed to set no flow control for port %1, error: %2").arg(name).arg(m_serialPort->errorString()) << endl;
        //return 1;
    }
}
