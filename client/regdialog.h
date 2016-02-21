#ifndef REGDIALOG_H
#define REGDIALOG_H

#include <QDialog>
#include <user.h>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class regDialog;
}

class regDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit regDialog(QWidget *parent = 0);
    ~regDialog();

    void createTmpUser(QString username, QString eidNum, QString skey);
    void connectSP();
    void sendSP();


    void getMfromUsername(char *username);
    void EncryptK0();
    void Enpack(char code[]);
    void DecryptK0();
    void Depack();
    int Verify();
    void caly0();
    void hash(char in[], char out[]);
    void SendAck();
    void Encrypt1K0();
    
protected:
    void changeEvent(QEvent *e);

signals:
    void showLog();

private slots:


    void on_ResetBtn_clicked();

    void on_SubmitBtn_clicked();

    void acceptSP();

private:
    Ui::regDialog *ui;
    user *tmpUser;
    user *verUser;
    QTcpSocket *client;

    char *msg1;
    int len_msg1;
    char *cipher1;
    int len_cipher1;

//    char *cipher2;
    char cipher2[256];
    int len_cipher2;
//    char *msg2;
    char msg2[256];
    int len_msg2;


//    char *cipher3;
    char cipher3[256];
    int len_cipher3;
//    char *msg3;
    char msg3[256];
    int len_msg3;
};

#endif // REGDIALOG_H
