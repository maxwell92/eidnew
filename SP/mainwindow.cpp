#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QMessageBox>
#include <openssl/aes.h>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QDateTime>
#include <QSqlDatabase>
#include "QtSql/QSqlQuery"
#include "openssl/rsa.h"
#include "openssl/md5.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    regServer = new QTcpServer(this);
    connect(regServer,SIGNAL(newConnection()),this,SLOT(newReg()));

    if(!regServer->isListening())
    {
        if(!regServer->listen(QHostAddress::Any,11500))
        {
            QMessageBox::about(this,"Attention!","regServer not listening!");
        }
    }

    regger = new person();

    logServer = new QTcpServer(this);
    connect(logServer, SIGNAL(newConnection()), this, SLOT(newLog()));

    if(!logServer->isListening())
    {
        if(!logServer->listen(QHostAddress::Any,12500))
        {
            QMessageBox::about(this,"Attention!","logServer not listening!");
        }
    }

    logger = new person();
    rsaKey();
}

void MainWindow::rsaKey()
{
    privateKey = RSA_generate_key(512, RSA_F4, NULL, NULL);

//    unsigned char PublicKey[512];
    unsigned char *PKey = PublicKey;
    int publicKeyLen = i2d_RSAPublicKey(privateKey, &PKey);

    qDebug()<<"[rsaKey]: "<<publicKeyLen;

    PKey = PublicKey;
    publicKey = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, publicKeyLen);
}

void MainWindow::newReg()
{
    regSocket = regServer->nextPendingConnection();
    connect(regSocket,SIGNAL(readyRead()),this,SLOT(recvReg()));
    connect(regSocket,SIGNAL(disconnected()),regSocket,SLOT(deleteLater()));
}

void MainWindow::newLog()
{
    logSocket = logServer->nextPendingConnection();
    connect(logSocket, SIGNAL(readyRead()), this, SLOT(recvLog()));
    connect(logSocket, SIGNAL(disconnected()), logSocket, SLOT(deleteLater()));
}

void MainWindow::recvReg()
{
    QString regInfo1 = regSocket->readAll();
    QByteArray b;
    b = regInfo1.toLatin1();
    len_cipher1 = regInfo1.length();
    qDebug()<<"[recvReg]: "<<len_cipher1;


    cipher1 = (char *)malloc(len_cipher1 * sizeof(char));
    memset(cipher1, '\0', len_cipher1);

    strcpy(cipher1, b.data());
    qDebug()<<"[recvReg]: "<<cipher1;

    DecryptK0();
    DepackReg();
}

void MainWindow::DecryptK0()
{
    unsigned char InBuff[16], OutBuff[16];

    char *plain;
    int len_plain;
    len_plain = (len_cipher1 / 16 + 1) * 16;
    plain = (char *)malloc(len_plain * sizeof(char));
    memset(plain, '\0', len_plain);

    unsigned char Seed[32];
    strcpy((char *)Seed, "AbCdEfGhIjKlMnOpQrStUvWxYz12345");

    AES_KEY AESEncryptKey, AESDecryptKey;
    AES_set_encrypt_key(Seed, 256, &AESEncryptKey);
    AES_set_decrypt_key(Seed, 256, &AESDecryptKey);

    int i;
    int num1;
    num1 = len_cipher1 / 16 + 1;

    for (i = 0; i < num1; i++) {
             memset((char *)InBuff, '\0', 16);
             memset((char *)OutBuff, '\0', 16);

             memcpy((char *)OutBuff, cipher1 + 16 * i, 16);

             AES_ecb_encrypt(OutBuff, InBuff, &AESDecryptKey, AES_DECRYPT);
             if(i == (num1 - 1)) {
                     memcpy(plain + 16 * i, (char *)InBuff, len_plain % 16);
             }else {
                     memcpy(plain + 16 * i, (char *)InBuff, 16);
             }
    }

    len_msg1 = strlen(plain);
    msg1 = (char *)malloc(len_msg1 * sizeof(char));
    memset(msg1, '\0', len_msg1);
    memcpy(msg1, plain, len_plain);
    qDebug()<<"[DecryptK0]: "<<msg1;
}

