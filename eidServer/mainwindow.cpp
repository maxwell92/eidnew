#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtNetwork/QHostAddress"
#include <QMessageBox>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/md5.h>
#include <QSqlDatabase>
#include "QtSql/QSqlQuery"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    eidServer = new QTcpServer(this);
    eidSocket = new QTcpSocket(this);
    spSocket = new QTcpSocket(this);

    connect(eidServer,SIGNAL(newConnection()),this,SLOT(newClient()));

    if(!eidServer->isListening())
    {
        if(!eidServer->listen(QHostAddress::Any,14500))
        {
            QMessageBox::about(this,"Attention!","eIDServer not listening!");
        }
    }

    connect(spSocket,SIGNAL(readyRead()),this,SLOT(recvSP()));
    getRsa();

    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("root");
    if(!db.open()){
        qDebug()<<"Unable to open database";
    }else{
        qDebug()<<"Database connection established";
    }

    someone = new people();

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

void MainWindow::getRsa()
{
    privateKey = RSA_generate_key(512, RSA_F4, NULL, NULL);

//    unsigned char PublicKey[512];
    unsigned char *PKey = PublicKey;
    int publicKeyLen = i2d_RSAPublicKey(privateKey, &PKey);

    qDebug()<<"[getRsa]: "<<publicKeyLen;

    PKey = PublicKey;
    publicKey = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, publicKeyLen);
}

void MainWindow::recvSP()
{
    char code[3];
    memset(code, '\0', 3);

    QString pubInfo;
    pubInfo = spSocket->readAll();
    memcpy(code, pubInfo.toLatin1().data(), 2);


    len_msg3 = 0;
    memset(msg3, '\0', 256);
    len_msg3 = pubInfo.length();
    memcpy(msg3, pubInfo.toLatin1().data(), len_msg3);

    if(!strcmp(code, "00"))
    {
        memcpy((char *)PkeyS, msg2 + 2, 74);
        unsigned char insd[2];
        memset(insd, '\0', 2);
        memcpy(insd, msg3 + 2 + 74, 1);

        for(int i = 0; i < 74; i++)
        {
            if(PkeyS[i] == insd[0])
            {
                PkeyS[i] = 0;
            }
                printf("%d, ", PkeyS[i]);
        }
        printf("\n");
        makePubS();
    }
}


void MainWindow::makePubS()
{
    unsigned char *PKey = PkeyS;
    int PublicKeyLen = 74;
    pubS = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, PublicKeyLen);
}

void MainWindow::newClient()
{
    eidSocket = eidServer->nextPendingConnection();
    connect(eidSocket, SIGNAL(readyRead()), this, SLOT(recvCli()));
    connect(eidSocket, SIGNAL(disconnected()), eidSocket, SLOT(deleteLater()));
}

void MainWindow::recvCli()
{
    QString cliInfo;
    cliInfo = eidSocket->readAll();

    len_cipher1 = 0;
    memset(cipher1, '\0', 256);
    memcpy(cipher1, cliInfo.toLatin1().data(), cliInfo.length());
    len_cipher1 = cliInfo.length();

    char code[3];
    memset(code, '\0', 3);
    memcpy(code, cipher1, 2);

    if(!strcmp(code, "01"))
    {
        rsaEnpack();
        sendRsa();
    }else{
        DecryptPrieid();
        Depack();
        if(Verify())
        {
            connectSP();
            getPubS();
            Enpack();
            EncryptPubS("03");

            sendSP();
        }
    }

}

