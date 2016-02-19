/********************************************************************************
** Form generated from reading UI file 'regdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGDIALOG_H
#define UI_REGDIALOG_H

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

class Ui_regDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *UsernameLinedit;
    QLineEdit *eidnumLinedit;
    QLineEdit *skeyLinedit;
    QLineEdit *skeyLinedit2;
    QPushButton *SubmitBtn;
    QPushButton *ResetBtn;

    void setupUi(QDialog *regDialog)
    {
        if (regDialog->objectName().isEmpty())
            regDialog->setObjectName(QString::fromUtf8("regDialog"));
        regDialog->resize(400, 325);
        label = new QLabel(regDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 20, 241, 16));
        label_2 = new QLabel(regDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 80, 141, 16));
        label_3 = new QLabel(regDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 120, 161, 20));
        label_4 = new QLabel(regDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 170, 151, 16));
        label_5 = new QLabel(regDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 210, 151, 16));
        UsernameLinedit = new QLineEdit(regDialog);
        UsernameLinedit->setObjectName(QString::fromUtf8("UsernameLinedit"));
        UsernameLinedit->setGeometry(QRect(230, 70, 113, 25));
        eidnumLinedit = new QLineEdit(regDialog);
        eidnumLinedit->setObjectName(QString::fromUtf8("eidnumLinedit"));
        eidnumLinedit->setGeometry(QRect(230, 110, 113, 25));
        skeyLinedit = new QLineEdit(regDialog);
        skeyLinedit->setObjectName(QString::fromUtf8("skeyLinedit"));
        skeyLinedit->setGeometry(QRect(230, 160, 113, 25));
        skeyLinedit->setEchoMode(QLineEdit::Password);
        skeyLinedit2 = new QLineEdit(regDialog);
        skeyLinedit2->setObjectName(QString::fromUtf8("skeyLinedit2"));
        skeyLinedit2->setGeometry(QRect(230, 210, 113, 25));
        skeyLinedit2->setEchoMode(QLineEdit::Password);
        SubmitBtn = new QPushButton(regDialog);
        SubmitBtn->setObjectName(QString::fromUtf8("SubmitBtn"));
        SubmitBtn->setGeometry(QRect(80, 260, 80, 26));
        ResetBtn = new QPushButton(regDialog);
        ResetBtn->setObjectName(QString::fromUtf8("ResetBtn"));
        ResetBtn->setGeometry(QRect(230, 260, 80, 26));

        retranslateUi(regDialog);

        QMetaObject::connectSlotsByName(regDialog);
    } // setupUi

    void retranslateUi(QDialog *regDialog)
    {
        regDialog->setWindowTitle(QApplication::translate("regDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("regDialog", "User Registration", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("regDialog", "Username: ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("regDialog", "eID Number: ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("regDialog", "SecurityKey: ", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("regDialog", "SecurityKey Again: ", 0, QApplication::UnicodeUTF8));
        SubmitBtn->setText(QApplication::translate("regDialog", "Submit", 0, QApplication::UnicodeUTF8));
        ResetBtn->setText(QApplication::translate("regDialog", "Reset", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class regDialog: public Ui_regDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGDIALOG_H
