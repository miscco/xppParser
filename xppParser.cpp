#include <xppParser.h>

/**
 * @brief Constructor of the parser object
 *
 * @param fn string representing the file name of the ode file
 *
 * This constructs the parser object of a given ode file. First unneeded
 * content is discarded, and a basic correctness check is don. Later arrays are
 * expanded and special constructs like markov processes and tables are handled.
 * Finally the different keywords are parsed and put into the opts arrays.
 */
xppParser::xppParser(std::string fn)
	: fileName(fn)
{
	try {
		/* Initially read in the ode file */
		readFile();

		/* Debug output */
		for(unsigned i = 0; i < lines.size(); ++i) {
			//std::cout << lines[i] << "!" << std::endl;
		}

		/* Initialize the keyword tree for command paring */
		initializeTree();

		/* Check for incorrect brackets */
		checkBrackets();

		/* Remove all comments */
		removeComments();

		/* Remove unnecessary whitespaces */
		removeWhitespace();

		/* Expand array descriptions */
		expandArrays();

		/* Extract markov processes */
		extractMarkov();

		/* Extract wiener processes */
		extractWiener();

		/* Extract all other definitions */
		extractDefinitions();

		/* Catch errors */
	} catch (xppParserException& e) {
		std::cerr << e.what();
	} catch (std::runtime_error& e) {
		std::cerr << e.what();
	} catch (std::exception& e) {
		std::cerr << e.what();
	} catch (...) {
		std::cerr << "Unexpected error\n";
		throw;
	}
}

/**
 * @brief Checks whether brackets are closed properly
 */
void xppParser::checkBrackets() {
	using charPos = std::pair<char, size_t>;
	std::stack<charPos> brackets;
	for (unsigned i=0; i < lines.size(); i++) {
		/* Create a stack containing the individual brackets and their position
		 */
		for (unsigned j=0; j < lines[i].size(); j++) {
			switch (lines[i].at(j)) {
			case '(':
				brackets.push(charPos(')', j));
				break;
			case '{':
				brackets.push(charPos('}', j));
				break;
			case '[':
				brackets.push(charPos(']', j));
				break;
			case ')':
				if (brackets.empty()) {
					throw xppParserException(MISSING_OPENING_BRACKET, lines[i],
											 i, j);
				} else if (brackets.top().first != ')') {
					throw xppParserException(MISSING_CLOSING_BRACKET, lines[i],
											 i, brackets.top().second);
				}
				brackets.pop();
				break;
			case '}':
				if (brackets.empty()) {
					throw xppParserException(MISSING_OPENING_BRACKET, lines[i],
											 i, j);
				} else if (brackets.top().first != '}') {
					throw xppParserException(MISSING_CLOSING_BRACKET, lines[i],
											 i, brackets.top().second);
				}
				brackets.pop();
				break;
			case ']':
				if (brackets.empty()) {
					throw xppParserException(MISSING_OPENING_BRACKET, lines[i],
											 i, j);
				} else if (brackets.top().first != ']') {
					throw xppParserException(MISSING_CLOSING_BRACKET, lines[i],
											 i, brackets.top().second);
				}
				brackets.pop();
				break;
			default:
				continue;
			}
		}
		if (!brackets.empty()) {
			throw xppParserException(MISSING_CLOSING_BRACKET, lines[i],
									 i, brackets.top().second);
		}
	}
}

/**
 * @brief Expand array expressions in ode files
 *
 * This function searches for the array expressions found in ode files, that are
 * marked by "x[start .. end]" for single line or "%[start .. end] ... %" for
 * multi line statements. Every array line is duplicated end-start+1 times,
 * while preserving the original order of lines.
 */
