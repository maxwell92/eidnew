#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <person.h>
#include <openssl/rsa.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void DecryptK0();
    void DepackReg();
    void Enpack(char code[]);
    void EncryptK0();
    void ResponseCli();
    int existDB(char name[]);
    void saveDB();
    void sendErr();
    void sendSucc();

    void rsaKey();
    int VerifyUsername();
    void makePKeyC();
    void rsaEnpack(char code[]);
    void SendPubS();
    void SendClient();
    void SendNoExist();
    void DecryptPriS();
    void Depacklog();
    void hash(char in[], char out[]);
    void fillLogger();
    void makeIdList();
    void getM1();
    void hashn0();
    void logEnpack(char code[]);
    void EncryptPubC();
    void sendIdlist();
    void getK1();
    void gety1();
    void DecryptK1();

    void DecrypteidPriS();
    void sendCli();
    

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::MainWindow *ui;
    person *logger;
    person *regger;

    QTcpServer *regServer;
    QTcpSocket *regSocket;

    QTcpServer *logServer;
    QTcpSocket *logSocket;

    QTcpServer *eidServer;
    QTcpSocket *eidSocket;

    RSA *publicKey;
    RSA *privateKey;
    unsigned char PublicKey[512];

    RSA *pubC;
    unsigned char PKeyC[512]; //use to make pubC;

    //client->sp code:01
//    char *cipher1;
    char cipher1[256];
    int len_cipher1;
//    char *msg1;
    char msg1[256];
    int len_msg1;

    //sp->client code:02
  //  char *cipher2;
    char cipher2[256];
    int len_cipher2;
//    char *msg2;
    char msg2[256];
    int len_msg2;

    //client->sp code:03

    char *msg3;
    int len_msg3;

    //pubC
//    char *msg4;
    char msg4[256];
    int len_msg4;

    //pubS
    char *msg5;
    int len_msg5;

    //client->sp code:01
//    char *cipher3;
    char cipher3[256];
    int len_cipher3;
//    char *msg6;
    char msg6[256];
    int len_msg6;

    char n0[2];
    char n1[2];
    char n2[2];
    char hn1[33];
    char hn2[33];
    char idlist[5];

    //sp->client code:02
    char *cipher4;
    int len_cipher4;
    char *msg7;
    int len_msg7;

    char cipher5[256];
    int len_cipher5;
    char msg8[256];
    int len_msg8;

    char cipher6[256];
    int len_cipher6;
    char msg9[256];
    int len_msg9;

private slots:
    void newReg();
    void recvReg();

    void newLog();
    void recvLog();

    void newEid();
    void recvEid();
};

#endif // MAINWINDOW_H