void MainWindow::DecryptPrieid()
{
    len_msg2 = 0;
    memset(msg2, '\0', len_msg2);


    int i;
    int num2;
    num2 = len_cipher1 / 64;


    if(len_cipher1 % 64 != 0)
    {
        char instead[2];
        memset(instead, '\0', 2);
        memcpy(instead, cipher1 + (num2) * 64 , 1);

        printf("[DecryptPrieid]:instead: %d\n", instead[0]);
        int cnt = 0;
        for(int i = 0; i < (num2) * 64; i++)
        {
            if(cipher1[i] == instead[0])
            {
                cipher1[i] = 0;
                cnt++;
                printf("%d, ", i);
            }

        }
        printf("\n%d\n", cnt);

//        for(int i = 0; i < (num2 - 1) * 64; i++)
//        {
//            printf("[%d], ", cipher2[i]);
//        }
//        printf("\d");
    }


    len_msg2 = len_cipher1 - 1;

//    msg4 = (char *)malloc(len_msg4 * sizeof(char));
//    memset(msg4, '\0', len_msg4);
    qDebug()<<"[DecryptPrieid]: "<<len_msg2;


    unsigned char InBuff[64], OutBuff[64];
    for(i = 0; i < num2; i++) {
        memset((char *)InBuff, '\0', 64);
        memset((char *)OutBuff, '\0', 64);
//        strcpy((char *)OutBuff, cipher2 + 64 * i);
        memcpy((char *)OutBuff, cipher1 + 64 * i, 64);
        qDebug()<<"[DecryptPrieid]: OutBuff["<<i<<"]: "<<OutBuff<<endl;
        qDebug()<<"[DecryptPrieid]: OutBuff["<<i<<"]: "<<strlen((char *)OutBuff)<<endl;
        int plen = 0;
        plen = RSA_private_decrypt(64, (const unsigned char*)OutBuff, InBuff, privateKey, RSA_PKCS1_PADDING);
        qDebug()<<"[DecryptPrieid]: InBuff["<<i<<"]: "<<InBuff<<endl;
        qDebug()<<"[DecryptPrieid]: InBuff["<<i<<"]: "<<plen<<endl;

        memcpy(msg2 + i * 50, (char *)InBuff, 50);

//        if(i == (num2 - 1)) {
//            memcpy(msg4 + i * 50, (char *)InBuff, len_msg4 % 64);
//        }else {
//            memcpy(msg4 + i * 50, (char *)InBuff, 64);
//        }
            //strcat(msg2, (char *)InBuff);
    }
    qDebug()<<"[DecryptPrieid]: msg4: "<<msg2;
    qDebug()<<"[DecryptPrieid]: "<<strlen(msg2);
}

void MainWindow::Depack()
{
    char idlist[10];
    memset(idlist, '\0', 10);



    memcpy(idlist, msg2 + 2, 8);
    qDebug()<<"[Depack]: "<<idlist;
    memset(msgHash, '\0', 33);
    memcpy(msgHash, msg2 + 2 + 8, 32);
    qDebug()<<"[Depack]: "<<msgHash;
    memset(someone->eidNumber, '\0', 9);
    memcpy(someone->eidNumber, msg2 + 2 + 8 + 32, 8);
    qDebug()<<"[Depack]: "<<someone->eidNumber;
    memset(hn2, '\0', 33);
    memcpy(hn2, msg2 + 2 + 8 + 32 + 8, 32);
    qDebug()<<"[Depack]: "<<hn2;

    fillSomeone(someone->eidNumber);

    int len_userMsg = 0;
    memset(userMsg, '\0', 256);


        if(idlist[0] == '1')
        {
            memcpy(userMsg + len_userMsg, someone->teleNumber, strlen(someone->teleNumber));
            len_userMsg += strlen(someone->teleNumber);
        }
        if(idlist[1] == '1')
        {
            memcpy(userMsg + len_userMsg, someone->realName, strlen(someone->realName));
            len_userMsg += strlen(someone->realName);
        }
        if(idlist[2] == '1')
        {
            memcpy(userMsg + len_userMsg, someone->age, strlen(someone->age));
            len_userMsg += strlen(someone->age);
        }
        if(idlist[3] == '1')
        {
            memcpy(userMsg + len_userMsg, someone->company, strlen(someone->company));
            len_userMsg += strlen(someone->company);
        }
        if(idlist[4] == '1')
        {
            memcpy(userMsg + len_userMsg, someone->gender, strlen(someone->gender));
            len_userMsg += strlen(someone->gender);
        }
        if(idlist[5] == '1')
        {
            memcpy(userMsg + len_userMsg, someone->address, strlen(someone->address));
            len_userMsg += strlen(someone->address);
        }
        if(idlist[6] == '1')
        {
            memcpy(userMsg + len_userMsg, someone->city, strlen(someone->city));
            len_userMsg += strlen(someone->city);
        }
        if(idlist[7] == '1')
        {
            memcpy(userMsg + len_userMsg, someone->idNumber, strlen(someone->idNumber));
            len_userMsg += strlen(someone->idNumber);
        }


}

