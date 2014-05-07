#-------------------------------------------------
#
# Project created by QtCreator 2014-05-05T09:21:20
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HMD-Monitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    indicatorellipse.cpp

HEADERS  += mainwindow.h \
    indicatorellipse.h

FORMS    += mainwindow.ui

OTHER_FILES +=
