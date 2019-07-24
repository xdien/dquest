TARGET = dquest

load(qt_module)

CONFIG += build_dquest_lib c++11
CONFIG-=create_cmake
include(dquestlib.pri)

DEFINES += DQUEST_EXPORTS
win32-clang-msvc* :{
    QMAKE_CXXFLAGS = +C99
}

#Define this macro if you want to run tests, so more AIPs will get exported.
#DEFINES += XLSX_TEST

QMAKE_TARGET_COMPANY = "Xuan Dien"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2018 Xuan Dien <xdienw@gmai.com>"
QMAKE_TARGET_DESCRIPTION = "databases wirter for Qt5"