void MainWindow::fillSomeone(char eidNumber[])
{
    QSqlQuery query;
    char op[100];
    memset(op, '\0', 100);
    strcat(op, "select * from realInfo where eidNumber = \"");
    strcat(op, eidNumber);
    strcat(op, "\";");
    qDebug()<<"[fillSomeone]: "<<op;
    query.exec(op);
    if(query.next())
    {
        QString text1 = query.value(0).toString(); //eidNumber
        QString text2 = query.value(1).toString(); //teleNumber
        memcpy(someone->realName, text2.toLatin1().data(), text2.length());
        QString text3 = query.value(2).toString(); //realName
        memcpy(someone->teleNumber, text3.toLatin1().data(), text3.length());
        QString text4 = query.value(3).toString(); //age
        memcpy(someone->age, text4.toLatin1().data(), text4.length());
        QString text5 = query.value(4).toString(); //company
        memcpy(someone->company, text5.toLatin1().data(), text5.length());
        QString text6 = query.value(5).toString(); //gender
        memcpy(someone->gender, text6.toLatin1().data(), text6.length());
        QString text7 = query.value(6).toString(); //address
        memcpy(someone->address, text7.toLatin1().data(), text7.length());
        QString text8 = query.value(7).toString(); //city
        memcpy(someone->city, text8.toLatin1().data(), text8.length());
        QString text9 = query.value(8).toString(); //idNumber
        memcpy(someone->idNumber, text9.toLatin1().data(), text9.length());
        qDebug()<<"[fillSomeone]: "<<someone->realName;
        qDebug()<<"[fillSomeone]: "<<someone->teleNumber;
        qDebug()<<"[fillSomeone]: "<<someone->age;
        qDebug()<<"[fillSomeone]: "<<someone->company;
        qDebug()<<"[fillSomeone]: "<<someone->gender;
        qDebug()<<"[fillSomeone]: "<<someone->address;
        qDebug()<<"[fillSomeone]: "<<someone->city;
        qDebug()<<"[fillSomeone]: "<<someone->idNumber;
    }
}

