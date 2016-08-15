#include "xppValidator.h"

xppValidator::xppValidator(xppParser p)
	: parser(&p)
{

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
std::string xppValidator::getNextOperand(const std::string &expr,
										 size_t &pos1,
										 size_t &pos2) {
	pos1 = expr.find_first_not_of("+-*/^", pos2);
	pos2 = expr.find_first_of("+-*/^", pos1);
	return expr.substr(pos1, pos2-pos1);
}

bool xppValidator::isNumeric(const std::string &str) {
	char* p;
	std::strtod(str.c_str(), &p);
	return (*p == 0);
}


void xppValidator::validateFunctions() {
	for (opts &opt : parser->Functions) {
		/* First check whether all arguments are used. Otherwise this is most
		 * likely a bug in the ode file.
		 */
		for (std::string &str : opt.Args) {
			if (opt.Expr.find(str) == std::string::npos) {
				throw xppParserException(MISSING_ARGUMENT,
										 std::make_pair(opt.Expr, opt.Line),
										 0);
			}
		}

		/* Check the individual operands of the expression */
		size_t pos1, pos2;
		std::string temp = getNextOperand(opt.Expr, pos1, pos2);
		while (pos2 != std::string::npos) {
			if (parser->usedNames.find(temp) == parser->usedNames.end() &&
					!isNumeric(temp)) {
				throw xppParserException(UNKNOWN_NAME,
										 std::make_pair(opt.Expr, opt.Line),
										 pos1);
			}

		}
	}
}
