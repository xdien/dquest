TARGET = dquest
TEMPLATE = lib

CONFIG += build_dquest_lib c++17 C99 shared
include(dquestlib.pri)

DEFINES += DQUEST_EXPORTS

# Basic install rules for Qt
target.path = $$[QT_INSTALL_LIBS]
headers.path = $$[QT_INSTALL_HEADERS]/dquest
headers.files = $$HEADERS dquest

modules.path = $$[QT_HOST_DATA]/mkspecs/modules
modules.files = qt_lib_dquest.pri

INSTALLS += target headers modules

win32: {
    dlltarget.path = $$[QT_INSTALL_BINS]
    INSTALLS += dlltarget
}

QMAKE_TARGET_COMPANY = "Xuan Dien"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2018 Xuan Dien <xdienw@gmai.com>"
QMAKE_TARGET_DESCRIPTION = "databases writer for Qt5/Qt6"
