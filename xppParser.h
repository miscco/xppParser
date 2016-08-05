#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "xppParserException.h"

#include "aho_corasick/aho_corasick.hpp"
#include "mpParser.h"
#include "mpDefines.h"
#include "mpTest.h"


typedef struct {
	std::string					Name;
	std::string					Expr;
	std::vector<std::string>	Args;
} opts;

typedef std::vector<opts> optsArray;

class xppParser {
public:
	xppParser(std::string);

private:
	void expandArrays		(void);
	void expandArrayLines	(std::vector<std::string> &lines,
							 const std::vector<std::string> &expressions,
							 int idx);
	void extractDefinitions	(void);
	void extractGlobals		(void);
	void extractMarkov		(void);
	void extractTable		(void);
	void extractWiener		(void);
	void initializeTree		(void);
	void readFile			(void);
	void removeComments		(void);
	void removeWhitespace	(void);
	void splitLines			(void);

	/* Helper functions */
	void checkBrackets		(void);
	int  checkNames			(const std::string&);
	void findNextAssignment (const std::string&, size_t& , size_t&);
	std::vector<std::string> getList (const std::string&, std::string, std::string);
	std::string getNextExpr (const std::string&, size_t& ,size_t&);
	std::string getNextWord (const std::string&, size_t& ,size_t&);

	/* Filename of the ode file */
	std::string					fileName;

	/* List of the already used names */
	std::set<std::string>		usedNames;

	/* Vector containing the individual lines from the ode file */
	std::vector<std::string>	lines;

	/* Keywords utilized in the ode file */
	const std::vector<std::string> keywords = {
		"!",
		"(t+1)",
		"'",
		"/dt",
		"(t)",
		"volt",
		// "markov", /* Handled separately */
		"aux",
		"par",
		"number",
		"(",		/* Handled separately to discriminate odes and volterra */
		//"table",  /* This is handeled separately */
		//"wiener", /* Handled separately */
		//"global",
		"init",
		"(0)",
		"bdry",
		"0=",
		"solve",
		"special",
		"set",
		"@",
		"export"
	};

	/* The corresponding keyword tree */
	aho_corasick::trie keywordTree;

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
	optsArray Markovs;
	optsArray Numbers;
	optsArray Options;
	optsArray Parameters;
	optsArray Special;
	optsArray Sets;
	optsArray Tables;
	optsArray Volterra;
	opts	  Wieners;
};

#endif // PARSER_H
