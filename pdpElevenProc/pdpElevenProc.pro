TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    processor.c

HEADERS += \
    opcodes.h \
    processor.h \
    test_program.h

