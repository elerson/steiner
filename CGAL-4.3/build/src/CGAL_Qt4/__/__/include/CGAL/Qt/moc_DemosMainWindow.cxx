/****************************************************************************
** Meta object code from reading C++ file 'DemosMainWindow.h'
**
** Created: Thu Jan 23 23:28:19 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../include/CGAL/Qt/DemosMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DemosMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGAL__Qt__DemosMainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   27,   26,   26, 0x05,

 // slots: signature, parameters, type, tag, flags
      60,   26,   26,   26, 0x0a,
     100,   92,   26,   26, 0x09,
     125,   92,   26,   26, 0x09,
     144,   26,   26,   26, 0x09,
     161,   26,   26,   26, 0x09,
     178,   26,   26,   26, 0x09,
     208,  199,   26,   26, 0x09,
     234,   26,   26,   26, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CGAL__Qt__DemosMainWindow[] = {
    "CGAL::Qt::DemosMainWindow\0\0filename\0"
    "openRecentFile(QString)\0"
    "setMaxNumberOfRecentFiles(uint)\0checked\0"
    "setUseAntialiasing(bool)\0setUseOpenGL(bool)\0"
    "popupAboutCGAL()\0popupAboutDemo()\0"
    "openRecentFile_aux()\0fileName\0"
    "addToRecentFiles(QString)\0"
    "updateRecentFileActions()\0"
};

void CGAL::Qt::DemosMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DemosMainWindow *_t = static_cast<DemosMainWindow *>(_o);
        switch (_id) {
        case 0: _t->openRecentFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setMaxNumberOfRecentFiles((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 2: _t->setUseAntialiasing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setUseOpenGL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->popupAboutCGAL(); break;
        case 5: _t->popupAboutDemo(); break;
        case 6: _t->openRecentFile_aux(); break;
        case 7: _t->addToRecentFiles((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->updateRecentFileActions(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CGAL::Qt::DemosMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CGAL::Qt::DemosMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CGAL__Qt__DemosMainWindow,
      qt_meta_data_CGAL__Qt__DemosMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CGAL::Qt::DemosMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CGAL::Qt::DemosMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CGAL::Qt::DemosMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGAL__Qt__DemosMainWindow))
        return static_cast<void*>(const_cast< DemosMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CGAL::Qt::DemosMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CGAL::Qt::DemosMainWindow::openRecentFile(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
