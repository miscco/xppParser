#ifndef XPPPARSEREXCEPTION_H
#define XPPPARSEREXCEPTION_H

#include <exception>
#include <string>
#include <utility>

#include "xppParserDefines.h"

/* Different parser errors */
enum xppParserError {
	DUPLICATED_NAME,
	MISSING_CLOSING_BRACKET,
	MISSING_OPENING_BRACKET,
	EXPECTED_LIST_ARGUMENT,
	EXPECTED_NUMBER,
	EXPECTED_TABLE_ASSIGNMENT,
	RESERVED_FUNCTION,
	RESERVED_KEYWORD,
	UNKNOWN_ASSIGNMENT,
	WRONG_ARRAY_ASSIGNMENT,
	WRONG_MARKOV_ASSIGNMENT,
	WRONG_TABLE_ASSIGNMENT
};

/* Extension of std::exception to accomodate the parsers need */
class xppParserException : public std::exception {
private:
	std::string m_msg;
public:
	explicit xppParserException(const xppParserError msgType,
								const lineNumber line,
								const size_t pos)
	{
		switch (msgType) {
		case DUPLICATED_NAME:
			m_msg = std::string("Name has already been reserved");
			break;
		case MISSING_CLOSING_BRACKET:
			m_msg = std::string("Cannot find closing bracket");
			break;
		case MISSING_OPENING_BRACKET:
			m_msg = std::string("Cannot find opening bracket");
			break;
		case EXPECTED_LIST_ARGUMENT:
			m_msg = std::string("Missing argument in list");
			break;
		case EXPECTED_NUMBER:
			m_msg = std::string("Cannot parse number");
			break;
		case EXPECTED_TABLE_ASSIGNMENT:
			m_msg = std::string("Missing value for the lookup table");
			break;
		case RESERVED_FUNCTION:
			m_msg = std::string("Given function name is reserved");
			break;
		case RESERVED_KEYWORD:
			m_msg = std::string("Given name is reserved");
			break;
		case UNKNOWN_ASSIGNMENT:
			m_msg = std::string("Unknown assignment");
			break;
		case WRONG_ARRAY_ASSIGNMENT:
			m_msg = std::string("Wrong array assignment");
			break;
		case WRONG_MARKOV_ASSIGNMENT:
			m_msg = std::string("Wrong assignment of markov process");
			break;
		case WRONG_TABLE_ASSIGNMENT:
			m_msg = std::string("Wrong assignment of table expression");
			break;
		}
		m_msg += " in line " + std::to_string(line.second) + ":\n";
		m_msg += line.first + "\n";
		m_msg += std::string(pos, ' ') + "^\n";
	}

	virtual const char* what() const throw()
	{
		return m_msg.c_str();
	}
};

#endif // XPPPARSEREXCEPTION_H

