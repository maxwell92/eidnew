#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "string.h"
#include "QtNetwork/QTcpServer"
#include "QtNetwork/QTcpSocket"
#include "QtNetwork/QHostAddress"
#include "QMessageBox"
#include "openssl/md5.h"
#include "openssl/rsa.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pdlg = new pindlg(this);
    pdlg->show();

    clientServer = new QTcpServer(this);
    clientSocket = new QTcpSocket(this);
    eidSocket = new QTcpSocket(this);

    connect(clientServer,SIGNAL(newConnection()),this,SLOT(newClient()));

    if(!clientServer->isListening())
    {
        if(!clientServer->listen(QHostAddress::Any,13500))
        {
            QMessageBox::about(this,"Attention!","eIDClient not listening!");
        }
    }

    connect(eidSocket,SIGNAL(readyRead()),this,SLOT(recvEid()));

    connect(pdlg, SIGNAL(ShowMain()), this, SLOT(onShowMain()));

    memset(realName, '\0', 20);
    memset(teleNumber, '\0', 14);
    memset(age, '\0', 3);
    memset(company, '\0', 50);
    memset(gender, '\0', 2);
    memset(address, '\0', 100);
    memset(city, '\0', 10);
    memset(idNumber, '\0', 19);
    memset(eidNumber, '\0', 9);

    memcpy(realName, "TanLuyang", 9);
    memcpy(teleNumber, "13200001100", 14);
    memcpy(age, "25", 3);
    memcpy(company, "Xian University of Posts and Telecommunications", 50);
    memcpy(gender, "fm", 2);
    memcpy(address, "ShaanXi Xian Rd Hongzhuan", 100);
    memcpy(city, "Xian", 10);
    memcpy(idNumber, "61011219911122001X", 19);
    memcpy(eidNumber, "12345678", 8);

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


void MainWindow::recvEid()
{
    QString eidInfo1;
    eidInfo1 = eidSocket->readAll();

    char code[3];
    memset(code, '\0', 3);
    memcpy(code, eidInfo1.toLatin1().data(), 2);

    if(!strcmp(code, "02"))
    {
        memset(msg1, '\0', 256);
        len_msg1 = eidInfo1.length();
        memcpy(msg1, eidInfo1.toLatin1().data(), eidInfo1.length());

        memcpy((char *)PkeyES, msg1 + 2, 74);
        unsigned char insd[2];
        memset(insd, '\0', 2);
        memcpy(insd, msg1 + 2 + 74, 1);

        for(int i = 0; i < 74; i++)
        {
            if(PkeyES[i] == insd[0])
            {
                PkeyES[i] = 0;
            }
            printf("%d, ", PkeyES[i]);
        }
        printf("\n");

        makePubES();
        getInfo();
        Enpack();
        EncryptPubeid();
        sendeIDServer("03");
    }

}

void MainWindow::makePubES()
{
    unsigned char *PKey = PkeyES;
    int PublicKeyLen = 74;
    pubES = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, PublicKeyLen);
}

void MainWindow::newClient()
{
    clientSocket = clientServer->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(recvCli()));
    connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
}

void MainWindow::onShowMain()
{
    this->show();
}

void MainWindow::on_pushButton_clicked()
{

    connecteIDServer();
    sendeIDServer("01");

 //   this->destroy();
}

void MainWindow::recvCli()
{
    memset(hn2, '\0', 33);
    QString cliInfo = clientSocket->readAll();
    memcpy(hn2, cliInfo.toLatin1().data(), 32);
    qDebug()<<"[recvCli]: "<<hn2;
}

