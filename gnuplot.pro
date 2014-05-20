TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    eggplot.cpp \
    linespec.cpp
HEADERS += \
    eggplot.h \
    linespec.h \
    common.h

CONFIG += c++11
