#include "pindlg.h"
#include "ui_pindlg.h"
#include "QMessageBox"

pindlg::pindlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pindlg)
{
    ui->setupUi(this);

    memset(pin, '\0', 10);
    memcpy(pin, "123456", 6);
}

pindlg::~pindlg()
{
    delete ui;
}

void pindlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void pindlg::on_pushButton_clicked()
{
    QString pinInput = ui->pinlineEdit->text();
    if(!strcmp(pin, pinInput.toLatin1().data()))
    {
        emit(ShowMain());
        this->hide();
    }
    else
    {
        QMessageBox::about(this, "Attention!", "WRONG pin!");
    }
}

void pindlg::on_pushButton_2_clicked()
{
    this->destroy();
}
