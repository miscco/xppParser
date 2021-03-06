TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(parser/muparserx/muparserx.pri)

HEADERS +=	parser/keywordTrie.hpp \
		parser/xppEvaluator.h \
		parser/xppParser.h \
		parser/xppParserDefines.h \
		parser/xppParserException.h \
		settings/xppAutoSettings.h \
		settings/xppMainSettings.h \
		settings/xppSettings.h \
		settings/xppSolverSettings.h \
		xppColor.h \
		xppPlots.h

SOURCES +=	main.cpp \
		parser/xppEvaluator.cpp \
		parser/xppParser.cpp \
		settings/xppSettings.cpp

PRECOMPILED_HEADER +=

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