void MainWindow::DepackReg()
{
    char code[3];
    memset(code, '\0', 3);
    memcpy(code, msg1, 2);
    qDebug()<<"[DepackReg]: "<<code;
    int len_name;

    if(!strcmp(code, "01"))
    {
        len_name = len_msg1 - 2 - 32 - 8;
        qDebug()<<"[DepackReg]: "<<len_name;
        memcpy(regger->username, msg1 + 2, len_name);
        memcpy(regger->eidNum, msg1 + 2 + len_name, 8);
        memcpy(regger->m, msg1 + 2 + len_name + 8, 32);

        qDebug()<<"[DepackReg]: "<<regger->username;
        qDebug()<<"[DepackReg]: "<<regger->eidNum;
        qDebug()<<"[DepackReg]: "<<regger->m;

        QDate date;
        QTime time;
        QDateTime dt;

        dt.setDate(date.currentDate());
        dt.setTime(time.currentTime());
        QString current = dt.toString("yyyy:MM:dd:hh:mm:ss");

        memcpy(regger->ti, current.toLatin1().data(), current.length());
        qDebug()<<"[DepackReg]: "<<regger->ti;

        Enpack("02");
        EncryptK0();
        ResponseCli();
    }

    if(!strcmp(code, "03"))
    {
        qDebug()<<"[DepackReg03]: "<<msg1;
        memcpy(regger->y0, msg1 + 2, 32);
        qDebug()<<"[DepackReg03]: "<<regger->y0;

        char ack[2];
        memset(ack, '\0', 2);
        memcpy(ack, msg1 + 2 + 32, 1);
        qDebug()<<"[DepackReg03]: "<<ack;
        if(!strcmp(ack,"y"))
        {
            qDebug()<<"[DepackReg03]: "<<"Ack yes";
            if(!existDB(regger->username))
            {
                saveDB();
                sendSucc();
            }else{
                sendErr();
            }

        }
    }
}

int MainWindow::existDB(char name[])
{
    QSqlQuery query;
    char op[100];
    memset(op, '\0', 100);
    strcat(op, "select * from person where username = \"");
    strcat(op, name);
    strcat(op, "\";");
    qDebug()<<"[existDB]: "<<op;
    query.exec(op);
    if(query.next())
    {
        qDebug()<<"[existDB]: "<<"exist";
        return 1;
    } else {
        qDebug()<<"[existDB]: "<<"not exist";
        return 0;
    }

}

void MainWindow::saveDB()
{
    QSqlQuery query;
    char op[255];
    memset(op, '\0', 100);
    strcat(op, "insert into person values(\"");
    strcat(op, regger->username);
    strcat(op, "\",\"");
    strcat(op, regger->eidNum);
    strcat(op, "\",\"");
    strcat(op, regger->m);
    strcat(op, "\",\"");
    strcat(op, regger->ti);
    strcat(op, "\",\"");
    strcat(op, regger->y0);
    strcat(op, "\");");
    qDebug()<<"[saveDB]: "<<op;
    query.exec(op);

}

void MainWindow::sendSucc()
{
    char succ[] = "succ";
    QString regSucc;
    regSucc = QString(QLatin1String(succ));
    if(regSocket->write(regSucc.toLatin1(), regSucc.length()) != regSucc.length())
    {
            qDebug()<<"[sendSucc]: "<<"error";
    }
    qDebug()<<"[sendSucc]: "<<regSucc.length();
    regSocket->close();

}

void MainWindow::sendErr()
{
    char err[] = "existed";
    QString regErr;
    regErr = QString(QLatin1String(err));
    if(regSocket->write(regErr.toLatin1(), regErr.length()) != regErr.length())
    {
            qDebug()<<"[sendErr]: "<<"error";
    }
    qDebug()<<"[sendErr]: "<<regErr.length();

}

void MainWindow::ResponseCli()
{
    QString VerInfo;
    VerInfo = QString(QLatin1String(cipher2));
    if(regSocket->write(VerInfo.toLatin1(), VerInfo.length()) != VerInfo.length())
    {
        qDebug()<<"[ResponseCli]: "<<"error";
    }
    qDebug()<<"[ResponseCli]: "<<VerInfo.length();
}

