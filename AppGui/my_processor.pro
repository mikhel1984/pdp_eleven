#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T11:47:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = my_processor
TEMPLATE = app


INCLUDEPATH += "../pdpEleven/"

CONFIG(debug, debug|release) {
    LIBS += -L../build-pdpEleven/debug/ -lpdpEleven
}
else {
    LIBS += -L./build-pdpEleven/release/ -lpdpEleven
}

SOURCES += main.cpp\
        processorwindow.cpp

HEADERS  += processorwindow.h

FORMS    += processorwindow.ui

DISTFILES +=
