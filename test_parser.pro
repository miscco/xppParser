TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    parser.cpp

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    parser.h

