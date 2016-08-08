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

#include "xppParserException.h"

#include "aho_corasick/aho_corasick.hpp"
#include "mpParser.h"
#include "mpDefines.h"

/* Basic structure that contains the textual information of an expression*/
typedef struct {
	std::string					Name;
	std::string					Expr;
	std::vector<std::string>	Args;
} opts;

/* Pair containing a parsed line and the original line number in the ode file.
 * This is mainly usefull for debugging if an error is thrown.
 */
typedef std::pair<std::string, int> lineNumber;

class xppParser {
public:
	xppParser(std::string);

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
	void splitLines			(void);

	/* Helper functions */
	void checkBrackets		(void);
	void checkName			(const std::string &name, const lineNumber &line, size_t pos);
	void findNextAssignment (const lineNumber &line, size_t &pos1, size_t &pos2);
	std::vector<std::string> getList (const std::string &line, int ln,
									  std::string closure, std::string delim);
	std::string getNextExpr (const lineNumber &line, size_t &pos1,size_t &pos2);
	std::string getNextWord (const lineNumber &line, size_t &pos1,size_t &pos2);

	/* Filename of the ode file */
	const std::string			fileName;

	/* List of the already used names */
	std::set<std::string>		usedNames;

	/* Vector containing the individual lines from the ode file */
	std::vector<lineNumber>		lines;

	/* The corresponding keyword tree */
	aho_corasick::trie keywordTree;

	/* Options arrays */
	std::vector<opts> Algebraic;
	std::vector<opts> Auxiliar;
	std::vector<opts> Boundaries;
	std::vector<opts> Constants;
	std::vector<opts> Equations;
	std::vector<opts> Exports;
	std::vector<opts> Functions;
	std::vector<opts> Globals;
	std::vector<opts> InitConds;
	std::vector<opts> Markovs;
	std::vector<opts> Numbers;
	std::vector<opts> Options;
	std::vector<opts> Parameters;
	std::vector<opts> Special;
	std::vector<opts> Sets;
	std::vector<opts> Tables;
	std::vector<opts> Volterra;
	opts	  Wieners;
};

#endif // PARSER_H
