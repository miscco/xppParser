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

	aho_corasick::trie createTrie	(const optsArray *expr);
	void parseArrays				(std::vector<optsArray*> arrays,
									 unsigned start);
	void parseExpressions			(aho_corasick::trie &trie,
									 optsArray *target);
};

#endif // XPPEVALUATOR_H
