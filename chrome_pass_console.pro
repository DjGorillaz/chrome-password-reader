QT += core
QT -= gui
QT += sql

LIBS += Crypt32.lib


CONFIG += c++11

TARGET = chrome_pass_console
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
