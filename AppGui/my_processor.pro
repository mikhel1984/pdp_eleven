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
INCLUDEPATH += "../qhexedit/"

CONFIG(debug, debug|release) {
    LIBS += -L../build-pdpEleven/debug/ -lpdpEleven
    LIBS += -L../build-qhexedit/debug/ -lqhexedit
}
else {
    LIBS += -L../build-pdpEleven/release/ -lpdpEleven
    LIBS += -L../build-qhexedit/release/ -lqhexedit
}

SOURCES += main.cpp\
        processorwindow.cpp

HEADERS  += processorwindow.h

FORMS    += processorwindow.ui

DISTFILES +=
