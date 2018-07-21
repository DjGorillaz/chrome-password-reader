QT += core
QT -= gui
QT += sql

TARGET = chrome-password-reader
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    passReader.cpp

HEADERS += \
    passReader.h

LIBS += Crypt32.lib