void MainWindow::EncryptK0()
{
    unsigned char InBuff[16], OutBuff[16];

    char *plain;
    int len_plain;
    len_plain = strlen(msg2);
    plain = (char *)malloc(len_plain * sizeof(char));
    memset(plain, '\0', len_plain);

    memcpy(plain, msg2, strlen(msg2));


    len_cipher2 = (len_plain / 16 + 1) * 16;
    cipher2 = (char *)malloc(len_cipher2 * sizeof(char));
    memset(cipher2, '\0', len_cipher2);

    unsigned char Seed[32];
    strcpy((char *)Seed, "AbCdEfGhIjKlMnOpQrStUvWxYz12345");

    AES_KEY AESEncryptKey, AESDecryptKey;
    AES_set_encrypt_key(Seed, 256, &AESEncryptKey);
    AES_set_decrypt_key(Seed, 256, &AESDecryptKey);


    int i;
    int num1;
    num1 = len_plain / 16 + 1;

    for(i = 0; i < num1; i++) {
        memset((char *)InBuff, '\0', 16);
        memset((char *)OutBuff, '\0', 16);

        if (i == (num1 - 1)) {
            memcpy((char *)InBuff, plain + 16 * i, len_plain % 16);
        } else {
            memcpy((char *)InBuff, plain + 16 * i, 16);
        }

        AES_ecb_encrypt(InBuff, OutBuff, &AESEncryptKey, AES_ENCRYPT);
        memcpy(cipher2 + 16 * i, (char *)OutBuff, 16);
    }
    qDebug()<<"[EncryptK0]: "<<strlen(cipher2);
}

void MainWindow::Enpack(char code[])
{
    if(!strcmp(code, "02"))
    {
        len_msg2 = 2 + strlen(regger->username) + strlen(regger->eidNum) + strlen(regger->m) + strlen(regger->ti);
        msg2 = (char *)malloc(len_msg2 * sizeof(char));
        memset(msg2, '\0', len_msg2);

        strcat(msg2, code);
        strcat(msg2, regger->username);
        strcat(msg2, regger->eidNum);
        strcat(msg2, regger->m);
        strcat(msg2, regger->ti);

        qDebug()<<"[Enpack]: "<<msg2;
        qDebug()<<"[Enpack]: "<<len_msg2;
    }
}

void MainWindow::recvLog()
{
    QString logInfo1 = logSocket->readAll();
    QByteArray b;
    b = logInfo1.toLatin1();
//    qDebug()<<"[recvLog]: "<<logInfo1.length();

    len_msg4 = 0;
    memset(msg4, '\0', 256);
    len_msg4 = logInfo1.length();
    memcpy(msg4, b.data(), len_msg4);
//    qDebug()<<"[recvLog]: "<<msg4;
    qDebug()<<"[recvLog]: "<<len_msg4;

    char code[3];
    memset(code, '\0', 3);
    memcpy(code, msg4, 2);

    if(!strcmp(code, "00"))
    {
        int len_name = logInfo1.length() - 2 - 74 - 1;

       // memset((char *)PKeyC, '\0', 512);
        memcpy((char *)PKeyC, msg4 + 2, 74);
        unsigned char instead[2];
        memset(instead, '\0', 2);
        memcpy(instead, msg4 + 2 + 74, 1);


        for(int i = 0; i < 74; i++)
        {
            if(PKeyC[i] == instead[0])
            {
                PKeyC[i] = 0;
            }
            printf("%d, ", PKeyC[i]);
        }
        printf("\n");

        makePKeyC();
        SendPubS();

    }else{
        DecryptPriS();
        Depacklog();
        fillLogger();
        makeIdList();
        getM1();
        hashn0();
        logEnpack("02");
        EncryptPubC();
        sendIdlist();
    }
}

