QT += qml testlib network sql
QT -= gui

CONFIG += c++11 qt

SOURCES += testmain.cpp \
    backendstuttgarttests.cpp

HEADERS += \
    backendstuttgarttests.h

INCLUDEPATH += ../../
include(../../harbour-vvs-incidents.pri)

TARGET = VVSIncidentsTests

DISTFILES += \
    testdata/incidents_ubahn.json

DEFINES += UNIT_TEST
