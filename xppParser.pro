TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES +=	main.cpp \
			xppParser.cpp

HEADERS += xppParser.h

PRECOMPILED_HEADER = exprtk/exprtk.hpp

QMAKE_CXXFLAGS += -std=c++11

