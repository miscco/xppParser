#ifndef XPPVALIDATOR_H
#define XPPVALIDATOR_H

#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#include <xppParser.h>
#include <xppParserDefines.h>
#include <xppParserException.h>

class xppValidator
{
public:
	xppValidator(xppParser p);

private:
	void validateFunctions (void);

	std::string getNextOperand (const std::string &expr,
								size_t &pos1,
								size_t &pos2);

	bool isNumeric (const std::string &str);

	xppParser* parser;
};

#endif // XPPVALIDATOR_H
