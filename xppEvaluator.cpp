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
		parser.Functions,
		/* opts arrays that will only be searched */
		parser.Algebraic,
		parser.Auxiliar,
		parser.Boundaries,
		parser.Equations,
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
keywordTrie::trie xppEvaluator::createTrie(const optsArray &array) {
	keywordTrie::trie trie;
	for (const opts &opt : array) {
		trie.addString(opt.Name);
	}

	return trie;
}

/**
 * @brief Create a emit_collection for all custom functions of the ode file
 *
 * For functions we have to know the position of all the function arguments in
 * the function expression, as the arguments may change every time the function
 * is invoked.
 */
xppEvaluator::resultTable xppEvaluator::createFunctionTable(const optsArray &array) {
	resultTable table;
	for (const opts &opt : array) {
		keywordTrie::trie trie;
		trie.addString(opt.Args);
		auto result = trie.parseText(opt.Expr);
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
stringList xppEvaluator::getFunctionArgs (const std::string &str,
										  const size_t &ln,
										  size_t &start) {
	size_t end = str.find(")", start);
	size_t pos1 = start;
	size_t pos2 = str.find_first_of(",)", pos1);
	stringList args = {str.substr(pos1, pos2-pos1)};
	if (parser.usedNames.parseText(args.back()).empty()) {
		throw xppParserException(UNKNOWN_NAME, std::make_pair(str, ln), pos1);
	}
	while (pos2 < end) {
		pos1 = pos2+1;
		pos2 = str.find_first_of(",)", pos1);
		args.push_back(str.substr(pos1, pos2-pos1));
		if (parser.usedNames.parseText(args.back()).empty()) {
			throw xppParserException(UNKNOWN_NAME, std::make_pair(str, ln), pos1);
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
 * @brief Utilize a trie search to replace constant expressions in an opts array
 *
 * @par arrays: An array of optsArrays containing the parsed expressions.
 */
void xppEvaluator::replaceConstants(std::vector<optsArray> &arrays) {
	for (size_t i=0; i < 2; i++) {
		keywordTrie::trie trie = createTrie(arrays.at(i));
		for (size_t j= i+1; j < arrays.size(); j++) {
			for (opts &opt : arrays.at(j)) {
				replaceExpression(trie, arrays.at(i), opt.Expr);
			}
		}

		/* Handle markov processes separately as the transition probabilities
		 * are stored in the args vector rather than the expression.
		 */
		for (opts &opt : parser.Markovs) {
			for (std::string &str : opt.Args) {
				replaceExpression(trie, arrays.at(i), str);
			}
		}
	}
}

/**
 * @brief Utilize a trie search to replace constant expressions in a string
 *
 * @par trie: The keywordTrie trie with the keywords.
 * @par source: The array containing the replacements.
 * @par str: The string that should be searched.
 */
void xppEvaluator::replaceExpression(keywordTrie::trie &trie,
									 const optsArray &source,
									 std::string &expr) {
	auto result = trie.parseText(expr);
	/* Reverse the order of the matches so the indices do not change
	 * when expression is replaced
	 */
	std::reverse(result.begin(), result.end());
	for (auto &res : result) {
		expr.replace(res.start, res.keyword.size(), source.at(res.id).Expr);
	}
}

/**
 * @brief Utilize a trie search to replace function expressions in a string
 *
 * @par trie: The keywordTrie trie with the keywords.
 * @par funcTable: The emit collection of the functions array.
 * @par str: The string that should be searched.
 * @par ln: The line number for error throws.
 */
void xppEvaluator::replaceFunExpression(keywordTrie::trie &trie,
										const resultTable &funTable,
										std::string &expr,
										const size_t &ln) {
	auto result = trie.parseText(expr);
	/* Reverse the order of the matches so the indices do not change
	 * when expression is replaced
	 */
	std::reverse(result.begin(), result.end());
	for (auto &res : result) {
		size_t pos = res.end+2;
		stringList args = getFunctionArgs(expr, ln, pos);
		std::string temp = parser.Functions.at(res.id).Expr;
		for (auto &res2 : funTable.at(res.id)) {
			temp.replace(res2.start, res2.keyword.size(),
						 args.at(res2.id));
		}
		expr.replace(res.start, res.keyword.size()+pos, temp);
	}
}

/**
 * @brief Utilize a trie search to replace function expressions in an opts array
 *
 * @par arrays: An array of optsArrays containing the parsed expressions.
 */
void xppEvaluator::replaceFunctions(std::vector<optsArray> &arrays) {
	keywordTrie::trie trie = createTrie(parser.Functions);
	resultTable funTable  = createFunctionTable(parser.Functions);

	for (optsArray::size_type j= 4; j < arrays.size(); j++) {
		for (opts &opt : arrays.at(j)) {
			replaceFunExpression(trie, funTable, opt.Expr, opt.Line);
		}
	}

	/* Handle markov processes separately as the transition probabilities
	 * are stored in the args vector rather than the expression.
	 */
	for (opts &opt : parser.Markovs) {
		for (std::string &str : opt.Args) {
			replaceFunExpression(trie, funTable, str, opt.Line);
		}
	}
}
