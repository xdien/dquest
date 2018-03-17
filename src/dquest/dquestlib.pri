INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core gui gui-private widgets sql
!build_dquest_lib:DEFINES += DQUEST_NO_LIB

HEADERS += \
    $$PWD/dqabstractmodel.h \
    $$PWD/dqbasefield.h \
    $$PWD/dqclause.h \
    $$PWD/dqconnection.h \
    $$PWD/dqexpression.h \
    $$PWD/dqfield.h \
    $$PWD/dqforeignkey.h \
    $$PWD/dqindex.h \
    $$PWD/dqlist.h \
    $$PWD/dqlistwriter.h \
    $$PWD/dqmetainfoquery_p.h \
    $$PWD/dqmodel.h \
    $$PWD/dqmodelmetainfo.h \
    $$PWD/dqquery.h \
    $$PWD/dqqueryrules.h \
    $$PWD/dqsharedlist.h \
    $$PWD/dqsharedquery.h \
    $$PWD/dqsharedquery_p.h \
    $$PWD/dqsql.h \
    $$PWD/dqsqlitestatement.h \
    $$PWD/dqsqlstatement.h \
    $$PWD/dqstream.h \
    $$PWD/dquest.h \
    $$PWD/dqwhere.h \
    $$PWD/dqwhere_p.h

SOURCES += \
    $$PWD/dqabstractmodel.cpp \
    $$PWD/dqbasefield.cpp \
    $$PWD/dqclause.cpp \
    $$PWD/dqconnection.cpp \
    $$PWD/dqexpression.cpp \
    $$PWD/dqfield.cpp \
    $$PWD/dqindex.cpp \
    $$PWD/dqlistwriter.cpp \
    $$PWD/dqmodel.cpp \
    $$PWD/dqmodelmetainfo.cpp \
    $$PWD/dqqueryrules.cpp \
    $$PWD/dqsharedlist.cpp \
    $$PWD/dqsharedquery.cpp \
    $$PWD/dqsql.cpp \
    $$PWD/dqsqlitestatement.cpp \
    $$PWD/dqsqlstatement.cpp \
    $$PWD/dqstream.cpp \
    $$PWD/dqwhere.cpp


