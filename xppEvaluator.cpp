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

	replaceConstants(arrays);
	replaceFunctions(arrays);
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
 * @brief Create a emit_collection for all custom functions of the ode file
 */
functionTable xppEvaluator::createFunctionTable(const optsArray &array) {
	functionTable table;
	for (const opts &opt : array) {
		aho_corasick::trie trie;
		for (const std::string &str : opt.Args) {
			trie.insert(str);
		}
		trie.only_whole_words();
		trie.remove_overlaps();

		auto result = trie.parse_text(opt.Expr);
		/* Reverse the order of the matches so the indices do not change
		 * when expression is replaced
		 */
		std::reverse(result.begin(), result.end());
		table.push_back(result);
	}
	return table;
}

/**
 * @brief Gets arguments of a function in a arbitrary expression
 *
 * @par opt: The opts structure containing the expression
 * @par	start: The start of the brace-enclosed argument list
 *
 * @return A vector of the respective argument strings
 */
stringList xppEvaluator::getFunctionArgs (const opts &opt,
										  size_t &start) {
	size_t end = opt.Expr.find(")", start);
	size_t pos1 = start;
	size_t pos2 = opt.Expr.find_first_of(",)", pos1);
	stringList args = {opt.Expr.substr(pos1, pos2-pos1)};
	if (parser.usedNames.find(args.back()) == parser.usedNames.end()) {
		throw xppParserException(UNKNOWN_NAME,
								 std::make_pair(opt.Expr, opt.Line), pos1);
	}
	while (pos2 < end) {
		pos1 = pos2+1;
		pos2 = opt.Expr.find_first_of(",)", pos1);
		args.push_back(opt.Expr.substr(pos1, pos2-pos1));
		if (parser.usedNames.find(args.back()) == parser.usedNames.end()) {
			throw xppParserException(UNKNOWN_NAME,
									 std::make_pair(opt.Expr, opt.Line), pos1);
		}
	}
	start = end+2-start;
	return args;
}

/**
 * @brief Gets the next operand in a mathematical expression
 *
 * @par expr: The expression we are parsing
 * @par	pos1: Old position which will be udpated
 * @par pos2: Old position at which the search startes
 *
 * @return pos1: First position of the operand
 * @return pos2: Position of the mathematical operator after the operand
 * @return string: String between [pos1, pos2-1]
 */
std::string xppEvaluator::getNextOperand(const std::string &expr,
										 size_t &pos1,
										 size_t &pos2) {
	pos1 = expr.find_first_not_of("+-*/^(", pos2);
	pos2 = expr.find_first_of("+-*/^(", pos1);
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
 * @brief Utilize a trie search to replace constant expressions in an opts array
 *
 * @par arrays: An array of optsArrays containing the parsed expressions.
 */
void xppEvaluator::replaceConstants(std::vector<optsArray> &arrays) {
	for (size_t i=0; i < 2; i++) {
		aho_corasick::trie trie = createTrie(arrays.at(i));
		for (size_t j= i+1; j < arrays.size(); j++) {
			for (opts &opt : arrays.at(j)) {
				auto result = trie.parse_text(opt.Expr);
				/* Reverse the order of the matches so the indices do not change
				 * when expression is replaced
				 */
				std::reverse(result.begin(), result.end());
				for (auto &res : result) {
					opt.Expr.replace(res.get_start(), res.size(),
									 arrays.at(i).at(res.get_index()).Expr);
				}
			}
		}
	}
}

/**
 * @brief Utilize a trie search to replace function expressions in an opts array
 *
 * @par arrays: An array of optsArrays containing the parsed expressions.
 */
void xppEvaluator::replaceFunctions(std::vector<optsArray> &arrays) {
	aho_corasick::trie trie = createTrie(parser.Functions);
	functionTable funTable  = createFunctionTable(parser.Functions);

	for (optsArray::size_type j= 4; j < arrays.size(); j++) {
		for (opts &opt : arrays.at(j)) {
			auto result = trie.parse_text(opt.Expr);
			/* Reverse the order of the matches so the indices do not change
			 * when expression is replaced
			 */
			std::reverse(result.begin(), result.end());
			for (auto &res : result) {
				size_t pos = res.get_end()+2;
				stringList args = getFunctionArgs(opt, pos);
				std::string temp = parser.Functions.at(res.get_index()).Expr;
				for (auto &res2 : funTable.at(res.get_index())) {
					temp.replace(res2.get_start(), res2.size(),
								 args.at(res2.get_index()));
				}
				opt.Expr.replace(res.get_start(), res.size()+pos, temp);
			}
		}
	}
}