int MainWindow::Verify()
{
    char userHash[33];
    memset(userHash, '\0', 33);

    hash(userMsg, userHash);
    if(!strcmp(userHash, msgHash))
    {
        return 1;
    }

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

void MainWindow::getPubS()
{
    char code[3];
    memset(code, '\0', 3);
    memcpy(code, "01", 2);

    QString spInfo1;
    spInfo1 = QString(QLatin1String(code));
    if(spSocket->write(spInfo1.toLatin1(), spInfo1.length()) != spInfo1.length())
    {
        qDebug()<<"[getPubS]: "<<"error";
    }
    qDebug()<<"[getPubS]: "<<spInfo1.length();

}

void MainWindow::Enpack()
{
    char code[3];
    memset(code, '\0', 3);
    memcpy(code, "03", 2);

    memset(ack, '\0', 2);
    ack[0] = 'y';

    len_msg4 = 2 + 1 + 32;
    memset(msg4, '\0', 256);
    memcpy(msg4, code, 2);
    memcpy(msg4 + 2, ack, 1);
    memcpy(msg4 + 2 + 1, hn2, 32);

    qDebug()<<"[Enpack]: "<<msg4;
    qDebug()<<"[Enpack]: "<<len_msg4;


}

void MainWindow::EncryptPubS(char code[])
{
    if(!strcmp(code, "03"))
    {
        int num;
        num = len_msg4 / 50 + 1;
        len_cipher2 = num * 64 + 1 + 1;
//        cipher2 = (char *)malloc(len_cipher2 * sizeof(char));
//        memset(cipher2, '\0', len_cipher2);

        memset(cipher2, '\0', 256);

        unsigned char InputBuff[65], OutputBuff[65];
        int i;
        for(i = 0; i < num; i++) {
            memset((char *)InputBuff, '\0', 65);
            memset((char *)OutputBuff, '\0', 65);

            if (i == (num - 1)){
                memcpy((char *)InputBuff, msg4 + 50 * i, len_msg4 % 50);
            } else{
                memcpy((char *)InputBuff, msg4 + 50 * i, 50);
            }

            int clen = 0;
            qDebug()<<"[EncryptpubS]: InputBuff["<<i<<"]: "<<InputBuff<<endl;
            qDebug()<<"[EncryptpubS]: InputBuff["<<i<<"]: "<<strlen((char *)InputBuff)<<endl;

            clen = RSA_public_encrypt(50,  (const unsigned char *)InputBuff, OutputBuff, pubS, RSA_PKCS1_PADDING);
            qDebug()<<"[EncryptpubS]: OutputBuff["<<i<<"]: "<<OutputBuff<<endl;
            qDebug()<<"[EncryptpubS]: OutputBuff["<<i<<"]: "<<clen<<endl;
            //strcat(cipher1, (char *)OutputBuff);
            memcpy(cipher2 + 64 * i, (char *)OutputBuff, 64);
            qDebug()<<"[EncryptpubS]: "<<strlen(cipher2);
        }
        qDebug()<<"[EncryptpubS]: "<<strlen(cipher2);

        int cnt = 0;
        char instead[2];
        memset(instead, '\0', 2);

        printf("%d, %d\n", instead[0], instead[1]);

        for(int i = 0; i < num * 64; i++)
        {
            printf("[%d], ", cipher2[i]);
        }
        printf("\n");

        int Count[256] = {0};
        for(int i = 0; i < num * 64; i++)
        {
            Count[cipher2[i] + 128]++;
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

            if(cipher2[i] == 0)
            {
                cnt++;
                cipher2[i] = instead[0];
                printf("%d, ", i);
            }
 //            printf("{%d}, ", cipher1[i]);
        }
        printf("\n%d\n", cnt);

        qDebug()<<"[EncryptPubS]: "<<cnt;
        qDebug()<<"[EncryptpubS]: "<<strlen(cipher2);
        if (cnt != 0)
        {
            memcpy(cipher2 + num * 64, instead, 1);
        }
    }
}

void MainWindow::connectSP()
{
    spSocket->connectToHost(QHostAddress::LocalHost, 15500);

}

void MainWindow::sendSP()
{
    QString ackInfo;
    ackInfo = QString(QLatin1String(cipher2));
    if(spSocket->write(ackInfo.toLatin1(), ackInfo.length()) != ackInfo.length())
    {
        qDebug()<<"[sendSP]: "<<"error";
    }
    qDebug()<<"[sendSP]: "<<ackInfo.length();

}

void MainWindow::rsaEnpack()
{
    int cnt = 0;
    unsigned char instead[2];
    memset(instead, '\0', 2);
    int Count[256] = {0};
    for(int i = 0; i < 74; i++)
    {
        Count[PublicKey[i]]++;
    }

    for(int i = 0; i < 256; i++)
    {
        if(Count[i] == 0)
        {
            instead[0] = i;
            break;
        }
    }

    for(int i = 0; i < 74; i++)
    {
        if(PublicKey[i] == 0)
        {
            cnt++;
            PublicKey[i] = instead[0];
        }
   //     printf("%d, ", PublicKey[i]);
    }

    for(int i = 0; i < 74; i++)
    {
        printf("%d, ", PublicKey[i]);
    }

    printf("\n");
    qDebug()<<"[rsaEnpack]: "<<strlen((char *)PublicKey);

    len_msg1 = 0;
    memset(msg1, '\0', 256);
    len_msg1 = 2 + 74 + 1 + 1;

    memcpy(msg1, "02", 2);
    memcpy(msg1 + 2, (char *)PublicKey, 74);
    memcpy(msg1 + 2 + 74, instead, 1);


    qDebug()<<"[rsaEnpack]: "<<msg1;
    qDebug()<<"[rsaEnpack]: "<<strlen(msg1);
}

void MainWindow::sendRsa()
{
    QString pkSInfo;
    pkSInfo = QString(QLatin1String(msg1));
    if(eidSocket->write(pkSInfo.toLatin1(), pkSInfo.length()) != pkSInfo.length())
    {
        qDebug()<<"[SendRsa]: "<<"error";
    }
    qDebug()<<"[SendRsa]: "<<pkSInfo.length();
}
