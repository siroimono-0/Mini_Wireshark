/****************************************************************************
** Meta object code from reading C++ file 'Pcap.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Pcap.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Pcap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN6WorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto Worker::qt_create_metaobjectdata<qt_meta_tag_ZN6WorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Worker",
        "sig_end",
        ""
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sig_end'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Worker, qt_meta_tag_ZN6WorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Worker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6WorkerE_t>.metaTypes,
    nullptr
} };

void Worker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Worker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sig_end(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Worker::*)()>(_a, &Worker::sig_end, 0))
            return;
    }
}

const QMetaObject *Worker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Worker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Worker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Worker::sig_end()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {
struct qt_meta_tag_ZN4PcapE_t {};
} // unnamed namespace

template <> constexpr inline auto Pcap::qt_create_metaobjectdata<qt_meta_tag_ZN4PcapE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Pcap",
        "sig_md",
        "",
        "sig_stop_flag",
        "sig_wk_flag",
        "create_Th",
        "QVariant",
        "qba",
        "ft",
        "stop_Th",
        "reset_md",
        "pcapFile_Read",
        "path",
        "save_md",
        "md",
        "Capture*",
        "stop_flag",
        "wk_flag"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sig_md'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sig_stop_flag'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sig_wk_flag'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'create_Th'
        QtMocHelpers::MethodData<void(QVariant, QVariant)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 }, { 0x80000000 | 6, 8 },
        }}),
        // Method 'stop_Th'
        QtMocHelpers::MethodData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reset_md'
        QtMocHelpers::MethodData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'pcapFile_Read'
        QtMocHelpers::MethodData<void(QString)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 },
        }}),
        // Method 'save_md'
        QtMocHelpers::MethodData<void(QString)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'md'
        QtMocHelpers::PropertyData<Capture*>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Final, 0),
        // property 'stop_flag'
        QtMocHelpers::PropertyData<bool>(16, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 1),
        // property 'wk_flag'
        QtMocHelpers::PropertyData<bool>(17, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 2),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Pcap, qt_meta_tag_ZN4PcapE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Pcap::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4PcapE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4PcapE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4PcapE_t>.metaTypes,
    nullptr
} };

void Pcap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Pcap *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sig_md(); break;
        case 1: _t->sig_stop_flag(); break;
        case 2: _t->sig_wk_flag(); break;
        case 3: _t->create_Th((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[2]))); break;
        case 4: _t->stop_Th(); break;
        case 5: _t->reset_md(); break;
        case 6: _t->pcapFile_Read((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->save_md((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Pcap::*)()>(_a, &Pcap::sig_md, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Pcap::*)()>(_a, &Pcap::sig_stop_flag, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Pcap::*)()>(_a, &Pcap::sig_wk_flag, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<Capture**>(_v) = _t->get_md(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->get_stop_flag(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->get_wk_flag(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->set_stop_flag(*reinterpret_cast<bool*>(_v)); break;
        case 2: _t->set_wk_flag(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Pcap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Pcap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4PcapE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Pcap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Pcap::sig_md()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Pcap::sig_stop_flag()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Pcap::sig_wk_flag()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
