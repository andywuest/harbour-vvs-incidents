QT += testlib network
QT -= gui

CONFIG += c++11 qt

SOURCES += testmain.cpp \
    backendstuttgarttests.cpp

HEADERS += \
    backendstuttgarttests.h

INCLUDEPATH += ../
include(../harbour-vvs-incidents.pri)

TARGET = BackendStuttgartTests

DISTFILES += \
    testdata/incidents_bus.json \
    testdata/indicents_ubahn.json

DEFINES += UNIT_TEST
