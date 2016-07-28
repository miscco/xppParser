#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>

//#include "exprtk/exprtk.hpp"

typedef struct {
	std::string					Name;
	std::string					Value;
	std::vector<std::string>	Args;
} opts;

typedef std::vector<opts> optsArray;

enum xppParserError {
	MISSING_BRACKET,
	MISSING_PARENTHESES,
	NO_NUMBER,
	UNKNOWN_ASSIGNMENT,
	WRONG_ARRAY_ASSIGNMENT,
	WRONG_MARKOV_ASSIGNMENT
};

class xppParser {
public:
	xppParser(std::string);

	/* Vector containing the individual lines from the ode file */
	std::vector<std::string> lines;
private:
	void expandArrays		(void);
	void expandArrayLines	(std::vector<std::string> &lines,
							 const std::vector<std::string> &expressions,
							 int idx);
	void extractDefinitions	(void);
	void extractMarkov		(void);
	void readFile			(void);
	void removeComments		(void);

	/* Filename of the ode file */
	std::string				 fileName;

	/* Error string */
	std::string				 errMsg;

	/* Keywords utilized in the ode file */
	std::vector<std::string> keywords;


	/* Options arrays */
	optsArray Constants;
	optsArray Equations;
	optsArray Functions;
	optsArray InitConds;
	optsArray Markov;
	optsArray Parameter;
	optsArray Settings;
};

class xppParserException : public std::exception {
private:
	std::string m_msg;
public:
	explicit xppParserException(const xppParserError msgType,
								const unsigned lineNumber,
								const size_t pos,
								const xppParser *Parser)
	{
		switch (msgType) {
		case MISSING_BRACKET:
			m_msg = std::string("Cannot find closing bracket ");
			break;
		case MISSING_PARENTHESES:
			m_msg = std::string("Cannot find closing parentheses ");
			break;
		case NO_NUMBER:
			m_msg = std::string("Cannot parse number ");
			break;
		case WRONG_ARRAY_ASSIGNMENT:
			m_msg = std::string("Wrong array assignment ");
			break;
		case WRONG_MARKOV_ASSIGNMENT:
			m_msg = std::string("Wrong assignment of markov process");
			break;
		case UNKNOWN_ASSIGNMENT:
			m_msg = std::string("Unknown assignment");
			break;
		}
		m_msg += "in line" + std::to_string(lineNumber+1) + ":\n";
		m_msg += Parser->lines[lineNumber] + "\n";
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

#endif // PARSER_H
