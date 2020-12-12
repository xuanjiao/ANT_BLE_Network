/****************************************************************************
** Meta object code from reading C++ file 'antwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../antwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'antwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ANTWidget_t {
    QByteArrayData data[15];
    char stringdata0[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ANTWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ANTWidget_t qt_meta_stringdata_ANTWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ANTWidget"
QT_MOC_LITERAL(1, 10, 12), // "recv_new_msg"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 12), // "send_new_msg"
QT_MOC_LITERAL(4, 37, 3), // "msg"
QT_MOC_LITERAL(5, 41, 8), // "new_text"
QT_MOC_LITERAL(6, 50, 11), // "displayText"
QT_MOC_LITERAL(7, 62, 16), // "handleBeaconInfo"
QT_MOC_LITERAL(8, 79, 22), // "handleBeaconDisconnect"
QT_MOC_LITERAL(9, 102, 21), // "on_QuitButton_clicked"
QT_MOC_LITERAL(10, 124, 21), // "on_SendButton_clicked"
QT_MOC_LITERAL(11, 146, 27), // "on_DisconnectButton_clicked"
QT_MOC_LITERAL(12, 174, 20), // "on_NewDevice_clicked"
QT_MOC_LITERAL(13, 195, 16), // "handleGoToSearch"
QT_MOC_LITERAL(14, 212, 15) // "handleNewBeacon"

    },
    "ANTWidget\0recv_new_msg\0\0send_new_msg\0"
    "msg\0new_text\0displayText\0handleBeaconInfo\0"
    "handleBeaconDisconnect\0on_QuitButton_clicked\0"
    "on_SendButton_clicked\0on_DisconnectButton_clicked\0"
    "on_NewDevice_clicked\0handleGoToSearch\0"
    "handleNewBeacon"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ANTWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    1,   75,    2, 0x06 /* Public */,
       5,    1,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   81,    2, 0x0a /* Public */,
       7,    3,   84,    2, 0x0a /* Public */,
       8,    1,   91,    2, 0x0a /* Public */,
       9,    0,   94,    2, 0x08 /* Private */,
      10,    0,   95,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    1,   98,    2, 0x08 /* Private */,
      14,    1,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void ANTWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ANTWidget *_t = static_cast<ANTWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recv_new_msg(); break;
        case 1: _t->send_new_msg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->new_text((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->displayText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->handleBeaconInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->handleBeaconDisconnect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_QuitButton_clicked(); break;
        case 7: _t->on_SendButton_clicked(); break;
        case 8: _t->on_DisconnectButton_clicked(); break;
        case 9: _t->on_NewDevice_clicked(); break;
        case 10: _t->handleGoToSearch((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->handleNewBeacon((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ANTWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ANTWidget::recv_new_msg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ANTWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ANTWidget::send_new_msg)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ANTWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ANTWidget::new_text)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ANTWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ANTWidget.data,
      qt_meta_data_ANTWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ANTWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ANTWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ANTWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ANTWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ANTWidget::recv_new_msg()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ANTWidget::send_new_msg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ANTWidget::new_text(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
