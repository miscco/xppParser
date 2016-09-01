TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


include(muparserx/muparserx.pri)

SOURCES +=	main.cpp \
			parser/xppEvaluator.cpp \
			parser/xppParser.cpp

HEADERS +=	parser/keywordTrie.hpp \
			parser/xppEvaluator.h \
			parser/xppParser.h \
			parser/xppParserDefines.h \
			parser/xppParserException.h

INCLUDEPATH += parser

PRECOMPILED_HEADER +=

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

