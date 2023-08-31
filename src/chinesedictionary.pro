# chinesedictionary.pro

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chinesedictionary
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += main.cpp \
    chinesedictionary.cpp \
    translator.cpp

HEADERS += chinesedictionary.h \
    translator.h

FORMS += chinesedictionary.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
