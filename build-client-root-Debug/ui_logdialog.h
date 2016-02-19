/********************************************************************************
** Form generated from reading UI file 'logdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGDIALOG_H
#define UI_LOGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_logDialog
{
public:
    QLineEdit *usernameLinedit;
    QLineEdit *passwdLinedit;
    QPushButton *LoginBtn;
    QPushButton *RegBtn;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *logDialog)
    {
        if (logDialog->objectName().isEmpty())
            logDialog->setObjectName(QString::fromUtf8("logDialog"));
        logDialog->resize(400, 300);
        usernameLinedit = new QLineEdit(logDialog);
        usernameLinedit->setObjectName(QString::fromUtf8("usernameLinedit"));
        usernameLinedit->setGeometry(QRect(200, 90, 113, 25));
        passwdLinedit = new QLineEdit(logDialog);
        passwdLinedit->setObjectName(QString::fromUtf8("passwdLinedit"));
        passwdLinedit->setGeometry(QRect(200, 160, 113, 25));
        passwdLinedit->setEchoMode(QLineEdit::Password);
        LoginBtn = new QPushButton(logDialog);
        LoginBtn->setObjectName(QString::fromUtf8("LoginBtn"));
        LoginBtn->setGeometry(QRect(20, 230, 161, 26));
        RegBtn = new QPushButton(logDialog);
        RegBtn->setObjectName(QString::fromUtf8("RegBtn"));
        RegBtn->setGeometry(QRect(200, 230, 171, 26));
        label = new QLabel(logDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 100, 121, 16));
        label_2 = new QLabel(logDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 160, 141, 16));
        label_3 = new QLabel(logDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(70, 20, 311, 16));

        retranslateUi(logDialog);

        QMetaObject::connectSlotsByName(logDialog);
    } // setupUi

    void retranslateUi(QDialog *logDialog)
    {
        logDialog->setWindowTitle(QApplication::translate("logDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        LoginBtn->setText(QApplication::translate("logDialog", "Login", 0, QApplication::UnicodeUTF8));
        RegBtn->setText(QApplication::translate("logDialog", "Registration", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("logDialog", "Username: ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("logDialog", "SecurityKey: ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("logDialog", "Student Information Management System", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class logDialog: public Ui_logDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGDIALOG_H
