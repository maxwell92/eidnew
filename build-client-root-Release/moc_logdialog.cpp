/****************************************************************************
** Meta object code from reading C++ file 'logdialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../client/logdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_logDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      21,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,   10,   10,   10, 0x08,
      54,   10,   10,   10, 0x08,
      74,   10,   10,   10, 0x08,
      97,   83,   10,   10, 0x08,
     127,   10,   10,   10, 0x08,
     136,   10,   10,   10, 0x08,
     158,  153,   10,   10, 0x08,
     173,   10,   10,   10, 0x08,
     185,   10,   10,   10, 0x08,
     195,   10,   10,   10, 0x08,
     206,  153,   10,   10, 0x08,
     224,   10,   10,   10, 0x08,
     232,  153,   10,   10, 0x08,
     252,  153,   10,   10, 0x08,
     268,   10,   10,   10, 0x08,
     282,   10,   10,   10, 0x08,
     291,   10,   10,   10, 0x08,
     304,   10,   10,   10, 0x08,
     313,   10,   10,   10, 0x08,
     321,   10,   10,   10, 0x08,
     329,  153,   10,   10, 0x08,
     354,  347,   10,   10, 0x08,
     383,  374,   10,   10, 0x08,
     407,   10,   10,   10, 0x08,
     426,   10,   10,   10, 0x08,
     436,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_logDialog[] = {
    "logDialog\0\0showReg()\0showMain()\0"
    "on_LoginBtn_clicked()\0on_RegBtn_clicked()\0"
    "recvSP()\0username,skey\0"
    "CreateLogger(QString,QString)\0rsaKey()\0"
    "ExchangeRSAKey()\0code\0Enpack(char[])\0"
    "connectSP()\0sendRSA()\0makePubS()\0"
    "Enpacklog(char[])\0getN0()\0EncryptpubS(char[])\0"
    "sendLog(char[])\0DecryptPriC()\0Depack()\0"
    "dealIdlist()\0getXor()\0getN2()\0gety1()\0"
    "EncryptK1(char[])\0in,out\0hash(char[],char[])\0"
    "username\0getMfromUsername(char*)\0"
    "connecteIDclient()\0sendeID()\0recveID()\0"
};

void logDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        logDialog *_t = static_cast<logDialog *>(_o);
        switch (_id) {
        case 0: _t->showReg(); break;
        case 1: _t->showMain(); break;
        case 2: _t->on_LoginBtn_clicked(); break;
        case 3: _t->on_RegBtn_clicked(); break;
        case 4: _t->recvSP(); break;
        case 5: _t->CreateLogger((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->rsaKey(); break;
        case 7: _t->ExchangeRSAKey(); break;
        case 8: _t->Enpack((*reinterpret_cast< char(*)[]>(_a[1]))); break;
        case 9: _t->connectSP(); break;
        case 10: _t->sendRSA(); break;
        case 11: _t->makePubS(); break;
        case 12: _t->Enpacklog((*reinterpret_cast< char(*)[]>(_a[1]))); break;
        case 13: _t->getN0(); break;
        case 14: _t->EncryptpubS((*reinterpret_cast< char(*)[]>(_a[1]))); break;
        case 15: _t->sendLog((*reinterpret_cast< char(*)[]>(_a[1]))); break;
        case 16: _t->DecryptPriC(); break;
        case 17: _t->Depack(); break;
        case 18: _t->dealIdlist(); break;
        case 19: _t->getXor(); break;
        case 20: _t->getN2(); break;
        case 21: _t->gety1(); break;
        case 22: _t->EncryptK1((*reinterpret_cast< char(*)[]>(_a[1]))); break;
        case 23: _t->hash((*reinterpret_cast< char(*)[]>(_a[1])),(*reinterpret_cast< char(*)[]>(_a[2]))); break;
        case 24: _t->getMfromUsername((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 25: _t->connecteIDclient(); break;
        case 26: _t->sendeID(); break;
        case 27: _t->recveID(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData logDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject logDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_logDialog,
      qt_meta_data_logDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &logDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *logDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *logDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_logDialog))
        return static_cast<void*>(const_cast< logDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int logDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void logDialog::showReg()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void logDialog::showMain()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
