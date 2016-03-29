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
	parser(std::string fn);

	/* Vector containing the individual lines from the ode file */
	std::vector<std::string> lines;
private:
	int parse_file(void);
	void extract_Constants(void);
	void extract_Equations(void);
	void extract_Functions(void);
	void extract_InitConds(void);
	void extract_Options(void);
	void extract_Parameter(void);
	void print_lines(void);
	void remove_comments(void);
	void remove_spaces(void);
	void split_lines(void);

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
