#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    logdlg = new logDialog(this);
    logdlg->show();
    regdlg = new regDialog(this);

    connect(logdlg, SIGNAL(showReg()), this, SLOT(onShowReg()));
    connect(logdlg, SIGNAL(showMain()), this, SLOT(onShowMain()));
    connect(regdlg, SIGNAL(showLog()), this, SLOT(onShowLog()));

    logger = new user();

}

void MainWindow::onShowReg()
{
    regdlg->show();
}

void MainWindow::onShowMain()
{
    char name[] = "maxwell";
    ui->label_2->setText(QString(QLatin1String(name)));
    this->show();
    qDebug()<<"[onShowMain]: "<<logger->username;
}

void MainWindow::onShowLog()
{
    logdlg->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    this->destroy();
}
