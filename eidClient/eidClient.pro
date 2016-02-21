#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T20:42:54
#
#-------------------------------------------------

QT       += core gui
QT       += network

LIBS     +=  -L/root/openssl-1.0.1f -lQtCore -lssl -lcrypto

INCLUDEPATH += /root/openssl-1.0.1f/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eidClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pindlg.cpp

HEADERS  += mainwindow.h \
    pindlg.h

FORMS    += mainwindow.ui \
    pindlg.ui
