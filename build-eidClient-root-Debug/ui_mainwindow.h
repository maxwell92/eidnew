/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_8;
    QPushButton *pushButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(20, 40, 111, 21));
        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(200, 40, 131, 21));
        checkBox_3 = new QCheckBox(centralWidget);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(20, 90, 85, 21));
        checkBox_4 = new QCheckBox(centralWidget);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setGeometry(QRect(200, 90, 111, 21));
        checkBox_5 = new QCheckBox(centralWidget);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));
        checkBox_5->setGeometry(QRect(20, 140, 85, 21));
        checkBox_6 = new QCheckBox(centralWidget);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));
        checkBox_6->setGeometry(QRect(200, 140, 131, 21));
        checkBox_7 = new QCheckBox(centralWidget);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));
        checkBox_7->setGeometry(QRect(20, 190, 91, 21));
        checkBox_8 = new QCheckBox(centralWidget);
        checkBox_8->setObjectName(QString::fromUtf8("checkBox_8"));
        checkBox_8->setGeometry(QRect(200, 190, 85, 21));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(150, 240, 80, 26));
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "teleNumber", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("MainWindow", "realName", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("MainWindow", "age", 0, QApplication::UnicodeUTF8));
        checkBox_4->setText(QApplication::translate("MainWindow", "company", 0, QApplication::UnicodeUTF8));
        checkBox_5->setText(QApplication::translate("MainWindow", "gender", 0, QApplication::UnicodeUTF8));
        checkBox_6->setText(QApplication::translate("MainWindow", "address", 0, QApplication::UnicodeUTF8));
        checkBox_7->setText(QApplication::translate("MainWindow", "city", 0, QApplication::UnicodeUTF8));
        checkBox_8->setText(QApplication::translate("MainWindow", "idNumber", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Submit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
