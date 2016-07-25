#include <parser.h>

parser::parser(std::string fn)
: fileName(fn)
{
	int err = parseFile();
	if(err == -1) {
		std::cout << "Could not open file " << fileName << std::endl;
		exit(-1);
	} else if (err==0) {
		std::cout << "Empty ode file " << fileName << std::endl;
		exit(0);
	}

	removeComments();
	removeSpaces();
	expandArrays();
	splitLines();

	extractConstants();
	extractEquations();
	extractFunctions();
	extractInitConds();
	extractParameter();
}

void parser::expandArrays() {
	unsigned i=0;
	while(i<lines.size()) {
		/* Search for brackets */
		std::size_t pos1 = lines[i].find("[");
		std::size_t pos2, pos3;
		if(pos1 != std::string::npos) {
			/* Search for the closing and the dots between the start and
			 * end number
			 */
			pos2 = lines[i].find("..");
			pos3 = lines[i].find("]");

			/* Determine the range of the indices of the array */
			int start = std::stoi(lines[i].substr(pos1+1, pos2-pos1-1));
			int end   = std::stoi(lines[i].substr(pos2+2, pos3-pos2-2));

			/* Copy the lines */
			for (int j= start; j<=end; j++) {
				std::string temp = lines[i];
				temp.replace(pos1, pos3-pos1+1, std::to_string(j));
				std::size_t subpos1 = temp.find("[");
				std::size_t subpos2 = temp.find("]");
				while(subpos1 != std::string::npos) {
				temp.replace(subpos1, subpos2-subpos1+1, std::to_string(j));
				subpos1 = temp.find("[");
				subpos2 = temp.find("]");
				}
				std::cout << temp << std::endl;
			}
			i++;
		} else {
			i++;
		}

	}
}

void parser::removeComments() {
	unsigned i = 0;
	while(i<lines.size()) {
		std::size_t pos = lines[i].find("#");
		if(pos == 0) {
			lines.erase(lines.begin() + i);
		} else if (pos != std::string::npos) {
			lines[i].resize(pos);
			i++;
		} else {
			i++;
		}
	}
}

void parser::extractConstants(void) {
	unsigned i = 0;
	std:: string key = "!";
	while(i<lines.size()) {
		std::size_t pos = lines[i].find(key);
		if(pos != std::string::npos) {
			opts opt;
			lines[i].erase(0, pos+key.length());
			pos		  = lines[i].find("=");
			opt.Name  = lines[i].substr(0,pos);
			opt.Value = lines[i].substr(pos+1);
			Constants.push_back(opt);
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

void parser::extractParameter(void) {
	unsigned i = 0;
	std:: string key = "param";
	while(i<lines.size()) {
		std::size_t pos = lines[i].find(key);
		if(pos != std::string::npos) {
			opts opt;
			lines[i].erase(0, pos+key.length());
			pos		  = lines[i].find("=");
			opt.Name  = lines[i].substr(0,pos);
			opt.Value = lines[i].substr(pos+1);
			Parameter.push_back(opt);
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

void parser::extractInitConds(void) {
	unsigned i = 0;
	std:: string key = "init";
	while(i<lines.size()) {
		std::size_t pos = lines[i].find(key);
		if(pos != std::string::npos) {
			opts opt;
			lines[i].erase(0, pos+key.length());
			pos		  = lines[i].find("=");
			opt.Name  = lines[i].substr(0,pos);
			opt.Value = lines[i].substr(pos+1);
			Parameter.push_back(opt);
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}

void parser::extractEquations(void) {
	unsigned i=0;
	std::vector<std::string> keywords = {"'", "(t)", "/dt"};
	/* The last keyword is d * /dt so we skip the first char */
	std::vector<int> start_pos = {0 , 0, 1};

	while(i<lines.size()) {
		for(int j=0; j < 3; j++){
			std::size_t pos = lines[i].find(keywords[j]);
			if(pos != std::string::npos) {
				opts opt;
				lines[i].erase(pos, keywords[j].length());
				pos		  = lines[i].find("=");
				opt.Name  = lines[i].substr(start_pos[j],pos-start_pos[j]);
				opt.Value = lines[i].substr(pos+1);
				Equations.push_back(opt);
				lines.erase(lines.begin() + i);
				break;
			}
			if(j==2) {
				i++;
			}
		}
	}
}


void parser::extractFunctions(void) {
	unsigned i = 0;
	std:: string key1 = "(";
	std:: string key2 = ")=";
	while(i<lines.size()) {
		std::size_t pos1 = lines[i].find(key1);
		std::size_t pos2 = lines[i].find(key2);
		if(pos2 < lines[i].length()) {
			opts opt;
			/* Extract the funtion arguments, splitting at commata */
			std::string args = lines[i].substr(pos1+1, pos2-(pos1+1));
			while(args.length()>0) {
				unsigned comma = args.find(",");
				if(comma<args.length()) {
					opt.Args.push_back(args.substr(0, comma));
					args.erase(0, comma+1);
				} else {
					opt.Args.push_back(args);
					args = {};
				}
			}
			opt.Name  = lines[i].substr(0,pos1);
			opt.Value = lines[i].substr(pos2+1);
			Functions.push_back(opt);
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
}


void parser::removeSpaces(void) {
	for(unsigned i=0; i<lines.size(); i++) {
		lines[i].erase(remove_if(lines[i].begin(), lines[i].end(), [](char x){return std::isspace(x);}),
					   lines[i].end());
	}
}


int parser::parseFile(void) {
	/* Open file for parsing */
	std::ifstream fileStream(fileName.c_str(), std::ios::in);
	if(fileStream==NULL) {
		return -1;
	}
	/* Parse in nonempty lines */
	std:: string temp;
	while(getline(fileStream, temp)) {
		if(temp.length()) {
			lines.push_back(temp);
		}
	}

	return lines.size();
}


void parser::splitLines(void) {
	unsigned i=0;
	while(i<lines.size()) {
		std::vector<int> left_par;
		std::vector<int> right_par;
		std::vector<int> comma;
		/* Search for commata. However, if they are inside a parenthis,
		 * then they are part of a function and we cannot split those.
		 */
		for(unsigned j=0; j<lines[i].size(); j++) {
			if(lines[i][j]=='(') {
				left_par.push_back(j);
			} else if (lines[i][j]==')') {
				right_par.push_back(j);
			} else if (lines[i][j]==',') {
				comma.push_back(j);
			}
		}
		comma.push_back(lines[i].size());
		if(left_par.size() != right_par.size()) {
			std::cout << "Error: Missing parenthis.\n";
		}
		/* Remove those indices to commata inside a parenthis */
		for(unsigned j=0; j<left_par.size(); j++) {
			unsigned k=0;
			while(k<comma.size() && comma.size()>0) {
				if(comma[k]>=left_par[j] && comma[k]<=right_par[j]) {
					comma.erase(comma.begin()+k);
				} else {
					k++;
				}
			}
		}

		/* Create the substrings */
		std::vector<std::string> substrings;
		for(unsigned j=0; j<comma.size()-1; j++) {
			substrings.push_back(lines[i].substr(comma[j], comma[j+1]));
		}

		/* Check if there were kexwords omitted in the listing */
		i++;
	}
}
