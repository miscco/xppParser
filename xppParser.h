#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>

#include "xppParserException.h"

//#include "exprtk/exprtk.hpp"

typedef struct {
	std::string					Name;
	std::string					Value;
	std::vector<std::string>	Args;
} opts;

typedef std::vector<opts> optsArray;

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

#endif // PARSER_H
