#include "regdialog.h"
#include "ui_regdialog.h"
#include "QString"
#include "QByteArray"
#include "QMessageBox"
#include "user.h"
#include "openssl/md5.h"
#include "openssl/aes.h"
#include "stdio.h"
#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>




regDialog::regDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regDialog)
{
    ui->setupUi(this);
    tmpUser = new user();
    verUser = new user();
    client = new QTcpSocket(this);

    connect(client, SIGNAL(readyRead()), this, SLOT(acceptSP()));
}

regDialog::~regDialog()
{
    delete ui;
}

void regDialog::changeEvent(QEvent *e)
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


void regDialog::on_ResetBtn_clicked()
{
    ui->UsernameLinedit->clear();
    ui->eidnumLinedit->clear();
    ui->skeyLinedit->clear();
    ui->skeyLinedit2->clear();
}

void regDialog::on_SubmitBtn_clicked()
{
    QString skeyInput = ui->skeyLinedit->text();
    QString skey2Input = ui->skeyLinedit2->text();
    QString usernameInput = ui->UsernameLinedit->text();
    QString eidNumInput = ui->eidnumLinedit->text();

    if(usernameInput.isEmpty() || eidNumInput.isEmpty() || skeyInput.isEmpty() || skey2Input.isEmpty())
    {
        QMessageBox::about(this, "Attention", "Can't be Empty!");
    }
    if(skeyInput.compare(skey2Input) != 0)
    {
        QMessageBox::about(this, "Attention!", "Security Keys don't match!");
    }

    createTmpUser(usernameInput, eidNumInput, skeyInput);
    Enpack("01");
    EncryptK0();
    connectSP();
    sendSP();

}

void regDialog::EncryptK0()
{
    unsigned char InBuff[16], OutBuff[16];

    char *plain;
    int len_plain;
    len_plain = strlen(msg1);
    plain = (char *)malloc(len_plain * sizeof(char));
    memset(plain, '\0', len_plain);

    memcpy(plain, msg1, strlen(msg1));


    len_cipher1 = (len_plain / 16 + 1) * 16;
    qDebug()<<"[EncryptK0]: "<<len_cipher1;
    cipher1 = (char *)malloc(len_cipher1 * sizeof(char));
    memset(cipher1, '\0', len_cipher1);

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
        memcpy(cipher1 + 16 * i, (char *)OutBuff, 16);
    }

}

void regDialog::Enpack(char code[])
{
    if(!strcmp(code,"01"))
    {
        len_msg1 = strlen(tmpUser->username) + strlen(tmpUser->eidNum) + strlen(tmpUser->m) + 2 + 1;
        msg1 = (char *)malloc(len_msg1 * sizeof(char));
        memset(msg1, '\0', len_msg1);

        strcpy(msg1, (char *)code);
        strcat(msg1, tmpUser->username);
        strcat(msg1, tmpUser->eidNum);
        strcat(msg1, tmpUser->m);

        qDebug()<<"[Enpack01]: "<<msg1;
        qDebug()<<"[Enpack01]: "<<len_msg1;

    }else if(!strcmp(code, "03"))
    {
        memcpy(msg3, code, 2);
        char ack[] = "y";
        memcpy(msg3 + 2 + 32, ack, 1);
        qDebug()<<"[Enpack03]: "<<msg3;
        len_msg3 = 36;  //severe error
        qDebug()<<"[Enpack03]: "<<len_msg3;
    }
}

void regDialog::createTmpUser(QString username, QString eidNum, QString skey)
{
    QByteArray b;
    b = username.toLatin1();
    memcpy(tmpUser->username, b.data(), username.length());

    b = eidNum.toLatin1();
    memcpy(tmpUser->eidNum, b.data(), eidNum.length());

    b = skey.toLatin1();
    memcpy(tmpUser->skey, b.data(), skey.length());

    //memcpy(tmpUser->m, getMfromUsername(tmpUser->username), strlen(tmpUser->m));
    getMfromUsername(tmpUser->username);

//    qDebug()<<tmpUser->username;
//    qDebug()<<tmpUser->m;

}

