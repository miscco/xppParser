#ifndef XPPEVALUATOR_H
#define XPPEVALUATOR_H

#include <algorithm>
#include <string>
#include <vector>

#include "keywordTrie.hpp"

#include "xppParser.h"
#include "xppParserDefines.h"

class xppEvaluator
{
public:
	typedef std::vector<keywordTrie::result> resultCollection;
	typedef std::vector<resultCollection>	 resultTable;

	xppEvaluator(xppParser &p);

private:
	xppParser   parser;

	void replaceConstants			(std::vector<optsArray> &arrays);
	void replaceFunctions			(std::vector<optsArray> &arrays);

	/* Helper functions */
	keywordTrie::trie	createTrie		(const optsArray &array);
	resultTable		createResultTable	(const optsArray &array);
	bool			isNumeric			(const std::string &str);
	stringList		getFunctionArgs		(const std::string &str,
										 const size_t &ln,
										 size_t &start);
	std::string		getNextOperand		(const std::string &expr,
										 size_t &pos1,
										 size_t &pos2);
	void			replaceExpression	(keywordTrie::trie &trie,
										 const optsArray &source,
										 std::string &expr);
	void			replaceFunExpression(keywordTrie::trie &trie,
										 const resultTable &funTable,
										 std::string &expr,
										 const size_t &ln);
};

#endif // XPPEVALUATOR_H
