#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <stack>
#include <string>
#include <vector>

#include "xppParserException.h"

#include "aho_corasick/aho_corasick.hpp"
//#include "exprtk/exprtk.hpp"

typedef struct {
	std::string					Name;
	std::string					Value;
	std::vector<std::string>	Args;
} opts;

typedef std::vector<opts> optsArray;

class xppParser {
public:
	xppParser(std::string);

	/* Vector containing the individual lines from the ode file */
	std::vector<std::string> lines;
private:
	void checkBrackets		(void);
	void expandArrays		(void);
	void expandArrayLines	(std::vector<std::string> &lines,
							 const std::vector<std::string> &expressions,
							 int idx);
	void extractDefinitions	(void);
	void extractMarkov		(void);
	int  getKeywordCode		(std::string key);
	void initializeTree		(void);
	void readFile			(void);
	void removeComments		(void);
	void removeWhitespace	(void);

	/* Filename of the ode file */
	std::string				 fileName;

	/* Keywords utilized in the ode file */
	const std::vector<std::string> keywords = {
		"!",
		"(t+1)",
		"'",
		"/dt",
		"(t)",
		"volterra",
		// "markov", /* This is handeled separately */
		"aux",
		"par",
		"number",
		//"(",		/* Handle separately to discriminate odes and volterra */
		// "table", /* This is handeled separately */
		"wiener",
		"global",
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
	optsArray Constants;
	optsArray Equations;
	optsArray Functions;
	optsArray InitConds;
	optsArray Markov;
	optsArray Parameter;
	optsArray Settings;
};

#endif // PARSER_H
