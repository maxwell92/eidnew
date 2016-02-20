#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>
#include <user.h>
#include <QtNetwork/QTcpSocket>
#include <openssl/rsa.h>

namespace Ui {
class logDialog;
}

class logDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit logDialog(QWidget *parent = 0);
    ~logDialog();
    
protected:
    void changeEvent(QEvent *e);
    
private slots:


    void on_LoginBtn_clicked();

    void on_RegBtn_clicked();
    void recvSP();
    void CreateLogger(QString username, QString skey);
    void rsaKey();
    void ExchangeRSAKey();
    void Enpack(char code[]);
    void connectSP();
    void sendRSA();
    void makePubS();
    void Enpacklog(char code[]);
    void getN0();
    void EncryptpubS(char code[]);
    void sendLog(char code[]);
    void DecryptPriC();
    void Depack();
    void fillLogger();
    void dealIdlist();

signals:
    void showReg();
    void showMain();

private:
    Ui::logDialog *ui;

    user *logger;
    QTcpSocket *logSocket;
    RSA *privateKey;  //priC
    RSA *publicKey;   //pubC
    unsigned char PublicKey[512];

    RSA *pubS;       //pubS
    unsigned char PkeyS[512]; //use to make pubS;

    char *msg1;    //rsa pubkey exchange and verify username
    int len_msg1;

//    char *msg2;     //recv pubkey
    char msg2[256];
    int len_msg2;

    char n0[2];
    char n1[2];
    char n2[2];
    char hn1[33];
    char idlist[5];

    //client->SP :01
    char *msg3;
    int len_msg3;
    char *cipher1;
    int len_cipher1;


    //SP->client :02
//    char *msg4;
//    int len_msg4;
//    char *cipher2;
//    int len_cipher2;
    char msg4[256];
    int len_msg4;
    char cipher2[256];
    int len_cipher2;
};

#endif // LOGDIALOG_H