void xppParser::expandArrays() {
	unsigned i=0;
	while(i < lines.size()) {
		std::size_t pos1, pos2, pos3;
		/* Search for opening brackets */
		pos1 = lines[i].find("[");

		if (pos1 != std::string::npos) {
			/* Search for the closing bracket and the dots between the start and
			 * the end indices. The dots are only necessary for single line
			 * statements.
			 */
			pos2 = lines[i].find("..");
			pos3 = lines[i].find("]");
			if (pos3 == std::string::npos) {
				throw xppParserException(MISSING_CLOSING_BRACKET, lines[i], i, pos1);
			} else if (pos2 != std::string::npos && pos2 > pos3) {
				throw xppParserException(WRONG_ARRAY_ASSIGNMENT, lines[i], i, pos2);
			}

			/* Determine the range of the indices of the array */
			int start, end;
			try {
				start = std::stoi(lines[i].substr(pos1+1, pos2-pos1-1));
			} catch (std::invalid_argument) {
				throw xppParserException(EXPECTED_NUMBER, lines[i], i, pos1+1);
			}
			try {
				end = std::stoi(lines[i].substr(pos2+2, pos3-pos2-2));
			} catch (std::invalid_argument) {
				throw xppParserException(EXPECTED_NUMBER, lines[i], i, pos2+2);
			}

			/* Copy the lines of the assignment into a separate vector */
			std::vector<std::string> arrayExpressions;
			int numLines = 1;
			if (lines[i].compare(pos1-1, 1, "%") == 0) {
				/* Multiline statements end with a line with a single "%" */
				while (true) {
					size_t endArray = lines[i+numLines].find("%");
					if (endArray != std::string::npos) {
						numLines++;
						break;
					}
					arrayExpressions.push_back(lines[i+numLines]);
					numLines++;
				}
			} else {
				arrayExpressions.push_back(lines[i]);
				/* Change the first bracket to [j] to unify expression handling
				 * with the multiline case.
				 */
				arrayExpressions[0].replace(pos1, pos3, "[j]");
			}

			/* Remove the old lines */
			lines.erase(lines.begin()+i, lines.begin()+i+numLines);

			/* Expand the lines */
			std::vector<std::string> arrayLines;
			for (int i = start; i <= end; i++) {
				expandArrayLines(arrayLines, arrayExpressions, i);
			}
			/* Insert the new lines back into the old position */
			lines.insert(lines.begin()+i, arrayLines.begin(), arrayLines.end());
		} else {
			i++;
		}
	}
}

/**
 * @brief Inserts a given array expression into the ode list for given j
 *
 * @param lines A vector that is to be filled with the resulting lines.
 * @param expressions A vector containing the individual lines of array
 *		  expressions.
 * @param idx The index that should be used for evaluation of the expressions.
 *
 * This function takes the array expressions and replaces every assignment of
 * type [...] with the evaluated expression for j=idx. Afterwards the resulting
 * line is inserted into the provided vector.
 */
void xppParser::expandArrayLines(std::vector<std::string>& lines,
								 const std::vector<std::string>& expressions,
								 int idx) {
	std::string temp;
	size_t pos1, pos2;
	int res;

	/* Create the parser objects */
	/* XXX: exprtk only knows double long double and float */
	/*
	typedef exprtk::symbol_table<double> symbol_table_t;
	typedef exprtk::expression<double>     expression_t;
	typedef exprtk::parser<double>             parser_t;

	double j = idx;
	symbol_table_t symbol_table;
	symbol_table.add_variable("j", j);
	symbol_table.add_constants();

	expression_t expression;
	expression.register_symbol_table(symbol_table);

	parser_t parser;
	*/
	for (unsigned i=0; i < expressions.size(); i++) {
		temp = expressions[i];
		pos1 = temp.find("[");
		pos2 = temp.find("]");
		while (pos1 != std::string::npos) {
			/* Evaluate the expression within the brackets and replace the
			 * brackets with the result.
			 */
			//parser.compile(temp.substr(pos1+1, pos2-pos1-1),expression);
			//res = (int) expression.value();
			res = idx;
			temp.replace(pos1, pos2-pos1+1, std::to_string(res));

			/* Search for other expressions. */
			pos1 = temp.find("[", pos2);
			pos2 = temp.find("]", pos1);
		}
		lines.push_back(temp);
	}
}

/**
 * @brief Extract definitions within the ode file
 *
 * This extracts definitions that are given in the keyword list and are marked
 * by an equal sign.
 */
