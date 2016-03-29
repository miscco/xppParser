#include <parser.h>

void parser::remove_comments() {
	unsigned i = 0;
	while(i<lines.size()) {
		unsigned pos = lines[i].find("#");
		if(pos == 0) {
			lines.erase(lines.begin() + i);
		} else if (pos < lines[i].length()) {
			lines[i].resize(pos);
			i++;
		} else {
			i++;
		}
	}
}

void parser::extract_Constants(void) {
	unsigned i = 0;
	std:: string key = "!";
	while(i<lines.size()) {
		unsigned pos = lines[i].find(key);
		if(pos < lines[i].length()) {
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

void parser::extract_Parameter(void) {
	unsigned i = 0;
	std:: string key = "param";
	while(i<lines.size()) {
		unsigned pos = lines[i].find(key);
		if(pos < lines[i].length()) {
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

void parser::extract_InitConds(void) {
	unsigned i = 0;
	std:: string key = "init";
	while(i<lines.size()) {
		unsigned pos = lines[i].find(key);
		if(pos < lines[i].length()) {
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

void parser::extract_Equations(void) {
	unsigned i=0;
	std::vector<std::string> keywords = {"'", "(t)", "/dt"};
	/* The last keyword is d * /dt so we skip the first char */
	std::vector<int> start_pos = {0 , 0, 1};

	while(i<lines.size()) {
		for(int j=0; j < 3; j++){
			unsigned pos = lines[i].find(keywords[j]);
			if(pos < lines[i].length()) {
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


void parser::extract_Functions(void) {
	unsigned i = 0;
	std:: string key1 = "(";
	std:: string key2 = ")=";
	while(i<lines.size()) {
		unsigned pos1 = lines[i].find(key1);
		unsigned pos2 = lines[i].find(key2);
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


void parser::remove_spaces(void) {
	for(unsigned i=0; i<lines.size(); i++) {
		lines[i].erase(remove_if(lines[i].begin(), lines[i].end(), [](char x){return std::isspace(x);}),
					   lines[i].end());
	}
}


int parser::parse_file(void) {
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


void parser::split_lines(void) {
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
