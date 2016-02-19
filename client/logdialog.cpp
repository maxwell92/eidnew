#include "logdialog.h"
#include "ui_logdialog.h"
#include "openssl/rsa.h"
#include "QByteArray"
#include "string.h"
#include "QDebug"
#include "QHostAddress"
#include "QMessageBox"
#include <QDateTime>


logDialog::logDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logDialog)
{
    ui->setupUi(this);


    logger = new user();
    logSocket = new QTcpSocket(this);
    connect(logSocket, SIGNAL(readyRead()), this, SLOT(recvSP()));



}

logDialog::~logDialog()
{
    delete ui;
}

void logDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void logDialog::on_LoginBtn_clicked()
{
//    this->hide();
//    emit(showMain());
    QString usernameInput = ui->usernameLinedit->text();
    QString skeyInput = ui->passwdLinedit->text();

    if(usernameInput.isEmpty() || skeyInput.isEmpty())
    {
        QMessageBox::about(this, "Attention", "Can't be Empty!");
    }

    CreateLogger(usernameInput, skeyInput);
    rsaKey();
    ExchangeRSAKey();
}

void logDialog::CreateLogger(QString username, QString skey)
{
//    QByteArray b;
    strcpy(logger->username, username.toLatin1().data());
    strcpy(logger->skey, skey.toLatin1().data());
    qDebug()<<"[CreateLogger]: "<<logger->username;
    qDebug()<<"[CreateLogger]: "<<logger->skey;

}

void logDialog::rsaKey()
{
    privateKey = RSA_generate_key(512, RSA_F4, NULL, NULL);


//    unsigned char PublicKey[512];
    unsigned char *PKey = PublicKey;


    int publicKeyLen = i2d_RSAPublicKey(privateKey, &PKey);

    qDebug()<<"[rsaKey]: "<<publicKeyLen;

    for(int i = 0; i < publicKeyLen; i++)
    {
        printf("%d, ", PublicKey[i]);
    }
    printf("\n");

    PKey = PublicKey;
    publicKey = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, publicKeyLen);

    qDebug()<<"[rsaKey]: "<<sizeof(publicKey);
    qDebug()<<"[rsaKey]: "<<sizeof(privateKey);

}

void logDialog::ExchangeRSAKey()
{
    Enpack("00");
    connectSP();
    sendRSA();
}

void logDialog::Enpack(char code[])
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
//        printf("%d, ", PublicKey[i]);
    }
    printf("\n");
    //qDebug()<<"[Enpack]: "<<strlen((char *)PublicKey);

    int len_msg1 = 2 + 74 + 1 + 1;
    msg1 = (char *)malloc(len_msg1 * sizeof(char));
    memset(msg1, '\0', len_msg1);

    memcpy(msg1, code, 2);
    memcpy(msg1 + 2, (char *)PublicKey, 74);
    memcpy(msg1 + 2 + 74, instead, 1);


    qDebug()<<"[Enpack]: "<<msg1;
    qDebug()<<"[Enpack]: "<<strlen(msg1);
}

void logDialog::connectSP()
{
    logSocket->connectToHost(QHostAddress::LocalHost, 12500);
}

void logDialog::sendRSA()
{
    QString rsaInfo;
    rsaInfo = QString(QLatin1String(msg1));
    if(logSocket->write(rsaInfo.toLatin1(), rsaInfo.length()) != rsaInfo.length())
    {
        qDebug()<<"[sendRSA]: "<<"error";
    }
    qDebug()<<"[sendRSA]: "<<rsaInfo.length();
}

void logDialog::recvSP()
{
    QString rsaInfo1 = logSocket->readAll();
    QByteArray b;
    b = rsaInfo1.toLatin1();

    qDebug()<<"[recvSP]: "<<rsaInfo1.length();

    len_msg2 = rsaInfo1.length() + 1;
    msg2 = (char *)malloc(len_msg2 * sizeof(char));
    memset(msg2, '\0', len_msg2);

    strcpy(msg2, b.data());
    qDebug()<<"[recvSP]: "<<msg2;

    char code[3];
    memset(code, '\0', 3);
    memcpy(code, msg2, 2);

    if(!strcmp(code, "00"))
    {
        memcpy((char *)PkeyS, msg2 + 2, 74);
        unsigned char insd[2];
        memset(insd, '\0', 2);
        memcpy(insd, msg2 + 2 + 74, 1);

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
        getN0();
        Enpacklog("01");
        EncryptpubS("01");
        sendLog("01");

    } else if(!strcmp(code, "99"))
    {
        char noExist[5];
        memset(noExist, '\0', 5);
        memcpy(noExist, msg2 + 2, 4);
        if(!strcmp(noExist, "none"))
        {
            QMessageBox::about(this, "Attention", "Username Not Exists!");
        }
    } else {
        DecryptPriC();
        Depack();
        fillLogger();
        dealIdlist();
    }
}

