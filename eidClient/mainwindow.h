#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pindlg.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
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

    char realName[33];
    char teleNumber[14];
    char age[3];
    char company[50];
    char gender[2];
    char address[100];
    char city[10];
    char idNumber[19];
    char eidNumber[9];

    void getInfo();
    void Enpack();
    void EncryptPubeid();
    void connecteIDServer();
    void sendeIDServer(char code[]);
    void hash(char in[], char out[]);


    pindlg *pdlg;
    QTcpServer *clientServer;
    QTcpSocket *clientSocket;

    QTcpSocket *eidSocket;
    char hn2[33];
    char idlist[10];
//    char idmsg[256];
    int cnt;

    RSA *pubES;
    unsigned char PkeyES[512];
    //eidserver->eidcient
    char msg1[256];
    int len_msg1;
    //eidclient->eidserver
    char idmsg[256];
    char msg2[256];
    int len_msg2;
    char cipher1[256];
    int len_cipher1;
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::MainWindow *ui;
private slots:
    void onShowMain();
    void on_pushButton_clicked();

    void newClient();
    void recvCli();
    void recvEid();
    void makePubES();
};

#endif // MAINWINDOW_H
