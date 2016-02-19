#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <logdialog.h>
#include <regdialog.h>
#include <user.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::MainWindow *ui;

    logDialog *logdlg;
    regDialog *regdlg;
    user *logger;

private slots:
    void onShowReg();
    void onShowMain();
    void onShowLog();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