void MainWindow::getInfo()
{
    cnt = 0;
    memset(idlist, '\0', 10);
    if(ui->checkBox->isChecked()) //teleNumber
    {
        cnt++;
        idlist[0] = '1';
    }else{
        idlist[0] = '0';
    }

    if(ui->checkBox_2->isChecked()) //realName
    {
        cnt++;
        idlist[1] = '1';
    }else{
        idlist[1] = '0';
    }

    if(ui->checkBox_3->isChecked()) //age
    {
        cnt++;
        idlist[2] = '1';
    }else{
        idlist[2] = '0';
    }

    if(ui->checkBox_4->isChecked()) //company
    {
        cnt++;
        idlist[3] = '1';
    }else{
        idlist[3] = '0';
    }

    if(ui->checkBox_5->isChecked()) //gender
    {
        cnt++;
        idlist[4] = '1';
    }else{
        idlist[4] = '0';
    }

    if(ui->checkBox_6->isChecked()) //address
    {
        cnt++;
        idlist[5] = '1';
    }else{
        idlist[5] = '0';
    }

    if(ui->checkBox_7->isChecked()) //city
    {
        cnt++;
        idlist[6] = '1';
    }else{
        idlist[6] = '0';
    }

    if(ui->checkBox_8->isChecked()) //idNumber
    {
        cnt++;
        idlist[7] = '1';
    }else{
        idlist[7] = '0';
    }

    memset(idmsg, '\0', 256);   //2467......
    int len_idmsg = 0;

    if(idlist[0] == '1')
    {
        memcpy(idmsg + len_idmsg, teleNumber, 13);
        len_idmsg += 13;
    }
    if(idlist[1] == '1')
    {
        memcpy(idmsg + len_idmsg, realName, strlen(realName));
        len_idmsg += strlen(realName);
    }
    if(idlist[2] == '1')
    {
        memcpy(idmsg + len_idmsg, age, strlen(age));
        len_idmsg += strlen(age);
    }
    if(idlist[3] == '1')
    {
        memcpy(idmsg + len_idmsg, company, strlen(company));
        len_idmsg += strlen(company);
    }
    if(idlist[4] == '1')
    {
        memcpy(idmsg + len_idmsg, gender, strlen(gender));
        len_idmsg += strlen(gender);
    }
    if(idlist[5] == '1')
    {
        memcpy(idmsg + len_idmsg, address, strlen(address));
        len_idmsg += strlen(address);
    }
    if(idlist[6] == '1')
    {
        memcpy(idmsg + len_idmsg, city, strlen(city));
        len_idmsg += strlen(city);
    }
    if(idlist[7] == '1')
    {
        memcpy(idmsg + len_idmsg, idNumber, strlen(idNumber));
        len_idmsg += strlen(idNumber);
    }





}

void MainWindow::Enpack()
{
    char code[3];
    memset(code, '\0', 3);
    memcpy(code, "03", 2);
    char hashMsg[33];
    memset(hashMsg, '\0', 33);
    hash(idmsg, hashMsg);

    //len_msg2 = 2 + 1 + cnt + 32;
    len_msg2 = 2 + 8 + 32 + 8 + 32;
    char tmp[8];
    memset(tmp, '\0', 8);
    int len_tmp = 0;

//    for(int i = 0; i < cnt; i++)
//    {
//        if(idlist[i] == '1')
//        {
//            char s[2];
//            memset(s, '\0', 2);
//            s[0] = i + '0';
//            memcpy(tmp + len_tmp, s, 1);
//            len_tmp++;
//        }
//    }
    qDebug()<<"[Enpack]: "<<idlist;
    memset(msg2, '\0', 256);
    memcpy(msg2, code, 2);
//    char s[2];
//    memset(s, '\0', 2);
//    s[0] = cnt - '0';
//    memcpy(msg2 + 2, s, 1);
//    memcpy(msg2 + 2 + 1, tmp, len_tmp);

    for(int i = 0; i < 8; i++)
    {
        printf("%c, ", idlist[i]);
    }
    printf("\n");
    memcpy(msg2 + 2, idlist, 8);
    memcpy(msg2 + 2 + 8, hashMsg, 32);
    memcpy(msg2 + 2 + 8 + 32, eidNumber, 8);
    memcpy(msg2 + 2 + 8 + 32 + 8, hn2, 32);
    qDebug()<<"[Enpack]: "<<msg2;
    qDebug()<<"[Enpack]: "<<strlen(msg2);
}

