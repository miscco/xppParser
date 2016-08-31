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

#include "mpParser.h"
#include "mpDefines.h"

#include "keywordTrie.hpp"
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
							 const int idx);
	void extractDefinition	(void);
	void extractExport		(void);
	void extractGlobal		(void);
	void extractMarkov		(void);
	void extractTable		(void);
	void extractWiener		(void);
	void initializeTries	(void);
	void readFile			(void);
	void removeComments		(void);
	void removeWhitespace	(void);
	void summarizeOde		(void);

	/* Helper functions */
	void checkBrackets		(void);
	void checkName			(const std::string &name, const lineNumber &line, size_t pos);

	stringList getList		(const std::string &line, unsigned ln,
							 const std::string &closure, const std::string &delim);
	std::string getNextExpr (const lineNumber &line, size_t &pos1,size_t &pos2);
	std::string getNextWord (const lineNumber &line, size_t &pos1,size_t &pos2);

	bool isNumeric			(const std::string &str);

	keywordTrie::result keywordSearch(const std::string &key,
									  const char &character);

	/* Filename of the ode file */
	const std::string		fileName;

	/* Vector containing the individual lines from the ode file */
	std::vector<lineNumber>	lines;

	/* Trie of xpp keyword */
	keywordTrie::trie		keywords;

	/* Trie of xpp options */
	keywordTrie::trie		options;

	/* Trie of reserved names */
	keywordTrie::trie		reservedNames;

	/* Trie of the already used names */
	keywordTrie::trie		usedNames;

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
};

#endif // PARSER_H