void regDialog::acceptSP()
{
    QString spInfo1 = client->readAll();
    QByteArray b;
    b = spInfo1.toLatin1();

    //len_cipher2 = (spInfo1.length() / 16 + 1) * 16;
    len_cipher2 = 0;
    memset(cipher2, '\0', 256);
    len_cipher2 = spInfo1.length();
    qDebug()<<"[acceptSP]: "<<len_cipher2;


//    cipher2 = (char *)malloc(len_cipher2 * sizeof(char));
//    memset(cipher2, '\0', len_cipher2);

//    strcpy(cipher2, b.data());
    memcpy(cipher2, b.data(), len_cipher2);
    qDebug()<<"[acceptSP]: "<<cipher2;

    if(!strcmp(cipher2, "existed"))
    {
        QMessageBox::about(this, "Attention!", "Username Existed!");
    }else if(!strcmp(cipher2, "succ"))
    {
        QMessageBox::about(this, "Attention!", "Registed Success!");
        client->close();

        emit(showLog());
        this->hide();
    }else {
        DecryptK0();
        Depack();
        if(Verify())
        {
            caly0();
            Enpack("03");
            Encrypt1K0();
            SendAck();
        }
    }

}

void regDialog::Encrypt1K0()
{
    unsigned char InBuff[16], OutBuff[16];

    char *plain;
    int len_plain;
    len_plain = strlen(msg3);
    plain = (char *)malloc(len_plain * sizeof(char));
    memset(plain, '\0', len_plain);

    memcpy(plain, msg3, strlen(msg3));

    len_cipher3 = 0;
    len_cipher3 = (len_plain / 16 + 1) * 16;
//    cipher3 = (char *)malloc(len_cipher3 * sizeof(char));
//    memset(cipher3, '\0', len_cipher3);
    memset(cipher3, '\0', 256);

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
        memcpy(cipher3 + 16 * i, (char *)OutBuff, 16);
    }

}

void regDialog::caly0()
{
    QString skey = ui->skeyLinedit->text();
    char *sekey;
    int len_sekey;
    len_sekey = skey.length();
    sekey = (char *)malloc(len_sekey * sizeof(char));
    memset(sekey, '\0', len_sekey);

    memcpy(sekey, skey.toLatin1().data(), len_sekey);  //p

    int round;
    round = tmpUser->m[0] % 10 + 20;  //round is [20..29];
    qDebug()<<"[caly0]: "<<round;

    char in[50], out[50];
    memcpy(in, sekey, len_sekey);
    memcpy(in + len_sekey, tmpUser->ti, 19);

    for(int i = 0; i < round - 1; i++ )  //m-1
    {
        memset(out, '\0', 50);
        hash(in,out);
        memcpy(in, out, 50);

    }

    qDebug()<<"[caly0]: "<<in;


    int len_msg3 = 2 + 32 + 2;
//    msg3 = (char *)malloc(len_msg3 * sizeof(char));
//    memset(msg3, '\0', len_msg3);
    memset(msg3, '\0', 256);
    memcpy(msg3 + 2, in, 32);
    qDebug()<<"[caly0]: "<<msg3 + 2;
    qDebug()<<"[caly0]: "<<len_msg3;
}

void regDialog::hash(char in[], char out[])
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


//    strcpy(tmpUser->m, buf);
    strcpy(out, buf);
}

