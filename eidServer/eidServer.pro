#-------------------------------------------------
#
# Project created by QtCreator 2016-02-21T03:03:42
#
#-------------------------------------------------

QT       += core gui
QT       += network

QT       += mysql
QT       += sql

LIBS     +=  -L/root/openssl-1.0.1f -lQtCore -lssl -lcrypto

INCLUDEPATH += /root/openssl-1.0.1f/include


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eidServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    people.cpp

HEADERS  += mainwindow.h \
    people.h

FORMS    += mainwindow.ui