void logDialog::DecryptPriC()
{
    len_cipher2 = len_msg2 - 1;
    cipher2 = (char *)malloc(len_cipher2 * sizeof(char));
    memset(cipher2, '\0', len_cipher2);
    memcpy(cipher2, msg2, len_msg2 - 1);

    int i;
    int num2;
    num2 = len_cipher2 / 64 + 1;
    qDebug()<<"[DecryptPriC]: "<<len_cipher2;

    if(len_cipher2 % 64 != 0)
    {
        char instead[2];
        memset(instead, '\0', 2);
        memcpy(instead, cipher2 + (num2 - 1) * 64 , 1);

        printf("[DecryptPriC]:instead: %d\n", instead[0]);

        for(int i = 0; i < (num2 - 1) * 64; i++)
        {
            if(cipher2[i] == instead[0])
            {
                cipher2[i] = 0;
            }
        }

        for(int i = 0; i < (num2 - 1) * 64; i++)
        {
            printf("[%d], ", cipher2[i]);
        }
        printf("\d");
    }

    len_msg4 = len_cipher2 + 1;
    msg4 = (char *)malloc(len_msg4 * sizeof(char));
    memset(msg4, '\0', len_msg4);
    qDebug()<<"[DecryptPriC]: "<<len_msg4;


    //num2 = len_cipher2 / 64;
    unsigned char InBuff[64], OutBuff[64];
    for(i = 0; i < num2; i++) {
        memset((char *)InBuff, '\0', 64);
        memset((char *)OutBuff, '\0', 64);
        strcpy((char *)OutBuff, cipher2 + 64 * i);
        qDebug()<<"[DecryptPriC]: OutBuff["<<i<<"]: "<<OutBuff<<endl;
        qDebug()<<"[DecryptPriC]: OutBuff["<<i<<"]: "<<strlen((char *)OutBuff)<<endl;
        int plen = 0;
        plen = RSA_private_decrypt(64, (const unsigned char*)OutBuff, InBuff, privateKey, RSA_PKCS1_PADDING);
        qDebug()<<"[DecryptPriC]: InBuff["<<i<<"]: "<<InBuff<<endl;
        qDebug()<<"[DecryptPriC]: InBuff["<<i<<"]: "<<plen<<endl;

        if(i == (num2 - 1)) {
            memcpy(msg4 + i * 50, (char *)InBuff, len_msg4 % 64);
        }else {
            memcpy(msg4 + i * 50, (char *)InBuff, 64);
        }
            //strcat(msg2, (char *)InBuff);
    }
    qDebug()<<"[DecryptPriC]: msg4: "<<msg4;
    qDebug()<<"[DecryptPriC]: "<<strlen(msg4);

    for(int i = 0; i < len_msg4; i++)
    {
        printf("msg4[%d]%d, ", i, msg4[i]);
    }
    printf("\n");
}

void logDialog::Depack()
{
//    char code[3];
//    memset(code, '\0', 3);
//    qDebug()<<"[Depack]";
//    memcpy(code, msg4, 2);
//    qDebug()<<"[Depack]: "<<code;
    char code[]="02";
    if(!strcmp(code, "02"))
    {
        char uname[21];
        memset(uname, '\0', 21);
        int len_uname;
        char hn1tmp[33];
        memset(hn1tmp, '\0', 33);


        memcpy(logger->eidNum, msg4 + 2 ,8);
        qDebug()<<"[Depack02]: "<<logger->eidNum;

        memset(hn1, '\0', 33);
        memcpy(hn1, msg4 + 2 + 8, 32);
        qDebug()<<"[Depack02]: "<<hn1;

        memset(idlist, '\0', 5);
        memcpy(idlist, msg4 + 2 + 8 + 32, 4);
        qDebug()<<"[Depack02]: "<<idlist;

        len_uname = strlen(msg4) - 2 - 8 - 32 - 4 - 19 - 32;
        memcpy(uname, msg4 + 2 + 8 + 32, len_uname);
        qDebug()<<"[Depack02]: "<<uname;

        memcpy(logger->ti, msg4 + 2 + 8 + 32 + 4 + len_uname, 19);
        qDebug()<<"[Depack02]: "<<logger->ti;

        memcpy(logger->m1 ,msg4 + 2 + 8 + 32 + 4 + len_uname + 19, 32);
        qDebug()<<"[Depack02]: "<<logger->m1;







    }
}

