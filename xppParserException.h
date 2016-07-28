#ifndef XPPPARSEREXCEPTION_H
#define XPPPARSEREXCEPTION_H

#include <exception>
#include <string>
#include <vector>

/* Different parser errors */
enum xppParserError {
	MISSING_BRACKET,
	MISSING_PARENTHESES,
	NO_NUMBER,
	UNKNOWN_ASSIGNMENT,
	WRONG_ARRAY_ASSIGNMENT,
	WRONG_MARKOV_ASSIGNMENT
};

/* Extension of std::exception to accomodate the parsers need */
class xppParserException : public std::exception {
private:
	std::string m_msg;
public:
	explicit xppParserException(const xppParserError msgType,
								const std::string& line,
								const unsigned lineNumber,
								const size_t pos)
	{
		switch (msgType) {
		case MISSING_BRACKET:
			m_msg = std::string("Cannot find closing bracket");
			break;
		case MISSING_PARENTHESES:
			m_msg = std::string("Cannot find closing parentheses");
			break;
		case NO_NUMBER:
			m_msg = std::string("Cannot parse number");
			break;
		case WRONG_ARRAY_ASSIGNMENT:
			m_msg = std::string("Wrong array assignment");
			break;
		case WRONG_MARKOV_ASSIGNMENT:
			m_msg = std::string("Wrong assignment of markov process");
			break;
		case UNKNOWN_ASSIGNMENT:
			m_msg = std::string("Unknown assignment");
			break;
		}
		m_msg += " in line " + std::to_string(lineNumber+1) + ":\n";
		m_msg += line + "\n";
		if (pos > 0) {
			m_msg += std::string(pos-1, ' ') + "^^^\n";
		} else {
			m_msg += "^^^\n";
		}
	}

	virtual const char* what() const throw()
	{
		return m_msg.c_str();
	}
};

#endif // XPPPARSEREXCEPTION_H

