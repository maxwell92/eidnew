/********************************************************************************
** Form generated from reading UI file 'pindlg.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PINDLG_H
#define UI_PINDLG_H

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

class Ui_pindlg
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *pinlineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *pindlg)
    {
        if (pindlg->objectName().isEmpty())
            pindlg->setObjectName(QString::fromUtf8("pindlg"));
        pindlg->resize(344, 183);
        label = new QLabel(pindlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 30, 191, 16));
        label_2 = new QLabel(pindlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 79, 111, 31));
        pinlineEdit = new QLineEdit(pindlg);
        pinlineEdit->setObjectName(QString::fromUtf8("pinlineEdit"));
        pinlineEdit->setGeometry(QRect(200, 80, 113, 25));
        pinlineEdit->setEchoMode(QLineEdit::Password);
        pushButton = new QPushButton(pindlg);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(50, 130, 80, 26));
        pushButton_2 = new QPushButton(pindlg);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(190, 130, 80, 26));

        retranslateUi(pindlg);

        QMetaObject::connectSlotsByName(pindlg);
    } // setupUi

    void retranslateUi(QDialog *pindlg)
    {
        pindlg->setWindowTitle(QApplication::translate("pindlg", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("pindlg", "eID Client", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("pindlg", "Enter your PIN:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("pindlg", "Submit", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("pindlg", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class pindlg: public Ui_pindlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PINDLG_H
