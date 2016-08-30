#include <xppParser.h>

/**
 * @brief Constructor of the parser object
 *
 * @param fn string representing the file name of the ode file
 *
 * This constructs the parser object of a given ode file. First unneeded
 * content is discarded, and a basic correctness check is don. Later arrays are
 * expanded and special constructs like markov processes and tables are handled.
 * Finally, the different keywords are parsed and put into the opts arrays.
 */
xppParser::xppParser(std::string fn)
	: fileName(fn)
{
	try {
		/* Initially read in the ode file */
		readFile();

		/* Initialize the keyword tries for command parsing */
		initializeTries();

		/* Check for incorrect brackets */
		checkBrackets();

		/* Remove all comments */
		removeComments();

		/* Remove unnecessary whitespaces */
		removeWhitespace();

		/* Expand array descriptions */
		expandArrays();

		/* Extract exports to dlls */
		extractExport();

		/* Extract markov processes */
		extractMarkov();

		/* Extract table definitions */
		extractTable();

		/* Extract wiener processes */
		extractWiener();

		/* Extract globals */
		extractGlobal();

		/* Extract all other definitions */
		extractDefinition();

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
xppParser::xppParser(const xppParser &parser)
	: keywords(parser.keywords),
	  options(parser.options),
	  reservedNames(parser.reservedNames),
	  usedNames(parser.usedNames),
	  Algebraic(parser.Algebraic),
	  Auxiliar(parser.Auxiliar),
	  Boundaries(parser.Boundaries),
	  Constants(parser.Constants),
	  Equations(parser.Equations),
	  Exports(parser.Exports),
	  Functions(parser.Functions),
	  Globals(parser.Globals),
	  InitConds(parser.InitConds),
	  Temporaries(parser.Temporaries),
	  Markovs(parser.Markovs),
	  Numbers(parser.Numbers),
	  Options(parser.Options),
	  Parameters(parser.Parameters),
	  Special(parser.Special),
	  Sets(parser.Sets),
	  Tables(parser.Tables),
	  Volterra(parser.Volterra),
	  Wieners(parser.Wieners)
{}

/**
 * @brief Checks whether brackets are closed properly
 */
void xppParser::checkBrackets() {
	std::stack<std::pair<char, size_t>> brackets;
	for (lineNumber line : lines) {
		for (unsigned j=0; j < line.first.size(); j++) {
			switch (line.first.at(j)) {
			case '(':
				brackets.push(std::make_pair<char, size_t>(')', j));
				break;
			case '{':
				brackets.push(std::make_pair<char, size_t>('}', j));
				break;
			case '[':
				brackets.push(std::make_pair<char, size_t>(']', j));
				break;
			case ')':
				if (brackets.empty()) {
					throw xppParserException(MISSING_OPENING_BRACKET, line, j);
				} else if (brackets.top().first != ')') {
					throw xppParserException(MISSING_CLOSING_BRACKET, line,
											 brackets.top().second);
				}
				brackets.pop();
				break;
			case '}':
				if (brackets.empty()) {
					throw xppParserException(MISSING_OPENING_BRACKET, line, j);
				} else if (brackets.top().first != '}') {
					throw xppParserException(MISSING_CLOSING_BRACKET, line,
											 brackets.top().second);
				}
				brackets.pop();
				break;
			case ']':
				if (brackets.empty()) {
					throw xppParserException(MISSING_OPENING_BRACKET, line, j);
				} else if (brackets.top().first != ']') {
					throw xppParserException(MISSING_CLOSING_BRACKET, line,
											 brackets.top().second);
				}
				brackets.pop();
				break;
			default:
				continue;
			}
		}
		if (!brackets.empty()) {
			throw xppParserException(MISSING_CLOSING_BRACKET, line,
									 brackets.top().second);
		}
	}
}

/**
 * @brief Checks a keyword search in case of multiple matches
 *
 * @par result: The emit collection from the keyword search
 * @par character: The character after the first word.
 *
 * This sanitizes the keyword search in case we have multiple matches. If
 * multiple keywords were found, the name of the expression contains a keyword.
 * This is only possible if it is either an temporary expression or a keyword
 * that is merged with the name. Consequently remove all other findings. If
 * there were no valid keywords left/found this must be a temporary expression.
 * So create a fake result with and index equal to the size of xppKeywords.
 */
void xppParser::sanitizeKeywordSearch(resultCollection &results,
									  const char &character) {
	if(character == '=') {
		auto it = results.begin();
		for (auto &res : results) {
			if (res.id == 0 || /* !Name */
				res.id == 2 || /* Name' */
				res.id == 3 || /* dName/dt */
				res.id == 9) { /* Name(Args) */
				it++;
			} else if (res.id == 1 || /* Name(t) */
					   res.id == 4 || /* Name(t+1) */
					   res.id == 11) {  /* Name(0) */
				results.erase(--it);  /* Remove Name(Args) */
			} else {
				results.erase(it);
			}

		}
		/* No valid keyword left, this must be an expression */
		if (results.size() == 0) {
			results.push_back(keywordTrie::result("", xppKeywords.size()));
		}
	}
}

/**
 * @brief Checks whether a given name is already taken or reserved
 *
 * @par name: The name of the new definition
 * @par line: The original line that was parsed from the ode file
 * @par pos: The position of the name in line
 */
void xppParser::checkName(const std::string &name, const lineNumber &line, size_t pos) {
	keywords.setWholeWords(true);
	if (!usedNames.parseText(name).empty()) {
		throw xppParserException(DUPLICATED_NAME, line, pos);
	} else if (!reservedNames.parseText(name).empty()) {
		throw xppParserException(RESERVED_FUNCTION, line, pos);
	} else if (!keywords.parseText(name).empty()) {
		throw xppParserException(RESERVED_KEYWORD, line, pos);
	} else if (!options.parseText(name).empty()) {
		throw xppParserException(RESERVED_OPTION, line, pos);
	}
	keywords.setWholeWords(false);
	usedNames.addString(name);
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
		pos1 = lines[i].first.find("[");
		if (pos1 != std::string::npos) {
			/* Search for the closing bracket and the dots between the start and
			 * the end indices. The dots are only necessary for single line
			 * statements.
			 */
			pos2 = lines[i].first.find("..");
			pos3 = lines[i].first.find("]");
			if (pos3 == std::string::npos) {
				throw xppParserException(MISSING_CLOSING_BRACKET, lines[i], pos1);
			} else if (pos2 != std::string::npos && pos2 > pos3) {
				throw xppParserException(WRONG_ARRAY_ASSIGNMENT, lines[i], pos2);
			}

			/* Determine the range of the indices of the array */
			int start, end;
			try {
				start = std::stoi(lines[i].first.substr(pos1+1, pos2-pos1-1));
			} catch (std::invalid_argument) {
				throw xppParserException(EXPECTED_NUMBER, lines[i], pos1+1);
			}
			try {
				end = std::stoi(lines[i].first.substr(pos2+2, pos3-pos2-2));
			} catch (std::invalid_argument) {
				throw xppParserException(EXPECTED_NUMBER, lines[i], pos2+2);
			}

			/* Copy the lines of the assignment into a separate vector */
			std::vector<lineNumber> arrayExpressions;
			int numLines = 1;
			if (lines[i].first.substr(pos1-1, 1) == "%") {
				/* Multiline statements end with a line with a single "%" */
				while (true) {
					size_t endArray = lines[i+numLines].first.find("%");
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
				arrayExpressions[0].first.replace(pos1, pos3, "[j]");
			}
			lines.erase(lines.begin()+i, lines.begin()+i+numLines);

			/* Expand the array expressions and insert it*/
			std::vector<lineNumber> arrayLines;
			for (int i = start; i <= end; i++) {
				expandArrayLines(arrayLines, arrayExpressions, i);
			}
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
void xppParser::expandArrayLines(std::vector<lineNumber>& lines,
								 const std::vector<lineNumber>& expressions,
								 int idx) {
	/* Initialize the parser with the variable "j" */
	mup::ParserX parser;
	mup::Value j((mup::int_type)idx);
	mup::Value result;
	parser.DefineVar("j",  mup::Variable(&j));
	for (const lineNumber &expr : expressions) {
		lineNumber temp = expr;
		size_t pos1 = temp.first.find("[");
		size_t pos2 = temp.first.find("]");
		while (pos1 != std::string::npos) {
			/* Evaluate the expression within the brackets and replace the
			 * brackets with the result.
			 */
			parser.SetExpr(temp.first.substr(pos1+1, pos2-pos1-1));
			result = parser.Eval();

			/* Check whether the result is an integer */
			if (result.GetType() != 'i') {
				throw xppParserException(WRONG_ARRAY_ASSIGNMENT, expr, pos1+1);
			}
			temp.first.replace(pos1, pos2-pos1+1, result.ToString());

			pos1 = temp.first.find("[", pos2);
			pos2 = temp.first.find("]", pos1);
		}
		lines.push_back(temp);
	}
}

/**
 * @brief Extract definitions from the ode file
 *
 * This extracts definitions that are given in the keyword list and are marked
 * by an equal sign.
 */
void xppParser::extractDefinition(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		/* Search for the first keyword. In most cases it should be the first
		 * consecutive string that precedes a whitespace or equal sign.
		 */
		std::size_t pos1 = 0, pos2 = 0;
		std::string key = getNextWord(lines[i], pos1, pos2);
		if (pos2 == std::string::npos) {
			throw xppParserException(UNKNOWN_ASSIGNMENT, lines[i], pos1+1);
		}
		auto results = keywords.parseText(key);

		while (pos2 != std::string::npos) {
			opts opt;
			opt.Line = lines[i].second;

			/* Sanity check whether an temporary expression was found or the
			 * parsed name contained a keyword. This is only relevant if there
			 * was no separate keyword, e.g Name(t). To simplify the handling
			 * below, we set get_index() of a temporary expression to
			 * keywords.size().
			 */
			sanitizeKeywordSearch(results, lines[i].first.at(pos2));

			switch (results.at(0).id) {
			case 0: /* !Name */
				opt.Name = lines[i].first.substr(pos1+1, pos2-pos1-1);
				break;
			case 1:  /* Name(t+1) */
			case 2:  /* Name' */
			case 4:  /* Name(t) */
			case 11: /* Name(0) */
				opt.Name = lines[i].first.substr(pos1, results.at(0).start);
				break;
			case 3: /* dName/dt */
				opt.Name = lines[i].first.substr(pos1+1, results.at(0).start-1);
				break;
			case 9: {/* Name(args...) */
				size_t pos3 = lines[i].first.find("(", pos1);
				opt.Name = lines[i].first.substr(pos1, pos3-pos1);
				opt.Args = getList(lines[i].first.substr(pos3, pos2-pos3),
								   opt.Line, ")", ",");
				break;
			}
			case 12:
				break;
			case 13: /* 0=Expression */
				opt.Name = "Initial Condition";
				break;
			case 19: /* Name=Expression */
				opt.Name = lines[i].first.substr(pos1, pos2-pos1);
				break;
			default: /* keyword Name */
				opt.Name = getNextWord(lines[i], pos1, pos2);
				break;
			}


			/* Check whether the name is already taken/reserved, except for
			 * initial conditions, where we check for existence.
			 */
			if (results.at(0).id != 10 &&
					results.at(0).id != 17) {
				checkName(opt.Name, lines[i], pos1);
			} else if (results.at(0).id == 10) {
				if (usedNames.parseText(opt.Name).empty()) {
					throw xppParserException(UNKNOWN_VARIABLE, lines[i], pos1);
				}
			} else if (results.at(0).id == 17) {
				if (options.parseText(opt.Name).empty()) {
					throw xppParserException(UNKNOWN_OPTION, lines[i], pos1);
				}
			}

			/* Get the expression */
			opt.Expr = getNextExpr(lines[i], pos1, pos2);

			/* Check numbers are indeed numeric expressions */
			if (results.at(0).id == 8) {
				if (!isNumeric(opt.Expr)) {
					throw xppParserException(EXPECTED_NUMBER, lines[i], pos1);
				}
				/* Check if all function arguments are used */
			} else if (results.at(0).id == 9) {
				size_t pos3 = opt.Name.length()+1;
				for (std::string &str : opt.Args) {
					if (opt.Expr.find(str) == std::string::npos) {
						throw xppParserException(MISSING_ARGUMENT, lines[i], pos3);
					}
					pos3 += str.length()+1;
				}
			}

			/* Find the type of the keyword */
			switch(results.at(0).id) {
			case 0:
				Constants.push_back(opt);
				break;
			case 1:
			case 2:
			case 3:
				Equations.push_back(opt);
				break;
			case 4:
				Volterra.push_back(opt);
				break;
			case 5:
				Constants.push_back(opt);
				break;
			case 6:
				Auxiliar.push_back(opt);
				break;
			case 7:
				Parameters.push_back(opt);
				break;
			case 8:
				Numbers.push_back(opt);
				break;
			case 9:
				Functions.push_back(opt);
				break;
			case 10:
				InitConds.push_back(opt);
				break;
			case 11:
				Volterra.push_back(opt);
				break;
			case 12:
				/* Boundary expressions do not have a name */
				opt.Expr = opt.Name;
				opt.Name = "";
				Boundaries.push_back(opt);
				break;
			case 13:
				Volterra.push_back(opt);
				break;
			case 14:
				Algebraic.push_back(opt);
				break;
			case 15:
				/* Extract the argument list */
				pos1 = opt.Expr.find("(");
				opt.Args = getList(opt.Expr.substr(pos1), opt.Line, ")", ",");
				opt.Expr.resize(pos1);
				Special.push_back(opt);
				break;
			case 16:
				/* Sets are a comma separated list */
				opt.Args = getList(opt.Expr, opt.Line, "}", ",");
				opt.Expr = "";
				Sets.push_back(opt);
				break;
			case 17:
				Options.push_back(opt);
				break;
			case 18:
				Exports.push_back(opt);
				break;
			case 19:
				Temporaries.push_back(opt);
				break;
			default:
				throw xppParserException(UNKNOWN_ASSIGNMENT, lines[i], pos1+1);
				break;
			}
		}
		lines.erase(lines.begin() + i);
	}
}

/**
 * @brief Extract exports to link to procompiled C routines inside xppaut
 *
 * This function extracts the in and out arguments of the respective function.
 * To keep with the other structures the Args vector contains both the input and
 * the output. Therefore the Expr string contains the number of input arguments.
 */
void xppParser::extractExport(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::size_t pos1 = lines[i].first.find("export");
		std::size_t pos2 = lines[i].first.find(" ", pos1);
		if (pos1 != std::string::npos) {
			opts opt;
			opt.Line = lines[i].second;
			stringList temp;
			temp = getList(getNextWord(lines[i], pos1, pos2), opt.Line, "}", ",");
			opt.Args = temp;
			opt.Expr = std::to_string(temp.size());

			temp = getList(getNextWord(lines[i], pos1, pos2), opt.Line, "}", ",");
			opt.Args.insert(opt.Args.end(), temp.begin(), temp.end());

			Exports.push_back(opt);
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

/**
 * @brief Extract a global flag implementing a delta function
 *
 * This function extracts the condition, the sign of the flag and the resets
 */
void xppParser::extractGlobal(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::size_t pos1 = lines[i].first.find("global");
		std::size_t pos2 = lines[i].first.find(" ", pos1);
		if (pos1 != std::string::npos) {
			opts opt;
			opt.Line = lines[i].second;

			/* Parse the sign flag. For simplicity store it in the name slot */
			opt.Name = getNextWord(lines[i], pos1, pos2);

			/* Parse flip condition.
			 */
			pos1 = lines[i].first.find_first_not_of(" ", pos2);
			pos2 = lines[i].first.find("{", pos1);
			opt.Expr = lines[i].first.substr(pos1, pos2-pos1-1);

			/* The individual resets are separated by a semicolon */
			opt.Args = getList(lines[i].first.substr(pos2), opt.Line, "}", ";");
			Globals.push_back(opt);
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

/**
 * @brief Extract a markov process from the parsed lines
 *
 * This function extracts the definition of markov processes as they constitute
 * one of the few multiline statements in an ode file.
 */
void xppParser::extractMarkov(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::size_t pos1 = lines[i].first.find("markov");
		std::size_t pos2 = lines[i].first.find(" ", pos1);
		if (pos1 != std::string::npos) {
			opts opt;
			opt.Line = lines[i].second;
			int nstates;

			/* Parse the name */
			opt.Name = getNextWord(lines[i], pos1, pos2);
			checkName(opt.Name, lines[i], pos1);

			/* Parse the number of states */
			try {
				nstates = std::stoi(getNextWord(lines[i], pos1, pos2));
			} catch (std::invalid_argument) {
				throw xppParserException(EXPECTED_NUMBER, lines[i], pos1);
			}
			/* Store the number of states for convenience */
			opt.Expr = nstates;

			/* Parse the transition probabilities */
			for (int j=0; j < nstates; j++) {
				pos2 = 0;
				for (int k=0; k < nstates; k++) {
					pos1 = lines[i+1].first.find("{", pos2);
					pos2 = lines[i+1].first.find("}", pos1);
					if (pos1 == std::string::npos) {
						throw xppParserException(MISSING_MARKOV_ASSIGNMENT,
												 lines[i+1], pos2);
					}
					opt.Args.push_back(lines[i+1].first.substr(pos1+1, pos2-pos1-1));
				}
				if (pos2 != lines[i+1].first.size()-1) {
					throw xppParserException(WRONG_MARKOV_ASSIGNMENT,
											 lines[i+1], lines[i+1].first.size());
				}
				lines.erase(lines.begin() + i + 1);
			}
			Markovs.push_back(opt);
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

/**
 * @brief Extract a table
 *
 * This function extracts a precomputed table either from a file or calculates
 * it from the definition.
 */
void xppParser::extractTable(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::size_t pos1 = lines[i].first.find("table");
		std::size_t pos2 = lines[i].first.find(" ", pos1);
		if (pos1 != std::string::npos) {
			opts opt;
			opt.Line = lines[i].second;
			unsigned npoints;
			double xLow, xHigh;

			/* Initialize the parser */
			mup::ParserX parser;

			/* Parse the name */
			opt.Name = getNextWord(lines[i], pos1, pos2);
			checkName(opt.Name, lines[i], pos1);

			/* If the table has to be calculated there is a % sign instead of a
			 * filename
			 */
			std::string fn = getNextWord(lines[i], pos1, pos2);
			if (fn == "%") {
				/* Get the number of points */
				try {
					npoints = std::stoi(getNextWord(lines[i], pos1, pos2));
				} catch (std::invalid_argument) {
					throw xppParserException(EXPECTED_NUMBER, lines[i], pos1);
				}

				/* Get the bounds */
				parser.SetExpr(getNextWord(lines[i], pos1, pos2));
				try {
					xLow = parser.Eval().GetFloat();
				} catch (mup::ParserError) {
					throw xppParserException(EXPECTED_NUMBER, lines[i], pos1);
				}
				parser.SetExpr(getNextWord(lines[i], pos1, pos2));
				try {
					xHigh = parser.Eval().GetFloat();
				} catch (mup::ParserError) {
					throw xppParserException(EXPECTED_NUMBER, lines[i], pos1);
				}

				/* Parse the defining function */
				mup::Value t;
				parser.DefineVar("t",  mup::Variable(&t));
				parser.SetExpr(getNextWord(lines[i], pos1, pos2));

				/* Evaluate the table expression */
				double dx = (xHigh - xLow)/(npoints-1);
				try {
					for(unsigned j = 0; j < npoints; j++) {
						t = (mup::float_type)(xLow + j * dx);
						opt.Args.push_back(parser.Eval().ToString());
					}
				} catch (mup::ParserError) {
					throw xppParserException(WRONG_TABLE_ASSIGNMENT, lines[i], pos1);
				}
			} else {
				/* Open file for parsing */
				std::ifstream fileStream(fn.c_str(), std::ios::in);
				if (fileStream.fail()) {
					throw std::runtime_error("Cannot open table file " + fn + "\n");
				}
				lineNumber temp;
				temp.second = 1;

				/* Get the number of points */
				try {
					getline(fileStream, temp.first);
					temp.second++;
					npoints = std::stoi(temp.first);
				} catch (std::invalid_argument) {
					throw xppParserException(EXPECTED_NUMBER, temp, pos1);
				}

				/* Get the bounds */
				getline(fileStream, temp.first);
				temp.second++;
				parser.SetExpr(temp.first);
				try {
					xLow = parser.Eval().GetFloat();
				} catch (mup::ParserError) {
					throw xppParserException(EXPECTED_NUMBER, temp, pos1);
				}
				getline(fileStream, temp.first);
				temp.second++;
				parser.SetExpr(temp.first);
				try {
					xHigh = parser.Eval().GetFloat();
				} catch (mup::ParserError) {
					throw xppParserException(EXPECTED_NUMBER, temp, pos1);
				}

				/* Parse the table values */
				try {
					for(unsigned j = 0; j < npoints; j++) {
						if (getline(fileStream, temp.first)) {
							temp.second++;
							parser.SetExpr(temp.first);
							opt.Args.push_back(parser.Eval().ToString());
						} else {
							throw xppParserException(EXPECTED_TABLE_ASSIGNMENT,
													 temp, pos1);
						}
					}
				} catch (mup::ParserError) {
					throw xppParserException(WRONG_TABLE_ASSIGNMENT, temp, pos1);
				}
				fileStream.close();
			}
			Tables.push_back(opt);
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

/**
 * @brief Extracts wiener processes
 *
 * This function extracts the declaration of wiener processes as their
 * definition does not contain an equal sign but only the names of the
 * individual proceses.
 */
void xppParser::extractWiener(void) {
	unsigned i = 0;
	while(i < lines.size()) {
		std::size_t pos1 = lines[i].first.find("wiener");
		std::size_t pos2 = lines[i].first.find(" ", pos1);
		if (pos1 != std::string::npos) {
			while (pos2 != std::string::npos) {
				Wieners.Args.push_back(getNextWord(lines[i], pos1, pos2));
				checkName(Wieners.Args.back(), lines[i], pos1);
			}
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

/**
 * @brief Finds the position of the next assignment within a given line
 *
 * @par line: String we are searching in
 * @par	pos1: Old start position which will be udpated
 * @par	pos2: Position of the last equal sign
 *
 * It is possible that a single line contains multiple assignments that are
 * separated by a commata. Therefore, search for the next commata after pos2
 * that is not encapsulated by a bracket and update pos1 to the position after
 * it.
 */
void xppParser::findNextAssignment(const lineNumber &line,
								   size_t &pos1,
								   size_t &pos2) {
	std::stack<char> brackets;
	pos1 = pos2;
	for (unsigned i=pos2; i < line.first.size(); i++) {
		switch (line.first.at(i)) {
		case '(':
			brackets.push(')');
			break;
		case '{':
			brackets.push('}');
			break;
		case '[':
			brackets.push(']');
			break;
		case ')':
			brackets.pop();
			break;
		case '}':
			brackets.pop();
			break;
		case ']':
			brackets.pop();
			break;
		case ',':
			if (brackets.empty()) {
				pos2 = i+1;
				return;
			} else {
				continue;
			}
		default:
			continue;
		}
	}
	pos2 = std::string::npos;
}

/**
 * @brief Extracts elements from a brace enclosed list
 *
 * @par line: String we are searching in
 * @par	encloure: The bracket type { or (
 * @par	delim: The delimiter type , or ;
 *
 * @return Args: Vector of strings of the list elements
 */
stringList xppParser::getList(const std::string& line,
							  unsigned ln,
							  const std::string &closure,
							  const std::string &delim) {
	size_t pos1 = 1;
	size_t pos2 = line.find_first_of(delim+closure, pos1);
	stringList temp;
	while (pos2 != std::string::npos) {
		temp.push_back(line.substr(pos1, pos2-pos1));
		if (temp.back().empty()) {
			throw xppParserException(EXPECTED_LIST_ARGUMENT,
									 std::make_pair(line, ln), pos1);
		}
		pos1 = pos2+1;
		pos2 = line.find_first_of(delim+closure, pos1);
	}
	return temp;
}

/**
 * @brief Gets the next expression between pos2 and a commata outside braces
 *
 * @par line: String we are searching in
 * @par	pos1: Old position. Will be updated to the position after the commata
 * or std::string::npos.
 * @par	pos2: Position of the equal sign
 *
 * @return string: String between pos2 and the next commata outside of braces
 */
std::string xppParser::getNextExpr(const lineNumber &line,
								   size_t &pos1,
								   size_t &pos2) {
	findNextAssignment(line, pos1, pos2);
	std::string expr = line.first.substr(pos1+1, pos2-pos1-2);
	/* Remove whitespaces */
	size_t pos;
	while ((pos = expr.find_first_of(" \t\f\v\r")) != std::string::npos) {
		expr.erase(pos, 1);
	}
	return expr;
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
std::string xppParser::getNextWord(const lineNumber &line,
								   size_t &pos1,
								   size_t &pos2) {
	pos1 = line.first.find_first_not_of(" ", pos2);
	pos2 = line.first.find_first_of(" =", pos1);
	return line.first.substr(pos1, pos2-pos1);
}

/**
 * @brief Initializes the keyword tree from the keyword list
 */
void xppParser::initializeTries (void) {
	keywords.addString(xppKeywords);

	options.setCaseSensitivity(false);
	options.setWholeWords(true);
	options.addString(xppOptionNames);

	reservedNames.setWholeWords(true);
	reservedNames.addString(xppReservedNames);

	usedNames.setWholeWords(true);
}

/**
 * @brief Checks whether a provided string is a numeric expression
 *
 * @par str: The std::string containing the expression
 *
 * @return true if the string is a numeric expression, false otherwise
 */
bool xppParser::isNumeric(const std::string &str) {
	char* p;
	std::strtod(str.c_str(), &p);
	return (*p == 0.0);
}

/**
 * @brief Reads in the ode file and stores the lines in a vector
 *
 * This function reads in the ode file given by fileName, ignoring empty lines
 * as well lines containing only whitespace. Furthermore it trims and truncates
 * all whitespaces.
 */
void xppParser::readFile(void) {
	std::ifstream fileStream(fileName.c_str(), std::ios::in);
	if (fileStream.fail()) {
		throw std::runtime_error("Cannot open ode file " + fileName + "\n");
	}

	/* Parse in nonempty lines */
	std::string temp;
	int lineCount = 1;
	while(getline(fileStream, temp)) {
		size_t pos1 = temp.find_first_not_of(" \t\f\v\r\n");
		if (temp == "done") {
			break;
		} else if (temp.length() != 0 && pos1 != std::string::npos) {
			/* Remove trailing and superflous whitespaces */
			temp.erase(0, pos1);
			temp.resize(temp.find_last_not_of(" \t\f\v\r\n")+1);
			auto last = std::unique(temp.begin(), temp.end(),
									[](char l, char r){return std::isspace(l) &&
						std::isspace(r);});
			temp.erase(last, temp.end());
			lines.push_back(std::make_pair(temp, lineCount));
		}
		lineCount++;
	}

	if (lines.size() == 0) {
		throw std::runtime_error("Empty ode file " + fileName + "\n");
	}
	fileStream.close();
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
		std::size_t pos1 = lines[i].first.find("#");
		if (pos1 == 0) {
			lines.erase(lines.begin() + i);
		} else if (pos1 != std::string::npos) {
			/* Check if this is the definition of a convolutional integral
			 * indicated by curly braces sourrunding the # sign
			 */
			std::size_t subpos1 = lines[i].first.find_last_of("{", pos1);
			std::size_t subpos2 = lines[i].first.find("}", subpos1);
			if (subpos1 == std::string::npos || subpos2 < pos1) {
				lines[i].first.resize(pos1);
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

		/* Remove all whitespaces between brackets */
		pos1 = lines[i].first.find("(");
		if (pos1 != std::string::npos) {
			pos2 = lines[i].first.find(")");
			pos3 = lines[i].first.find_first_of(" \t\f\v\r", pos1);
			while (pos3 < pos2) {
				lines[i].first.erase(pos3, 1);
				pos3 = lines[i].first.find_first_of(" \t\f\v\r", pos1);
			}
		}

		/* Search around the equal signs */
		pos1 = lines[i].first.find("=");
		if (pos1 != std::string::npos) {
			pos2 = lines[i].first.find_first_not_of(" \t\f\v\r", pos1+1);
			if (pos2 != pos1+1) {
				lines[i].first.erase(pos1+1, pos2-pos1-1);
			}
			pos2 = lines[i].first.find_last_not_of(" \t\f\v\r", pos1-1);
			if (pos2 != pos1-1) {
				lines[i].first.erase(pos2+1, pos1-pos2-1);
			}
		}

		/* Remove trailing commata etc */
		pos1 = lines[i].first.find_last_not_of(",;");
		if (pos1 != lines[i].first.size()-1) {
			lines[i].first.resize(pos1+1);
		}
		i++;
	}
}

/**
 * @brief Returns the different parsed objects and their definition
 */
void xppParser::summarizeOde() {
	for (opts &opt : Constants) {
		std::cout << "Added constant " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
	for (opts &opt : Numbers) {
		std::cout << "Added number " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
	for (opts &opt : Temporaries) {
		std::cout << "Added temporary " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
	for (opts &opt : Parameters) {
		std::cout << "Added parameter " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
	for (opts &opt : Algebraic) {
		std::cout << "Added algebraic expression " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
	for (opts &opt : Auxiliar) {
		std::cout << "Added auxiliar variable " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
	for (opts &opt : Boundaries) {
		std::cout << "Added boundary condition: " << opt.Name << std::endl;
	}
	for (opts &opt : Globals) {
		std::cout << "Added delta function " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
	for (opts &opt : Functions) {
		std::cout << "Added function " << opt.Name << "(";
		std::string temp;
		for (std::string &arg : opt.Args) {
			temp += arg + ",";
		}
		temp.pop_back();
		std::cout << temp << ")=" << opt.Expr << std::endl;
	}
	for (opts &opt : Equations) {
		std::cout << "Added ODE " << opt.Name
				  << "'=" << opt.Expr << std::endl;
	}
	for (opts &opt : Volterra) {
		std::cout << "Added differential ODE " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
	if (Wieners.Args.size() > 0) {
		std::cout << "Added wiener processes ";
		for (std::string &str : Wieners.Args) {
			std::cout << str << " ";
		}
		std::cout << std::endl;
	}
	for (opts &opt : Markovs) {
		std::cout << "Added markov processes " << opt.Name << std::endl;
	}
	for (opts &opt : InitConds) {
		std::cout << "Set initial conditions " << opt.Name
				  << "=" << opt.Expr << std::endl;
	}
}