void xppParser::extractDefinitions(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::string  keyword;
		/* Any definition is marked by the equal sign */
		std::size_t pos1 = lines[i].find("=");
		std::size_t pos2;

		/* If this function is reached every other valid statement should be
		 * handeled, so bail out if we do not find an assignment.
		 */
		if (pos1 == std::string::npos) {
			pos2 = lines[i].find("done");
			if (pos2 == std::string::npos) {
				throw xppParserException(UNKNOWN_ASSIGNMENT, lines[i], i, 0);
			} else {
				lines.erase(lines.begin()+i);
			}
		}

		/* Extract the keyword. In most cases it should be the first consequtive
		 * string that precedes a whitespace and is followed by the name of the
		 * variable.
		 */
		while (pos1 != std::string::npos) {
			pos2 = lines[i].find_first_of(" \t\f\v\r");
			keyword = lines[i].substr(0, pos2);
			auto result = keywordTree.parse_text(keyword);
			/* A keyword was found  extract its name and values */
			if (result.size() == 1) {
				std::string optName;
				if (result.at(0).get_keyword() == keywords[0]) {
					/* !Name */
					optName = lines[i].substr(1, pos1-1);
				} else if (result.at(0).get_keyword() == keywords[1] ||
						   result.at(0).get_keyword() == keywords[2]) {
					/* Name(t+1) or Name' */
					optName = lines[i].substr(0, result.at(0).get_start());

				} else if (result.at(0).get_keyword() == keywords[3]) {
					/* dName/dt */
					optName = lines[i].substr(1, result.at(0).get_start()-1);

				} else if (result.at(0).get_keyword() == keywords[4]) {
					/* Name(t) */
					optName = lines[i].substr(0, result.at(0).get_start());

				} else if (result.at(0).get_keyword() == keywords[5]) {
					/* volterra Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[6]) {
					/* aux Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[7]) {
					/* par Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[8]) {
					/* number Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[9]) {
					/* wiener Name1 Name2 ...*/
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[10]) {
					/* global Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[11]) {
					/* init Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[12]) {
					/* Name(0) */
					optName = lines[i].substr(0, result.at(0).get_start());

				} else if (result.at(0).get_keyword() == keywords[13]) {
					/* bdry Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[14]) {
					/* 0= Expression */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[15]) {
					/* solve Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[16]) {
					/* special Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[17]) {
					/* set Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);

				} else if (result.at(0).get_keyword() == keywords[18]) {
					/* @ Name */
					optName = lines[i].substr(pos2+1, pos1-pos2-1);
				}
			} else {
				/* This can only be a function or expression definition that may
				 * contain parts of keywords.
				 */
			}
			pos1 = lines[i].find("=", pos1+1);
		}
		i++;
	}
}

/**
 * @brief Extract a markov process from the parsed lines
 *
 * This function extracts the definition of markove processes as they constitute
 * one of the few multiline statements in an ode file.
 */
