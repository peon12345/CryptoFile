/****************************************************************************
** Meta object code from reading C++ file 'log.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../crypto/source/log.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'log.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Log_t {
    QByteArrayData data[12];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Log_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Log_t qt_meta_stringdata_Log = {
    {
QT_MOC_LITERAL(0, 0, 3), // "Log"
QT_MOC_LITERAL(1, 4, 10), // "needBackUp"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 21), // "needClearBackUpFolder"
QT_MOC_LITERAL(4, 38, 10), // "addMessage"
QT_MOC_LITERAL(5, 49, 7), // "message"
QT_MOC_LITERAL(6, 57, 4), // "type"
QT_MOC_LITERAL(7, 62, 11), // "addProgress"
QT_MOC_LITERAL(8, 74, 7), // "percent"
QT_MOC_LITERAL(9, 82, 10), // "showResult"
QT_MOC_LITERAL(10, 93, 23), // "on_CancelButton_clicked"
QT_MOC_LITERAL(11, 117, 23) // "on_BackUpButton_clicked"

    },
    "Log\0needBackUp\0\0needClearBackUpFolder\0"
    "addMessage\0message\0type\0addProgress\0"
    "percent\0showResult\0on_CancelButton_clicked\0"
    "on_BackUpButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Log[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   56,    2, 0x0a /* Public */,
       4,    1,   61,    2, 0x2a /* Public | MethodCloned */,
       7,    1,   64,    2, 0x0a /* Public */,
       9,    0,   67,    2, 0x0a /* Public */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Log::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Log *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->needBackUp(); break;
        case 1: _t->needClearBackUpFolder(); break;
        case 2: _t->addMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->addMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->addProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->showResult(); break;
        case 6: _t->on_CancelButton_clicked(); break;
        case 7: _t->on_BackUpButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Log::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Log::needBackUp)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Log::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Log::needClearBackUpFolder)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Log::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Log.data,
    qt_meta_data_Log,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Log::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Log::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Log.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Log::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Log::needBackUp()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Log::needClearBackUpFolder()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
