#ifndef XPPEVALUATOR_H
#define XPPEVALUATOR_H

#include <algorithm>
#include <string>
#include <vector>

#include "aho_corasick/aho_corasick.hpp"

#include "xppParser.h"
#include "xppParserDefines.h"

class xppEvaluator
{
public:
	xppEvaluator(xppParser &p);

private:
	xppParser   parser;

	void replaceConstants			(std::vector<optsArray> &arrays);
	void replaceFunctions			(std::vector<optsArray> &arrays);

	/* Helper functions */
	aho_corasick::trie	createTrie			(const optsArray &array);
	functionTable		createFunctionTable	(const optsArray &array);
	bool				isNumeric			(const std::string &str);
	stringList			getFunctionArgs		(const opts &opt, size_t &start);
	std::string			getNextOperand		(const std::string &expr,
											 size_t &pos1,
											 size_t &pos2);
};

#endif // XPPEVALUATOR_H
