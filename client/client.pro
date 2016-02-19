#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T11:50:26
#
#-------------------------------------------------

QT       += core gui
QT       += network

LIBS     +=  -L/root/openssl-1.0.1f -lQtCore -lssl -lcrypto

INCLUDEPATH += /root/openssl-1.0.1f/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logdialog.cpp \
    regdialog.cpp \
    user.cpp

HEADERS  += mainwindow.h \
    logdialog.h \
    regdialog.h \
    user.h

FORMS    += mainwindow.ui \
    logdialog.ui \
    regdialog.ui