void MainWindow::hash(char in[], char out[])
{
    char buf[33] = {'\0'};
    char tmp[3] = {'\0'};
    unsigned char md[16];

    QString str;
    QByteArray b;
    b = str.toLatin1();

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, in, strlen(in));
    MD5_Final(md, &ctx);

    for(int i = 0; i < 16; i++ )
    {
        sprintf(tmp, "%2.2x", md[i]);
        strcat(buf, tmp);
    }

    strcpy(out, buf);
}

void MainWindow::EncryptPubeid()
{
    int num;
    num = len_msg2 / 50 + 1;
    len_cipher1 = num * 64 + 1 + 1;
//    cipher1 = (char *)malloc(len_cipher1 * sizeof(char));
//    memset(cipher1, '\0', len_cipher1);
    memset(cipher1, '\0', 256);

    unsigned char InputBuff[65], OutputBuff[65];
    int i;
    for(i = 0; i < num; i++) {
        memset((char *)InputBuff, '\0', 65);
        memset((char *)OutputBuff, '\0', 65);

        if (i == (num - 1)){
            memcpy((char *)InputBuff, msg2 + 50 * i, len_msg2 % 50);
        } else{
            memcpy((char *)InputBuff, msg2 + 50 * i, 50);
        }

        int clen = 0;
        qDebug()<<"[Encryptpubeid]: InputBuff["<<i<<"]: "<<InputBuff<<endl;
        qDebug()<<"[Encryptpubeid]: InputBuff["<<i<<"]: "<<strlen((char *)InputBuff)<<endl;

        clen = RSA_public_encrypt(50,  (const unsigned char *)InputBuff, OutputBuff, pubES, RSA_PKCS1_PADDING);
        qDebug()<<"[Encryptpubeid]: OutputBuff["<<i<<"]: "<<OutputBuff<<endl;
        qDebug()<<"[Encryptpubeid]: OutputBuff["<<i<<"]: "<<clen<<endl;
        //strcat(cipher1, (char *)OutputBuff);
        memcpy(cipher1 + 64 * i, (char *)OutputBuff, 64);
        qDebug()<<"[EncryptPubeid]: "<<strlen(cipher1);
    }

    int cnt = 0;
    char instead[2];
    memset(instead, '\0', 2);

    printf("%d, %d\n", instead[0], instead[1]);

    for(int i = 0; i < num * 64; i++)
    {
        printf("[%d], ", cipher1[i]);
    }
    printf("\n");

    int Count[256] = {0};
    for(int i = 0; i < num * 64; i++)
    {
        Count[cipher1[i] + 128]++;
    }

    for(int i = 0; i < 256; i++)
    {
        if(Count[i] == 0)
        {
            instead[0] = i - 128;
            printf("\ninstead: %d\n", instead[0]);
            break;
        }
    }

    for(int i = 0; i < num * 64; i++)
    {

        if(cipher1[i] == 0)
        {
            cnt++;
            cipher1[i] = instead[0];
            printf("%d, ", i);
        }
//         printf("{%d}, ", cipher4[i]);

    }
    printf("\n%d\n", cnt);

    qDebug()<<"[EncryptPubeid]: "<<cnt;
    qDebug()<<"[Encryptpubeid]: "<<strlen(cipher1);
    if (cnt != 0)
    {
        memcpy(cipher1 + num * 64, instead, 1);
    }

}

void MainWindow::connecteIDServer()
{
    eidSocket->connectToHost(QHostAddress::LocalHost, 14500);
}

void MainWindow::sendeIDServer(char code[])
{
    if(!strcmp(code, "01"))
    {
        QString rsaInfo1 = QString(QLatin1String(code));
        if(eidSocket->write(rsaInfo1.toLatin1(), rsaInfo1.length()) != rsaInfo1.length())
        {
            qDebug()<<"[sendeIDServer01]: "<<"error";
        }
        qDebug()<<"[sendeIDServer01]: "<<rsaInfo1.length();
    }
    if(!strcmp(code, "03"))
    {

        QString rsaInfo3 = QString(QLatin1String(cipher1));
        if(eidSocket->write(rsaInfo3.toLatin1(), rsaInfo3.length()) != rsaInfo3.length())
        {
            qDebug()<<"[sendeIDServer03]: "<<"error";
        }
        qDebug()<<"[sendeIDServer03]: "<<rsaInfo3.length();
    }

}
