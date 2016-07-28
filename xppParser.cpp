#include <xppParser.h>

/**
 * @brief Constructor of the parser object
 *
 * @param fn string representing the file name of the ode file
 *
 * The constructs the parser object from a given ode file. First unneeded
 * content is discarded, then arrays are expanded. Finally the different
 * keywords are parsed and put into the opts arrays.
 */
xppParser::xppParser(std::string fn)
	: fileName(fn)
{
	try {
		/* Initially read in the ode file */
		readFile();

		/* Initialize the keyword tree for command paring */
		initializeTree();

		/* Remove all comments */
		removeComments();

		/* Expand array descriptions */
		expandArrays();

		/* Extract markove processes */
		extractMarkov();

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
			 * the end indices
			 */
			pos2 = lines[i].find("..");
			pos3 = lines[i].find("]");
			if (pos3 == std::string::npos) {
				throw xppParserException(MISSING_BRACKET, this->lines[i], i, pos1);
			} else if (pos2 != std::string::npos)  {
				if (pos2 > pos3) {
					throw xppParserException(WRONG_ARRAY_ASSIGNMENT, this->lines[i], i, pos2);
				}
			}

			/* Determine the range of the indices of the array */
			int start, end;
			try {
				start = std::stoi(lines[i].substr(pos1+1, pos2-pos1-1));
			} catch (std::invalid_argument) {
				throw xppParserException(NO_NUMBER, this->lines[i], i, pos1+1);
			}
			try {
				end = std::stoi(lines[i].substr(pos2+2, pos3-pos2-2));
			} catch (std::invalid_argument) {
				throw xppParserException(NO_NUMBER, this->lines[i], i, pos2+2);
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
			for (int i=start; i<=end; i++) {
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
 */
void xppParser::extractDefinitions(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::string  keyword;
		/* Any definition is marked by the equal sign */
		std::size_t pos1;
		std::size_t pos2 = lines[i].find("=");

		/* If this function is reached every other valid statement should be
		 * handeled, so bail out if we do not find an assignment.
		 */
		if (pos2 == std::string::npos) {
			pos1 = lines[i].find("done");
			if (pos1 == std::string::npos) {
				throw xppParserException(UNKNOWN_ASSIGNMENT, this->lines[i], i, 0);
			} else {
				lines.erase(lines.begin()+i);
			}
		}

		/* For simplicity remove all whitespaces between an equal sign and the
		 * preceding word.
		 */
		while (pos2 != std::string::npos) {
			pos1 = lines[i].find_last_not_of(" \t\f\v\r", pos2-1);
			if (pos1 != pos2-1) {
				lines[i].erase(pos1+1, pos2-pos1-1);
			}
			pos2 = lines[i].find("=", pos2+1);
		}

		pos2 = lines[i].find("=");
		while (pos2 != std::string::npos) {
			std::size_t pos1 = lines[i].find_first_of(" \t\f\v\r");
			keyword = lines[i].substr(0, pos1-1);
			std::cout << pos1 << "\t"
					  << keyword << std::endl;
			pos2 = lines[i].find("=", pos2+1);
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

			pos1 = lines[i].find_first_of(" ", pos1);
			pos1 = lines[i].find_first_not_of(" ", pos1+1);
			pos2 = lines[i].find_first_of(" ", pos1);
			opt.Name = lines[i].substr(pos1, pos2-pos1);

			/* Parse the number of states */
			pos1 = lines[i].find_first_not_of(" ", pos2);
			pos2 = lines[i].find_first_of(" ", pos1);
			if (pos2 == std::string::npos) {
				try {
					nstates = std::stoi(lines[i].substr(pos1, lines[i].back()));
				} catch (std::invalid_argument) {
					throw xppParserException(NO_NUMBER, this->lines[i], i, pos1);
				}
			} else {
				try {
					nstates = std::stoi(lines[i].substr(pos1, pos2-pos1));
				} catch (std::invalid_argument) {
					throw xppParserException(NO_NUMBER, this->lines[i], i, pos1);
				}
			}
			/* Store the number of states for convenience */
			opt.Value = nstates;

			/* Parse the transition probabilities */
			for (int j=0; j < nstates; j++) {
				pos1 = 0;
				pos2 = 0;
				for (int k=0; k < nstates; k++) {
					pos1 = lines[i+1].find("{", pos2);
					pos2 = lines[i+1].find("}", pos1);
					if (pos1 == std::string::npos ||
						pos2 == std::string::npos) {
						throw xppParserException(WRONG_MARKOV_ASSIGNMENT, this->lines[i+1], i+1, pos1);
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
 * @brief Initializes the keyword tree from the keywords
 */
void xppParser::initializeTree (void) {
	for (size_t i=0; i < keywords.size(); i++) {
		keywordTree.insert(keywords.at(i));
	}
}

/**
 * @brief Reads in the ode file and stores it in a vector
 *
 * The function ignores empty lines as well lines containing only whitespace
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
		size_t pos = temp.find_first_not_of(" ");
		if (temp.length() != 0 && pos != std::string::npos) {
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
		std::size_t pos = lines[i].find("#");
		std::size_t pos2 = lines[i].find_first_not_of(" \t\f\v\r\n");
		std::size_t pos3 = lines[i].find_last_not_of(" \t\f\v\r\n");
		if (pos == 0 || pos == pos2) {
			lines.erase(lines.begin() + i);
		} else if (pos != std::string::npos) {
			lines[i].resize(pos);
			i++;
		} else if (pos3 != lines[i].size()-1) {
			lines[i].resize(pos3);
			i++;
		} else {
			i++;
		}
	}
}
