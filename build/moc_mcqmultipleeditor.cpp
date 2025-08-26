/****************************************************************************
** Meta object code from reading C++ file 'mcqmultipleeditor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../editors/mcqmultipleeditor.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mcqmultipleeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
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
struct qt_meta_tag_ZN17McqMultipleEditorE_t {};
} // unnamed namespace

template <> constexpr inline auto McqMultipleEditor::qt_create_metaobjectdata<qt_meta_tag_ZN17McqMultipleEditorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "McqMultipleEditor",
        "addOption",
        ""
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'addOption'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<McqMultipleEditor, qt_meta_tag_ZN17McqMultipleEditorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject McqMultipleEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseQuestionEditor::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17McqMultipleEditorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17McqMultipleEditorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17McqMultipleEditorE_t>.metaTypes,
    nullptr
} };

void McqMultipleEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<McqMultipleEditor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->addOption(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *McqMultipleEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *McqMultipleEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17McqMultipleEditorE_t>.strings))
        return static_cast<void*>(this);
    return BaseQuestionEditor::qt_metacast(_clname);
}

int McqMultipleEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseQuestionEditor::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