void xppParser::extractMarkov(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::size_t pos1 = lines[i].find("markov");
		std::size_t pos2;
		if (pos1 != std::string::npos) {
			/* Parse the name */
			opts opt;
			int nstates;

			pos1 = lines[i].find_first_not_of(" ", pos1+6);
			pos2 = lines[i].find_first_of(" ", pos1);
			opt.Name = lines[i].substr(pos1, pos2-pos1);

			/* Parse the number of states */
			pos1 = lines[i].find_first_not_of(" ", pos2);
			pos2 = lines[i].find_first_of(" ", pos1);
			if (pos2 == std::string::npos) {
				try {
					nstates = std::stoi(lines[i].substr(pos1, lines[i].back()));
				} catch (std::invalid_argument) {
					throw xppParserException(EXPECTED_NUMBER, lines[i], i, pos1);
				}
			} else {
				try {
					nstates = std::stoi(lines[i].substr(pos1, pos2-pos1));
				} catch (std::invalid_argument) {
					throw xppParserException(EXPECTED_NUMBER, lines[i], i, pos1);
				}
			}
			/* Store the number of states for convenience */
			opt.Value = nstates;

			/* Parse the transition probabilities */
			for (int j=0; j < nstates; j++) {
				pos2 = 0;
				for (int k=0; k < nstates; k++) {
					pos1 = lines[i+1].find("{", pos2);
					pos2 = lines[i+1].find("}", pos1);
					if (pos1 == std::string::npos) {
						throw xppParserException(WRONG_MARKOV_ASSIGNMENT,
												 lines[i+1], i+1, pos1);
					}
					opt.Args.push_back(lines[i+j].substr(pos1+1, pos2-pos1-1));
				}
				lines.erase(lines.begin() + i + 1);
			}
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

/**
 * @brief Extracts wiener processes
 *
 * This function extracts the definition of wiener processes as their definition
 * does not contain an equal sign but only the names of the individual proceses.
 */
void xppParser::extractWiener(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::size_t pos1 = lines[i].find("wiener");
		std::size_t pos2 = lines[i].find_first_of(" ", pos1);
		if (pos1 != std::string::npos) {
			pos1 = lines[i].find_first_not_of(" ", pos2);
			pos2 = lines[i].find_first_of(" ", pos1);
			while (pos1 != std::string::npos) {
				Wiener.Args.push_back(lines[i].substr(pos1, pos2-pos1));
				pos1 = lines[i].find_first_not_of(" ", pos2);
				pos2 = lines[i].find_first_of(" ", pos1);
				std::cout << Wiener.Args.back() << std::endl;
			}
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}

}

/**
 * @brief Initializes the keyword tree from the keyword list
 */
void xppParser::initializeTree (void) {
	for (size_t i=0; i < keywords.size(); i++) {
		keywordTree.insert(keywords.at(i));
	}
}

/**
 * @brief Reads in the ode file and stores the lines in a vector
 *
 * This function reads in the ode file given by fileName, ignoring empty lines
 * as well lines containing only whitespace. Furthermore it trims and truncates
 * all whitespaces.
 */
void xppParser::readFile(void) {
	/* Open file for parsing */
	std::ifstream fileStream(fileName.c_str(), std::ios::in);
	if (fileStream.fail()) {
		throw std::runtime_error("Cannot open ode file " + fileName + "\n");
	}

	/* Parse in nonempty lines */
	std::string temp;
	while(getline(fileStream, temp)) {
		size_t pos1 = temp.find_first_not_of(" \t\f\v\r\n");
		if (temp.length() != 0 && pos1 != std::string::npos) {
			/* Remove trailing and superflous whitespaces */
			temp.erase(0, pos1);
			temp.resize(temp.find_last_not_of(" \t\f\v\r\n")+1);
			auto last = std::unique(temp.begin(), temp.end(),
						[](char l, char r){return std::isspace(l) &&
												  std::isspace(r);});
			temp.erase(last, temp.end());
			lines.push_back(temp);
		}
	}

	/* Check whether some lines have been parsed */
	if (lines.size() == 0) {
		throw std::runtime_error("Empty ode file " + fileName + "\n");
	}
}

/**
 * @brief Removes comments from ode files
 *
 * This function purges all standard comments from a ode file. If a line would
 * be comment only it is deleted.
 */
void xppParser::removeComments() {
	unsigned i = 0;
	while(i < lines.size()) {
		std::size_t pos1 = lines[i].find("#");
		if (pos1 == 0) {
			lines.erase(lines.begin() + i);
		} else if (pos1 != std::string::npos) {
			/* Check if this is the definition of a convolutional integral
			 * indicated by curly braces sourrunding the # sign
			 */
			std::size_t subpos1 = lines[i].find_last_of("{", pos1);
			std::size_t subpos2 = lines[i].find("}", subpos1);
			if (subpos1 == std::string::npos || subpos2 < pos1) {
				lines[i].resize(pos1);
			}
			i++;
		} else {
			i++;
		}
	}
}

/**
 * @brief Removes unneeded whitespaces
 *
 * This function searches for whitespaces within parentheses or before/after an
 * equal sign and removes those.
 */
void xppParser::removeWhitespace() {
	unsigned i=0;
	while(i < lines.size()) {
		std::size_t pos1, pos2, pos3;
		/* Search for opening brackets */
		pos1 = lines[i].find("(");
		if (pos1 != std::string::npos) {
			/* Search for the closing bracket remove all whitespaces between */
			pos2 = lines[i].find(")");
			pos3 = lines[i].find_first_of(" \t\f\v\r", pos1);
			while (pos3 < pos2) {
				lines[i].erase(pos3, pos2-pos3);
				pos3 = lines[i].find_first_of(" \t\f\v\r", pos1);
			}
		}

		/* Search around the equal signs */
		pos1 = lines[i].find("=");
		if (pos1 != std::string::npos) {
			/* Search for whitspace after equal sign */
			pos2 = lines[i].find_first_not_of(" \t\f\v\r", pos1+1);
			if (pos2 != pos1+1) {
				lines[i].erase(pos1+1, pos2-pos1-1);
			}
			/* Search for whitspace before equal sign */
			pos2 = lines[i].find_last_not_of(" \t\f\v\r", pos1-1);
			if (pos2 != pos1-1) {
				lines[i].erase(pos2+1, pos1-pos2-1);
			}
		}
		i++;
	}
}
