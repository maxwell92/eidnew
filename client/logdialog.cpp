#include "logdialog.h"
#include "ui_logdialog.h"
#include "openssl/rsa.h"
#include "QByteArray"
#include "string.h"
#include "QDebug"
#include "QHostAddress"
#include "QMessageBox"
#include <QDateTime>
#include "openssl/md5.h"
#include "openssl/aes.h"


logDialog::logDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logDialog)
{
    ui->setupUi(this);


    logger = new user();
    logSocket = new QTcpSocket(this);
    connect(logSocket, SIGNAL(readyRead()), this, SLOT(recvSP()));

    eidSocket = new QTcpSocket(this);
    connect(eidSocket, SIGNAL(readyRead()), this, SLOT(recveID()));
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

void logDialog::recveID()
{

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
    unsigned char *PKey = PublicKey;

    int publicKeyLen = i2d_RSAPublicKey(privateKey, &PKey);

    qDebug()<<"[rsaKey]: "<<publicKeyLen;

//    for(int i = 0; i < publicKeyLen; i++)
//    {
//        printf("%d, ", PublicKey[i]);
//    }
//    printf("\n");

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
    }


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

    len_msg2 = 0;
    memset(msg2, '\0', 256);
    len_msg2 = rsaInfo1.length();
    memcpy(msg2, b.data(), len_msg2);
//    msg2 = (char *)malloc(len_msg2 * sizeof(char));
//  strcpy(msg2, b.data());
//    qDebug()<<"[recvSP]: "<<msg2;

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

        getXor();
        getN2();
        gety1();
        Enpacklog("03");
        EncryptK1("03");
        sendLog("03");
        dealIdlist();
        connecteIDclient();
        sendeID();
    }
}

void logDialog::connecteIDclient()
{
    eidSocket->connectToHost(QHostAddress::LocalHost, 13500);

}

void logDialog::sendeID()
{
    QString eidInfo1;
    eidInfo1 = QString(QLatin1String(hn2));
    if(eidSocket->write(eidInfo1.toLatin1(), eidInfo1.length()) != eidInfo1.length())
    {
        qDebug()<<"[sendeID]: "<<"error";
    }
    qDebug()<<"[sendeID]: "<<eidInfo1;
    qDebug()<<"[sendeID]: "<<eidInfo1.length();
}

void logDialog::getMfromUsername(char *username)
{
    char buf[33] = {'\0'};
    char tmp[3] = {'\0'};
    unsigned char md[16];

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, username, strlen(username));
    MD5_Final(md, &ctx);

    for(int i = 0; i < 16; i++ )
    {
        sprintf(tmp, "%2.2x", md[i]);
        strcat(buf, tmp);
    }


    strcpy(logger->m, buf);
}

void logDialog::getXor()
{

    getMfromUsername(logger->username);
    memset(logger->k1, '\0', 32);
    for(int i = 0; i < 32; i++)
    {
        logger->k1[i] = logger->m[i] ^ logger->m1[i];
    }
    qDebug()<<"[getXor]: "<<logger->k1;

}

void logDialog::hash(char in[], char out[])
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

void logDialog::getN2()
{
    memset(n2, '\0', 2);
    n2[0] = n0[0] + 2;
    memset(hn1, '\0', 32);
    hash(n2, hn2);

}

void logDialog::gety1()
{
    char *sekey;
    int len_sekey;
    len_sekey = strlen(logger->skey);
    sekey = (char *)malloc(len_sekey * sizeof(char));
    memset(sekey, '\0', len_sekey);

    //memcpy(sekey, skey.toLatin1().data(), len_sekey);  //p
    memcpy(sekey, logger->skey, strlen(logger->skey));

    int round;
    round = logger->m[0] % 10 + 20; //round is [20..29];
    qDebug()<<"[gety1]: "<<round;

    char in[50], out[50];
    memcpy(in, sekey, len_sekey);
    memcpy(in + len_sekey, logger->ti, 19);

    for(int i = 0; i < round - 2; i++ )  //m-1
    {
        memset(out, '\0', 50);
        hash(in,out);
        memcpy(in, out, 50);
    }

    memcpy(logger->y1, in, 32);
    qDebug()<<"[gety1]: "<<logger->y1;

}

