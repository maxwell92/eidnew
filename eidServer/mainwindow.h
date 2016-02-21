#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <openssl/rsa.h>
#include <people.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTcpServer *eidServer;
    QTcpSocket *eidSocket;
    QTcpSocket *spSocket;




    //eidServer->eidClient pubEid
    char msg1[256];
    int len_msg1;

    RSA *publicKey;
    RSA *privateKey;
    unsigned char PublicKey[512];

    RSA *pubS;
    unsigned char PkeyS[512]; //use to make pubS;

    //eidclient->eidserver msg
    char msg2[256];
    int len_msg2;
    char cipher1[256];
    int len_cipher1;

    char userMsg[256];
    char msgHash[33];

    //sp->eidServer
    char msg3[256];
    int len_msg3;

    people *someone;

    //eidserver->sp
    char msg4[256];
    int len_msg4;

    char cipher2[256];
    int len_cipher2;

    char hn2[33];
    char ack[2];

private slots:
    void getRsa();
    void newClient();
    void recvCli();
    void rsaEnpack();
    void sendRsa();
    void recvSP();

    void DecryptPrieid();
    void Depack();
    int  Verify();
    void getPubS();
    void Enpack();
    void EncryptPubS(char code[]);
    void connectSP();
    void sendSP();
    void makePubS();
    void hash(char in[], char out[]);
    void fillSomeone(char eidNumber[]);

    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
