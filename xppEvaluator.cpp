#include "xppEvaluator.h"

xppEvaluator::xppEvaluator(xppParser &p)
	:parser(xppParser(p))
{
	/* The arrays containing the expressions that are replaced are at the
	 * beginning of the array. T
	 */
	std::vector<optsArray> arrays = {
		parser.Constants,
		parser.Temporaries,
		parser.Numbers,
		/* opts arrays that will only be searched */
		parser.Functions,
		parser.Algebraic,
		parser.Auxiliar,
		parser.Boundaries,
		parser.Equations,
		parser.Markovs,
		parser.Special,
		parser.Volterra
	};

	/* Loop over the first 3 arrays and replace their expressions in the
	 * following ones.
	 */
	for (unsigned i=0; i < 2; i++) {
		aho_corasick::trie trie = createTrie(arrays.at(i));
		for (optsArray::size_type j= i+1; j < arrays.size(); j++) {
			replaceExpressions(trie, arrays.at(i), arrays.at(j));
		}
	}

	validateExpression(parser.Equations);
}

/**
 * @brief Create a keyword trie from an opts array
 *
 * @par expr: The opts array containing the expressions.
 */
aho_corasick::trie xppEvaluator::createTrie(const optsArray &array) {
	aho_corasick::trie trie;
	for (const opts &opt : array) {
		trie.insert(opt.Name);
	}
	trie.only_whole_words();
	trie.remove_overlaps();

	return trie;
}

/**
 * @brief Gets the next whitespace separated word after pos2
 *
 * @par line: String we are searching in
 * @par	pos1: Old position which will be udpated
 * @par pos2: Old position at which the search startes
 *
 * @return pos1: First position of the new word
 * @return pos2: Position of the first whitespace character after the word
 * @return string: String between [pos1, pos2-1]
 */
std::string xppEvaluator::getNextOperand(const std::string &expr,
										 size_t &pos1,
										 size_t &pos2) {

	pos1 = expr.find_first_not_of("+-*/^", pos2);
	pos2 = expr.find_first_of("+-*/^", pos1);
	return expr.substr(pos1, pos2-pos1);
}

/**
 * @brief Checks whether a provided string is a numeric expression
 *
 * @par str: The std::string containing the expression
 *
 * @return true if the string is a numeric expression, false otherwise
 */
bool xppEvaluator::isNumeric(const std::string &str) {
	char* p;
	std::strtod(str.c_str(), &p);
	return (*p == 0.0);
}

/**
 * @brief Utilize a trie search to replace expressions in an opts array
 *
 * @par trie: A trie containing the expressions to be replaced
 * @par target: The opts array that will be searched for possible replacements
 */
void xppEvaluator::replaceExpressions(aho_corasick::trie &trie,
									  const optsArray &source,
									  optsArray &target) {
	for (opts &opt : target) {
		auto result = trie.parse_text(opt.Expr);
		for (auto &res : result) {
			opt.Expr.replace(res.get_start(), res.size(),
							 source.at(res.get_index()).Expr);
		}
	}
}

void xppEvaluator::validateExpression(const optsArray &array) {
	for (const opts &opt : array) {
		/* Check the individual operands of the expression */
		size_t pos1 = 0, pos2 = 0;
		std::string temp = getNextOperand(opt.Expr, pos1, pos2);
		while (pos2 != std::string::npos) {
			if (parser.usedNames.find(temp) == parser.usedNames.end() &&
				!isNumeric(temp)) {
				throw xppParserException(UNKNOWN_NAME,
										 std::make_pair(opt.Expr, opt.Line),
										 pos1);
			}
			temp = getNextOperand(opt.Expr, pos1, pos2);
		}
	}
}