void logDialog::EncryptK1(char code[])
{
    unsigned char InBuff[16], OutBuff[16];

    char *plain;
    int len_plain;
    len_plain = strlen(msg5);
    plain = (char *)malloc(len_plain * sizeof(char));
    memset(plain, '\0', len_plain);

    memcpy(plain, msg5, strlen(msg5));


    len_cipher3 = 0;
    memset(cipher3, '\0', 256);
    len_cipher3 = (len_plain / 16 + 1) * 16;

    unsigned char Seed[32];
    //strcpy((char *)Seed, "AbCdEfGhIjKlMnOpQrStUvWxYz12345");
    memcpy((char *)Seed, logger->k1, 32);

    AES_KEY AESEncryptKey, AESDecryptKey;
    AES_set_encrypt_key(Seed, 256, &AESEncryptKey);
    AES_set_decrypt_key(Seed, 256, &AESDecryptKey);


    int i;
    int num1;
    num1 = len_plain / 16 + 1;

    len_cipher3 = 0;
    memset(cipher3, '\0', 256);
    len_cipher3 = num1 * 16;

    for(i = 0; i < num1; i++) {
        memset((char *)InBuff, '\0', 16);
        memset((char *)OutBuff, '\0', 16);

        if (i == (num1 - 1)) {
            memcpy((char *)InBuff, plain + 16 * i, len_plain % 16);
        } else {
            memcpy((char *)InBuff, plain + 16 * i, 16);
        }

        AES_ecb_encrypt(InBuff, OutBuff, &AESEncryptKey, AES_ENCRYPT);
        memcpy(cipher3 + 16 * i, (char *)OutBuff, 16);
    }


}

