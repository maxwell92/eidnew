/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../eidServer/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      21,   11,   11,   11, 0x08,
      33,   11,   11,   11, 0x08,
      43,   11,   11,   11, 0x08,
      55,   11,   11,   11, 0x08,
      65,   11,   11,   11, 0x08,
      74,   11,   11,   11, 0x08,
      90,   11,   11,   11, 0x08,
     103,   11,   99,   11, 0x08,
     112,   11,   11,   11, 0x08,
     122,   11,   11,   11, 0x08,
     136,  131,   11,   11, 0x08,
     156,   11,   11,   11, 0x08,
     168,   11,   11,   11, 0x08,
     177,   11,   11,   11, 0x08,
     195,  188,   11,   11, 0x08,
     225,  215,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0getRsa()\0newClient()\0"
    "recvCli()\0rsaEnpack()\0sendRsa()\0"
    "recvSP()\0DecryptPrieid()\0Depack()\0int\0"
    "Verify()\0getPubS()\0Enpack()\0code\0"
    "EncryptPubS(char[])\0connectSP()\0"
    "sendSP()\0makePubS()\0in,out\0"
    "hash(char[],char[])\0eidNumber\0"
    "fillSomeone(char[])\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->getRsa(); break;
        case 1: _t->newClient(); break;
        case 2: _t->recvCli(); break;
        case 3: _t->rsaEnpack(); break;
        case 4: _t->sendRsa(); break;
        case 5: _t->recvSP(); break;
        case 6: _t->DecryptPrieid(); break;
        case 7: _t->Depack(); break;
        case 8: { int _r = _t->Verify();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: _t->getPubS(); break;
        case 10: _t->Enpack(); break;
        case 11: _t->EncryptPubS((*reinterpret_cast< char(*)[]>(_a[1]))); break;
        case 12: _t->connectSP(); break;
        case 13: _t->sendSP(); break;
        case 14: _t->makePubS(); break;
        case 15: _t->hash((*reinterpret_cast< char(*)[]>(_a[1])),(*reinterpret_cast< char(*)[]>(_a[2]))); break;
        case 16: _t->fillSomeone((*reinterpret_cast< char(*)[]>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