void logDialog::fillLogger()
{


}

void logDialog::dealIdlist()
{

}

void logDialog::Enpacklog(char code[])
{
    if(!strcmp(code, "01"))
    {
        len_msg3 = 2 + strlen(logger->username) + 1 + 1;
        msg3 = (char *)malloc(len_msg3 * sizeof(char));
        memset(msg3, '\0', len_msg3);

        memcpy(msg3, code, 2);
        memcpy(msg3 + 2, logger->username, strlen(logger->username));
        memcpy(msg3 + 2 + strlen(logger->username), n0, 1);
        qDebug()<<"[Enpacklog01]: "<<msg3;
        qDebug()<<"[Enpacklog01]: "<<strlen(msg3);
    }
}

void logDialog::EncryptpubS(char code[])
{
    if(!strcmp(code, "01"))
    {
        int num;
        num = len_msg3 / 50 + 1;
        len_cipher1 = num * 64 + 1 + 1;
        cipher1 = (char *)malloc(len_cipher1 * sizeof(char));
        memset(cipher1, '\0', len_cipher1);

        unsigned char InputBuff[65], OutputBuff[65];
        int i;
        for(i = 0; i < num; i++) {
            memset((char *)InputBuff, '\0', 65);
            memset((char *)OutputBuff, '\0', 65);

            if (i == (num - 1)){
                memcpy((char *)InputBuff, msg3 + 50 * i, len_msg3 % 50);
            } else{
                memcpy((char *)InputBuff, msg3 + 50 * i, 50);
            }

            int clen = 0;
            qDebug()<<"[EncryptpubS]: InputBuff["<<i<<"]: "<<InputBuff<<endl;
            qDebug()<<"[EncryptpubS]: InputBuff["<<i<<"]: "<<strlen((char *)InputBuff)<<endl;

            clen = RSA_public_encrypt(50,  (const unsigned char *)InputBuff, OutputBuff, pubS, RSA_PKCS1_PADDING);
            qDebug()<<"[EncryptpubS]: OutputBuff["<<i<<"]: "<<OutputBuff<<endl;
            qDebug()<<"[EncryptpubS]: OutputBuff["<<i<<"]: "<<clen<<endl;
            //strcat(cipher1, (char *)OutputBuff);
            memcpy(cipher1 + 64 * i, (char *)OutputBuff, 64);
            qDebug()<<"[EncryptpubS]: "<<strlen(cipher1);
        }
        qDebug()<<"[EncryptpubS]: "<<strlen(cipher1);

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
            }
             printf("{%d}, ", cipher1[i]);
        }
        printf("\n");

        qDebug()<<"[EncryptPubS]: "<<cnt;
        qDebug()<<"[EncryptpubS]: "<<strlen(cipher1);
        if (cnt != 0)
        {
            memcpy(cipher1 + num * 64, instead, 1);
        }




    }
    if(!strcmp(code, "03"))
    {

    }


}

void logDialog::sendLog(char code[])
{
    QString logInfo1;
    if(!strcmp(code, "01"))
    {
        logInfo1 = QString(QLatin1String(cipher1));
        if(logSocket->write(logInfo1.toLatin1(), logInfo1.length()) != logInfo1.length())
        {
            qDebug()<<"[sendLog01]: "<<"error";
        }
        qDebug()<<"[sendLog01]: "<<logInfo1.length();
    }

}

void logDialog::getN0()
{
    QDate date;
    QTime time;
    QDateTime dt;

    dt.setDate(date.currentDate());
    dt.setTime(time.currentTime());
    QString current = dt.toString("yyyy:MM:dd:hh:mm:ss");
    char str[25];
    memset(str, '\0', 25);
    memcpy(str, current.toLatin1().data(), current.length());
    memset(n0, '\0', 2);
    memcpy(n0, str + 18, 1);
    qDebug()<<"[getN0]: "<<n0;

}

void logDialog::makePubS()
{
    unsigned char *PKey = PkeyS;
    int PublicKeyLen = 74;
    pubS = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, PublicKeyLen);
}

void logDialog::on_RegBtn_clicked()
{
    this->hide();
    emit(showReg());
}
