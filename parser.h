#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

typedef struct {
	std::string					Name;
	std::string					Value;
	std::vector<std::string>	Args;
} opts;

typedef std::vector<opts> optsArray;

class parser {
public:
	parser(std::string);

	/* Vector containing the individual lines from the ode file */
	std::vector<std::string> lines;
private:
	int parseFile			(void);
	void expandArrays		(void);
	void extractConstants	(void);
	void extractEquations	(void);
	void extractFunctions	(void);
	void extractInitConds	(void);
	void extractOptions		(void);
	void extractParameter	(void);
	void removeComments		(void);
	void removeSpaces		(void);
	void splitLines			(void);

	/* Filename of the ode file */
	std::string				 fileName;

	/* Keywords utilized in the ode file */
	std::vector<std::string> keywords;


	/* Options arrays */
	optsArray Constants;
	optsArray Equations;
	optsArray Functions;
	optsArray InitConds;
	optsArray Parameter;
	optsArray Settings;
};

#endif // PARSER_H