void MainWindow::fillLogger()
{
    QSqlQuery query;
    char op[256];
    memset(op, '\0', 256);
    strcat(op, "select * from person where username = \"");
    strcat(op, logger->username);
    strcat(op, "\";");
    qDebug()<<"[fillLogger]: "<<op;
    query.exec(op);
    if(query.next())
    {
        QString text1 = query.value(0).toString(); //username
        QString text2 = query.value(1).toString(); //eidnum
        memcpy(logger->eidNum, text2.toLatin1().data(), 8);
        QString text3 = query.value(2).toString(); //m
        memcpy(logger->m, text3.toLatin1().data(), 32);
        QString text4 = query.value(3).toString(); //ti
        memcpy(logger->ti, text4.toLatin1().data(), 19);
        QString text5 = query.value(4).toString(); //y0
        memcpy(logger->y0, text5.toLatin1().data(), 32);

        qDebug()<<"[fillLogger]: "<<logger->eidNum;
        qDebug()<<"[fillLogger]: "<<logger->m;
        qDebug()<<"[fillLogger]: "<<logger->ti;
        qDebug()<<"[fillLogger]: "<<logger->y0;
    }
}

void MainWindow::makeIdList()
{
//id info set[8]: address, gender, postcode, job, age, tele, company, insurancenum;
    memset(idlist, '\0', 5);
    idlist[0] = '2';
    idlist[1] = '4';
    idlist[2] = '5';
    idlist[3] = '7';

}

void MainWindow::getM1()
{
    char buf[33] = {'\0'};
    char tmp[3] = {'\0'};
    unsigned char md[16];

    QDate date;
    QTime time;
    QDateTime dt;

    dt.setDate(date.currentDate());
    dt.setTime(time.currentTime());
    QString current = dt.toString("yyyy:MM:dd:hh:mm:ss");
    char t[20];
    memset(t, '\0', 20);
    //t = current.toLatin1().data();
    memcpy(t, current.toLatin1().data(), current.length());
    memcpy(logger->tm, current.toLatin1().data(), current.length());

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, t, strlen(t));
    MD5_Final(md, &ctx);

    for(int i = 0; i < 16; i++ )
    {
        sprintf(tmp, "%2.2x", md[i]);
        strcat(buf, tmp);
    }


    strcpy(logger->m1, buf);
}

void MainWindow::hashn0()
{
    memset(n1, '\0', 2);
    n1[0] = n0[0] + 1;
    memset(hn1, '\0', 32);
    hash(n1, hn1);

}

void MainWindow::logEnpack(char code[])
{
    if(!strcmp(code, "02"))
    {
        //len_msg7 = 2 + 8 + 32 + 4 + strlen(logger->username) + 19 + 32 + 1;
        len_msg7 = 2 + 8 + 32 + 4 + strlen(logger->username) + 19 + 19 + 1;
        msg7 = (char *)malloc(len_msg7 * sizeof(char));
        memset(msg7, '\0', len_msg7);

        memcpy(msg7, code, 2);
        memcpy(msg7 + 2, logger->eidNum, 8);
        memcpy(msg7 + 2 + 8, hn1, 32);
        memcpy(msg7 + 2 + 8 + 32, idlist, 4);
        memcpy(msg7 + 2 + 8 + 32 + 4, logger->username, strlen(logger->username));
        memcpy(msg7 + 2 + 8 + 32 + 4 + strlen(logger->username), logger->ti, 19);
        memcpy(msg7 + 2 + 8 + 32 + 4 + strlen(logger->username) + 19, logger->tm, 19);
        //memcpy(msg7 + 2 + 8 + 32 + 4 + strlen(logger->username) + 19, logger->m1, 32);

        qDebug()<<"[logEnpack02]: "<<msg7;
        qDebug()<<"[logEnpack02]: "<<strlen(msg7);
    }
}

