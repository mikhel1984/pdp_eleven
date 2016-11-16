
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE    = lib

VERSION     = 3.0.0

CONFIG      += qt warn_on release
CONFIG += staticlib

HEADERS     = \
    qhexedit.h \
    chunks.h \
    commands.h


SOURCES     = \
    qhexedit.cpp \
    chunks.cpp \
    commands.cpp

TARGET      = qhexedit

DESTDIR     = ../build-qhexedit/debug
