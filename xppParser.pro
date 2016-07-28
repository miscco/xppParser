TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES +=	main.cpp \
			xppParser.cpp

HEADERS += xppParser.h \
    xppParserException.h


PRECOMPILED_HEADER += exprtk/exprtk.hpp \
					  aho_corasick/aho_corasick.hpp

QMAKE_CXXFLAGS += -std=c++11