void MainWindow::EncryptPubC()
{
    int num;
    num = len_msg7 / 50 + 1;
    len_cipher4 = num * 64 + 1 + 1;
    cipher4 = (char *)malloc(len_cipher4 * sizeof(char));
    memset(cipher4, '\0', len_cipher4);

    unsigned char InputBuff[65], OutputBuff[65];
    int i;
    for(i = 0; i < num; i++) {
        memset((char *)InputBuff, '\0', 65);
        memset((char *)OutputBuff, '\0', 65);

        if (i == (num - 1)){
            memcpy((char *)InputBuff, msg7 + 50 * i, len_msg7 % 50);
        } else{
            memcpy((char *)InputBuff, msg7 + 50 * i, 50);
        }

        int clen = 0;
        qDebug()<<"[EncryptpubC]: InputBuff["<<i<<"]: "<<InputBuff<<endl;
        qDebug()<<"[EncryptpubC]: InputBuff["<<i<<"]: "<<strlen((char *)InputBuff)<<endl;

        clen = RSA_public_encrypt(50,  (const unsigned char *)InputBuff, OutputBuff, pubC, RSA_PKCS1_PADDING);
        qDebug()<<"[EncryptpubC]: OutputBuff["<<i<<"]: "<<OutputBuff<<endl;
        qDebug()<<"[EncryptpubC]: OutputBuff["<<i<<"]: "<<clen<<endl;
        //strcat(cipher1, (char *)OutputBuff);
        memcpy(cipher4 + 64 * i, (char *)OutputBuff, 64);
        qDebug()<<"[EncryptPubC]: "<<strlen(cipher4);
    }

    int cnt = 0;
    char instead[2];
    memset(instead, '\0', 2);

    printf("%d, %d\n", instead[0], instead[1]);

    for(int i = 0; i < num * 64; i++)
    {
        printf("[%d], ", cipher4[i]);
    }
    printf("\n");

    int Count[256] = {0};
    for(int i = 0; i < num * 64; i++)
    {
        Count[cipher4[i] + 128]++;
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

        if(cipher4[i] == 0)
        {
            cnt++;
            cipher4[i] = instead[0];
            printf("%d, ", i);
        }
//         printf("{%d}, ", cipher4[i]);

    }
    printf("\n%d\n", cnt);

    qDebug()<<"[EncryptPubC]: "<<cnt;
    qDebug()<<"[EncryptpubC]: "<<strlen(cipher4);
    if (cnt != 0)
    {
        memcpy(cipher4 + num * 64, instead, 1);
    }


}

