#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T17:24:51
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += mysql
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS     +=  -L/root/openssl-1.0.1f -lQtCore -lssl -lcrypto

INCLUDEPATH += /root/openssl-1.0.1f/include

TARGET = SP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    person.cpp

HEADERS  += mainwindow.h \
    person.h

FORMS    += mainwindow.ui
