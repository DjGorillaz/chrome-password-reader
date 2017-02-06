QT += core
QT -= gui
QT += sql

CONFIG += c++11

TARGET = chromePassReader
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    passReader.cpp \
    PassReaderExample.cpp

HEADERS += \
    passReader.h

LIBS += Crypt32.lib
