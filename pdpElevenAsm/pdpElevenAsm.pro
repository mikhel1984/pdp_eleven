TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "../pdpEleven/"

CONFIG(debug, debug|release) {
    LIBS += -L../build-pdpEleven/debug/ -lpdpEleven
}
else {
    LIBS += -L./build-pdpEleven/release/ -lpdpEleven
}

SOURCES += main.c

DISTFILES += \
    strcpylen.asm
