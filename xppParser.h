#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <algorithm>
#include <fstream>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "aho_corasick/aho_corasick.hpp"
#include "mpParser.h"
#include "mpDefines.h"

#include "xppParserDefines.h"
#include "xppParserException.h"

class xppParser {
public:
	xppParser(std::string fn);
	xppParser(const xppParser &parser);

private:
	void expandArrays		(void);
	void expandArrayLines	(std::vector<lineNumber> &lines,
							 const std::vector<lineNumber> &expressions,
							 int idx);
	void extractDefinition	(void);
	void extractExport		(void);
	void extractGlobal		(void);
	void extractMarkov		(void);
	void extractTable		(void);
	void extractWiener		(void);
	void initializeTree		(void);
	void readFile			(void);
	void removeComments		(void);
	void removeWhitespace	(void);
	void summarizeOde		(void);

	/* Helper functions */
	void checkBrackets		(void);
	void checkName			(const std::string &name, const lineNumber &line, size_t pos);
	void findNextAssignment (const lineNumber &line, size_t &pos1, size_t &pos2);
	stringList getList		(const std::string &line, unsigned ln,
							 std::string closure, std::string delim);
	std::string getNextExpr (const lineNumber &line, size_t &pos1,size_t &pos2);
	std::string getNextWord (const lineNumber &line, size_t &pos1,size_t &pos2);

	/* Filename of the ode file */
	const std::string		fileName;

	/* List of the already used names */
	std::set<std::string>	usedNames;

	/* Vector containing the individual lines from the ode file */
	std::vector<lineNumber>	lines;

	/* The corresponding keyword trie */
	aho_corasick::trie		keywordTrie;

	/* Options arrays */
	optsArray Algebraic;
	optsArray Auxiliar;
	optsArray Boundaries;
	optsArray Constants;
	optsArray Equations;
	optsArray Exports;
	optsArray Functions;
	optsArray Globals;
	optsArray InitConds;
	optsArray Temporaries;
	optsArray Markovs;
	optsArray Numbers;
	optsArray Options;
	optsArray Parameters;
	optsArray Special;
	optsArray Sets;
	optsArray Tables;
	optsArray Volterra;
	opts	  Wieners;

	friend class xppEvaluator;
	friend class xppValidator;
};

#endif // PARSER_H
