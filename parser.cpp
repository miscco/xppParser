#include <parser.h>

parser::parser(std::string fn)
	: fileName(fn)
{
	int err = parse_file();
	if(err == -1) {
		std::cout << "Could not open file " << fileName << std::endl;
	} else if (err==0) {
		std::cout << "Empty ode file " << fileName << std::endl;
	}

	remove_comments();
	remove_spaces();
	split_lines();
	print_lines();

	extract_Constants();
	extract_Equations();
	extract_Functions();
	extract_InitConds();
	extract_Parameter();

}

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
		std::vector<int> left_par(0);
		std::vector<int> right_par(0);
		std::vector<int> comma(0);
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

		/* Check if there was a stray comma at end of line */
		if(comma.size() > 0) {
			if(comma.back()==(int)lines[i].size()-1) {
				lines[i].pop_back();
				comma.pop_back();
			}
		}
		/* If there are commata left then split thestring and insert as a new line */
		if(comma.size()>0) {
			/* Add the size of the line as ending of the last substring */
			comma.push_back(lines[i].size());
			/* Insert the substrings */
			for(unsigned j=0; j<comma.size()-1; j++) {
				lines.insert(lines.begin()+i+j+1, lines[i].substr(comma[j]+1, comma[j+1]-comma[j]-1));
			}
			/* cut the original line */
			lines[i] = lines[i].substr(0, comma[0]);
		}
		i++;
	}
}


void parser::print_lines(void) {
	for(unsigned i=0; i<lines.size(); ++i) {
		std::cout << lines[i] << std::endl;
	}
}