void MainWindow::sendIdlist()
{
    QString idlistInfo;
    idlistInfo = QString(QLatin1String(cipher4));

    if(logSocket->write(idlistInfo.toLatin1(), idlistInfo.length()) != idlistInfo.length())
    {
        qDebug()<<"[sendIDlist]: "<<"error";
    }
    qDebug()<<"[sendIDlist]: "<<idlistInfo.length();
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

void MainWindow::DecryptPriS()
{
    len_cipher3 = 0;
    memset(cipher3, '\0', len_cipher3);
    len_cipher3 = len_msg4;
    memcpy(cipher3, msg4, len_cipher3);


    int i;
    int num2;
    num2 = len_cipher3 / 64;
    qDebug()<<"[DecryptPriS]: "<<len_cipher3;

    if(len_cipher3 % 64 != 0)
    {
        char instead[2];
        memset(instead, '\0', 2);
        memcpy(instead, cipher3 + (num2) * 64 , 1);

        printf("[DecryptPriS]:instead: %d\n", instead[0]);
        int cnt = 0;
        for(int i = 0; i < (num2) * 64; i++)
        {
            if(cipher3[i] == instead[0])
            {
                cipher3[i] = 0;
                printf("%d, ", i);
            }
        }
        printf("\n%d\n", cnt);

//        for(int i = 0; i < (num2 - 1) * 64; i++)
//        {
//            printf("[%d], ", cipher3[i]);
//        }
//        printf("\d");

    }

    len_msg6 = 0;
    memset(msg6, '\0', 256);
    len_msg6 = len_cipher3 - 1;



    //num2 = len_cipher2 / 64;
    unsigned char InBuff[64], OutBuff[64];
    for(i = 0; i < num2; i++) {
        memset((char *)InBuff, '\0', 64);
        memset((char *)OutBuff, '\0', 64);
//        strcpy((char *)OutBuff, cipher3 + 64 * i);
        memcpy((char *)OutBuff, cipher3 + 64 * i, 64);
        qDebug()<<"[DecryptPris]: OutBuff["<<i<<"]: "<<OutBuff<<endl;
        qDebug()<<"[DecryptPris]: OutBuff["<<i<<"]: "<<strlen((char *)OutBuff)<<endl;
        int plen = 0;
        plen = RSA_private_decrypt(64, (const unsigned char*)OutBuff, InBuff, privateKey, RSA_PKCS1_PADDING);
        qDebug()<<"[DecryptPris]: InBuff["<<i<<"]: "<<InBuff<<endl;
        qDebug()<<"[DecryptPris]: InBuff["<<i<<"]: "<<plen<<endl;

        memcpy(msg6 + i * 50, (char *)InBuff, 50);
//        if(i == (num2 - 1)) {
//            memcpy(msg6 + i * 50, (char *)InBuff, len_msg6 % 64);
//        }else {
//            memcpy(msg6 + i * 50, (char *)InBuff, 64);
//        }
            //strcat(msg2, (char *)InBuff);
    }
    qDebug()<<"[DecryptPris]: msg6: "<<msg6<<endl;
    for(int i = 0; i < len_msg6; i++)
    {
        printf("msg6[%d]%d, ", i, msg6[i]);
    }
    printf("\n");

}

void MainWindow::Depacklog()
{
    char code[3];
    memset(code, '\0', 3);
    memcpy(code, msg6, 2);

    if(!strcmp(code, "01"))
    {
        memset(n0, '\0', 2);

        int len_name;
        len_name = strlen(msg6) - 2 - 1;
        memcpy(logger->username, msg6 + 2, len_name);
        qDebug()<<"[Depacklog01]: "<<logger->username;

        memcpy(n0, msg6 + 2 + len_name, 1);
        qDebug()<<"[Depacklog01]: "<<n0;

        if(!VerifyUsername())
        {
            this->SendNoExist();
        }

    }

}
void MainWindow::makePKeyC()
{
    unsigned char *PKey = PKeyC;
    int PublicKeyLen = 74;
    pubC = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, PublicKeyLen);

}

int MainWindow::VerifyUsername()
{
    QSqlQuery query;
    char op[256];
    memset(op, '\0', 256);
    strcat(op, "select * from person where username = \"");
    strcat(op, logger->username);
    strcat(op, "\";");
    qDebug()<<"[VerifyUsername]: "<<op;
    query.exec(op);
    if(query.next())
    {
        qDebug()<<"[VerifyUsername]: "<<"exist";
        return 1;
    } else {
        qDebug()<<"[VerifyUsername]: "<<"not exist";
        return 0;
    }
}

void MainWindow::SendPubS()
{
    rsaEnpack("00");
    SendClient();
}

void MainWindow::rsaEnpack(char code[])
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

    int len_msg5 = 2 + 74 + 1 + 1;
    msg5 = (char *)malloc(len_msg5 * sizeof(char));
    memset(msg5, '\0', len_msg5);

    memcpy(msg5, code, 2);
    memcpy(msg5 + 2, (char *)PublicKey, 74);
    memcpy(msg5 + 2 + 74, instead, 1);


    qDebug()<<"[rsaEnpack]: "<<msg5;
    qDebug()<<"[rsaEnpack]: "<<strlen(msg5);

}

void MainWindow::SendClient()
{
    QString pkSInfo;
    pkSInfo = QString(QLatin1String(msg5));
    if(logSocket->write(pkSInfo.toLatin1(), pkSInfo.length()) != pkSInfo.length())
    {
        qDebug()<<"[SendClient]: "<<"error";
    }
    qDebug()<<"[SendClient]: "<<pkSInfo.length();

}


void MainWindow::SendNoExist()
{
    char noExist[]="99none";
    QString None;
    None = QString(QLatin1String(noExist));
    if(logSocket->write(None.toLatin1(), None.length()) != None.length())
    {
        qDebug()<<"[SendNoExist]: "<<"error";
    }
    qDebug()<<"[SendNoExist]: "<<None.length();

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
