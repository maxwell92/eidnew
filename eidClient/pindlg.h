#ifndef PINDLG_H
#define PINDLG_H

#include <QDialog>

namespace Ui {
class pindlg;
}

class pindlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit pindlg(QWidget *parent = 0);
    ~pindlg();

    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::pindlg *ui;

    char pin[10];
signals:
    void ShowMain();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // PINDLG_H