void regDialog::DecryptK0()
{
    unsigned char InBuff[16], OutBuff[16];

    char *plain;
    int len_plain;
    len_plain = len_cipher2;
    plain = (char *)malloc(len_plain * sizeof(char));
    memset(plain, '\0', len_plain);

    unsigned char Seed[32];
    strcpy((char *)Seed, "AbCdEfGhIjKlMnOpQrStUvWxYz12345");

    AES_KEY AESEncryptKey, AESDecryptKey;
    AES_set_encrypt_key(Seed, 256, &AESEncryptKey);
    AES_set_decrypt_key(Seed, 256, &AESDecryptKey);

    int i;
    int num1;
    num1 = len_cipher2 / 16 + 1;

    for (i = 0; i < num1; i++) {
             memset((char *)InBuff, '\0', 16);
             memset((char *)OutBuff, '\0', 16);

             memcpy((char *)OutBuff, cipher2 + 16 * i, 16);

             AES_ecb_encrypt(OutBuff, InBuff, &AESDecryptKey, AES_DECRYPT);
             if(i == (num1 - 1)) {
                     memcpy(plain + 16 * i, (char *)InBuff, len_plain % 16);
             }else {
                     memcpy(plain + 16 * i, (char *)InBuff, 16);
             }
    }

    len_msg2 = 0;
    memset(msg2, '\0', 256);
    len_msg2 = strlen(plain);
//    msg2 = (char *)malloc(len_msg2 * sizeof(char));
//    memset(msg2, '\0', len_msg2);
    memcpy(msg2, plain, len_plain);
    qDebug()<<"[DecryptK0]: "<<msg2;
}

void regDialog::Depack()
{
    char code[3];
    memset(code, '\0', 3);
    memcpy(code, msg2, 2);
    qDebug()<<"[Depack]: "<<strlen(msg2);
    qDebug()<<"[Depack]: "<<code;
    int len_name;
    len_name = len_msg2 - 2 - 32 - 8 - 19;
    qDebug()<<"[Depack]: "<<len_name;

    memcpy(verUser->username, msg2 + 2, len_name);
    memcpy(verUser->eidNum, msg2 + 2 + len_name, 8);
    memcpy(verUser->m, msg2 + 2 + len_name + 8, 32);
    memcpy(verUser->ti, msg2 + 2 + len_name + 8 + 32, 19);

    qDebug()<<"[Depack]: "<<verUser->username;
    qDebug()<<"[Depack]: "<<verUser->eidNum;
    qDebug()<<"[Depack]: "<<verUser->m;
    qDebug()<<"[Depack]: "<<verUser->ti;
}

int regDialog::Verify()
{
    if(strcmp(tmpUser->username, verUser->username) || strcmp(tmpUser->eidNum, verUser->eidNum) || strcmp(tmpUser->m, verUser->m))
    {
        qDebug()<<"[Verify]: "<<"error";
        exit(1);
    } else {
        memcpy(tmpUser->ti, verUser->ti, 19);
        qDebug()<<"[Verify]: "<<"true";
        return 1;
    }

}

void regDialog::SendAck()
{
    QString reginfo2;
    reginfo2 = QString(QLatin1String(cipher3));
    if(client->write(reginfo2.toLatin1(), reginfo2.length()) != reginfo2.length())
    {
        qDebug()<<"[sendAck]: "<<"error";
    }
    qDebug()<<"[sendAck]: "<<reginfo2.length();

}

void regDialog::connectSP()
{
    client->connectToHost(QHostAddress::LocalHost, 11500);

}

void regDialog::sendSP()
{
    QString reginfo1;
    reginfo1 = QString(QLatin1String(cipher1));
    if(client->write(reginfo1.toLatin1(), reginfo1.length()) != reginfo1.length())
    {
        qDebug()<<"[sendSP]: "<<"error";
    }
    qDebug()<<"[sendSP]: "<<reginfo1.length();

}

void regDialog::getMfromUsername(char *username)
{
    char buf[33] = {'\0'};
    char tmp[3] = {'\0'};
    unsigned char md[16];

//    QString str;
//    QByteArray b;
//    b = str.toLatin1();


    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, username, strlen(username));
    MD5_Final(md, &ctx);

    for(int i = 0; i < 16; i++ )
    {
        sprintf(tmp, "%2.2x", md[i]);
        strcat(buf, tmp);
    }


    strcpy(tmpUser->m, buf);

}

