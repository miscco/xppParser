#ifndef XPPEVALUATOR_H
#define XPPEVALUATOR_H

#include "aho_corasick/aho_corasick.hpp"

#include "xppParser.h"

class xppEvaluator
{
public:
	xppEvaluator(xppParser &p);

private:
	xppParser   parser;

	aho_corasick::trie createTrie	(const optsArray &array);
	void replaceExpressions			(aho_corasick::trie &trie,
									 const optsArray &source,
									 optsArray &target);
	void validateExpression			(const optsArray &array);

	/* helper functions */
	bool isNumeric					(const std::string &str);
	std::string getNextOperand		(const std::string &expr,
									 size_t &pos1,
									 size_t &pos2);
};

#endif // XPPEVALUATOR_H
