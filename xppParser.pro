TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


include(muparserx/muparserx.pri)

SOURCES +=	main.cpp \
			xppParser.cpp \
    xppEvaluator.cpp \
    keywordTrie.cpp

HEADERS +=	xppParser.h \
			xppParserException.h \
    xppParserDefines.h \
    xppEvaluator.h \
    keywordTrie.h

PRECOMPILED_HEADER += aho_corasick/aho_corasick.hpp

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

