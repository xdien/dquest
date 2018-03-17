TARGET = dquest

load(qt_module)

CONFIG += build_dquest_lib c++11
CONFIG-=create_cmake
include(dquestlib.pri)


contains(CONFIG, static_build){
    CONFIG += staticlib
}

!contains(CONFIG, staticlib){
    CONFIG += lib
    CONFIG += dll
}

CONFIG += create_prl
CONFIG += link_prl

macx{
    CONFIG  -= dll
    CONFIG  += lib_bundle
    CONFIG  += plugin
}

DEFINES += DQUEST_EXPORTS

contains(CONFIG, staticlib){
    DEFINES += HAVE_STATIC_BUILD
    message(STATIC_BUILD)
    DEFINES -= DQUEST_EXPORTS
}

#Define this macro if you want to run tests, so more AIPs will get exported.
#DEFINES += XLSX_TEST

QMAKE_TARGET_COMPANY = "Xuan Dien"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2018 Xuan Dien <xdienw@gmai.com>"
QMAKE_TARGET_DESCRIPTION = "databases wirter for Qt5"