void logDialog::DecryptPriC()
{
    len_cipher2 = 0;
    memset(cipher2, '\0', 256);
    len_cipher2 = len_msg2;
    memcpy(cipher2, msg2, len_cipher2);
    qDebug()<<"[DecryptPriC]: "<<len_cipher2;
//    qDebug()<<"[DecryptPriC]: "<<cipher2;

    int i;
    int num2;
    num2 = len_cipher2 / 64;


    if(len_cipher2 % 64 != 0)
    {
        char instead[2];
        memset(instead, '\0', 2);
        memcpy(instead, cipher2 + (num2) * 64 , 1);

        printf("[DecryptPriC]:instead: %d\n", instead[0]);
        int cnt = 0;
        for(int i = 0; i < (num2) * 64; i++)
        {
            if(cipher2[i] == instead[0])
            {
                cipher2[i] = 0;
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

    len_msg4 = 0;
    memset(msg4, '\0', 256);
    len_msg4 = len_cipher2 - 1;

//    msg4 = (char *)malloc(len_msg4 * sizeof(char));
//    memset(msg4, '\0', len_msg4);
    qDebug()<<"[DecryptPriC]: "<<len_msg4;


    unsigned char InBuff[64], OutBuff[64];
    for(i = 0; i < num2; i++) {
        memset((char *)InBuff, '\0', 64);
        memset((char *)OutBuff, '\0', 64);
//        strcpy((char *)OutBuff, cipher2 + 64 * i);
        memcpy((char *)OutBuff, cipher2 + 64 * i, 64);
        qDebug()<<"[DecryptPriC]: OutBuff["<<i<<"]: "<<OutBuff<<endl;
        qDebug()<<"[DecryptPriC]: OutBuff["<<i<<"]: "<<strlen((char *)OutBuff)<<endl;
        int plen = 0;
        plen = RSA_private_decrypt(64, (const unsigned char*)OutBuff, InBuff, privateKey, RSA_PKCS1_PADDING);
        qDebug()<<"[DecryptPriC]: InBuff["<<i<<"]: "<<InBuff<<endl;
        qDebug()<<"[DecryptPriC]: InBuff["<<i<<"]: "<<plen<<endl;

        memcpy(msg4 + i * 50, (char *)InBuff, 50);

//        if(i == (num2 - 1)) {
//            memcpy(msg4 + i * 50, (char *)InBuff, len_msg4 % 64);
//        }else {
//            memcpy(msg4 + i * 50, (char *)InBuff, 64);
//        }
            //strcat(msg2, (char *)InBuff);
    }
    qDebug()<<"[DecryptPriC]: msg4: "<<msg4;
    qDebug()<<"[DecryptPriC]: "<<strlen(msg4);
}
//    for(int i = 0; i < len_msg4; i++)
//    {
//        printf("msg4[%d]%d, ", i, msg4[i]);
//    }
//    printf("\n");


void logDialog::Depack()
{
    qDebug()<<"[Depack]: "<<msg4;

    char code[3];
    memset(code, '\0', 3);
    memcpy(code, msg4, 2);
    qDebug()<<"[Depack]: "<<code;
//    char code[]="02";
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
//        len_uname = strlen(msg4) - 2 - 8 - 32 - 4 - 19 - 19;

        memcpy(uname, msg4 + 2 + 8 + 32 + 4, len_uname);
        qDebug()<<"[Depack02]: "<<uname;

        memcpy(logger->ti, msg4 + 2 + 8 + 32 + 4 + len_uname, 19);
        qDebug()<<"[Depack02]: "<<logger->ti;

//        memcpy(logger->tm, msg4 + 2 + 8 + 32 + 4 + len_uname + 19, 19);
//        qDebug()<<"[Depack02]: "<<logger->tm;

        memcpy(logger->m1 ,msg4 + 2 + 8 + 32 + 4 + len_uname + 19, 32);
        qDebug()<<"[Depack02]: "<<logger->m1;




    }
}



void logDialog::dealIdlist()
{
    char idInfo[8][20] = {0};
    strcpy(idInfo[0], "teleNumber,");
    strcpy(idInfo[1], "realName,");
    strcpy(idInfo[2], "age,");
    strcpy(idInfo[3], "company,");
    strcpy(idInfo[4], "gender,");
    strcpy(idInfo[5], "address,");
    strcpy(idInfo[6], "city,");
    strcpy(idInfo[7], "idNumber,");

    char msg[128];
    memset(msg, '\0', 128);

    for(int i = 0; i < strlen(idlist); i++)
    {
        memcpy(msg + strlen(msg), idInfo[idlist[i] - '0'], strlen(idInfo[idlist[i] - '0']));
    }
   // strcat(msg, "\nEnsure eIDClient is running!");

    QString ndMsg;
    ndMsg = QString(QLatin1String(msg));

    QString noMsg;
    char msgt[128];
    memset(msgt, '\0', 128);
    strcpy(msgt, "Ensure eIDClient is running!");
    noMsg = QString(QLatin1String(msgt));

    QMessageBox::about(this, "Attention!", noMsg);
    QMessageBox::about(this, "Tips", ndMsg);


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
    if(!strcmp(code, "03"))
    {
        len_msg5 = 0;
        memset(msg5, '\0', 256);
        len_msg5 = 2 + 32 + 32;
        memcpy(msg5, code, 2);
        memcpy(msg5 + 2, hn2, 32);
        memcpy(msg5 + 2 + 32, logger->y1, 32);
        qDebug()<<"[Enpacklog03]: "<<msg5;
        qDebug()<<"[Enpacklog03]: "<<strlen(msg5);

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
                printf("%d, ", i);
            }
 //            printf("{%d}, ", cipher1[i]);
        }
        printf("\n%d\n", cnt);

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
    if(!strcmp(code, "03"))
    {
        logInfo1 = QString(QLatin1String(cipher3));
        if(logSocket->write(logInfo1.toLatin1(), logInfo1.length()) != logInfo1.length())
        {
            qDebug()<<"[sendLog03]: "<<"error";
        }
        qDebug()<<"[sendLog03]: "<<logInfo1.length();
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
