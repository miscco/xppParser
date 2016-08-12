#include "xppEvaluator.h"

xppEvaluator::xppEvaluator(xppParser &p)
	:parser(xppParser(p))
{
	/* The arrays containing the expressions that are replaced are at the
	 * beginning of the array. T
	 */
	std::vector<optsArray*> arrays = {
		&parser.Constants,
		&parser.Internal,
		&parser.Numbers,
		&parser.Functions,
		/* opts arrays that will only be searched */
		&parser.Algebraic,
		&parser.Auxiliar,
		&parser.Boundaries,
		&parser.Equations,
		&parser.Markovs,
		&parser.Special,
		&parser.Volterra
	};

	/* Loop over the first 4 arrays and replace their expressions in the
	 * following ones.
	 */
	for (unsigned i=0; i < 3; i++) {
		parseArrays(arrays, i);
	}
}

/**
 * @brief Create a keyword trie from an opts array
 *
 * @par expr: The opts array containing the expressions.
 */
aho_corasick::trie xppEvaluator::createTrie(const optsArray *expr) {
	aho_corasick::trie trie;

	trie.only_whole_words();
	trie.remove_overlaps();

	for (unsigned i=0; i < expr->size(); i++) {
		trie.insert(expr->at(i).Name);
	}

	return trie;
}

/**
 * @brief Replace expressions stored in an opts array in all other arrays
 *
 * @par expr: The opts array containing the expressions to be replaced
 * @par arrays: An array containing the pointers to the arrays to be searched
 * @par start: The first array in arrays that should be searched
 */
void xppEvaluator::parseArrays(std::vector<optsArray*> arrays,
							   unsigned start) {
	aho_corasick::trie trie = createTrie(arrays.at(start));

	for (unsigned i = start+1; i < arrays.size(); i++) {
		parseExpressions(trie, arrays.at(i));
	}
}

/**
 * @brief Utilize a trie search to replace expressions in an opts array
 *
 * @par trie: A trie containing the expressions to be replaced
 * @par target: The opts array that will be searched for possible replacements
 */
void xppEvaluator::parseExpressions(aho_corasick::trie &trie,
									optsArray *target) {
	for (unsigned i=0; i < target->size(); i++) {
		auto result = trie.parse_text(target->at(i).Expr);
		for (unsigned j=0; j < result.size(); j++) {
			target->at(i).Expr.replace(result.at(i).get_start(),
									   result.at(i).size(),
									   result.at(i).get_keyword());
		}
	}
}
