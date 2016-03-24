#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
typedef struct {
	std::string					Name;
	std::string					Value;
	std::vector<std::string>	Args;
} opts;
typedef std::vector<std::string> vec_str;
typedef std::vector<opts> optsArray;

int parse_file(vec_str &, string &);
optsArray extract_Constants(vec_str &);
optsArray extract_Equations(vec_str &);
optsArray extract_Functions(vec_str &);
optsArray extract_InitConds(vec_str &);
optsArray extract_Options(vec_str &);
optsArray extract_Parameter(vec_str &);
void remove_comments(vec_str &);
void remove_spaces(vec_str &);
void split_lines(vec_str &);

int main()
{
	string filename = "../test_parser/Thalamus.ode";
	vec_str lines;
	int err = parse_file(lines, filename);
	if(err == -1) {
		std::cout << "Could not open file " << filename << std::endl;
		return -1;
	} else if (err==0) {
		std::cout << "Empty ode file " << filename << std::endl;
		return 0;
	}
	remove_comments(lines);
	remove_spaces(lines);
	split_lines(lines);

	optsArray Constants = extract_Constants(lines);
	optsArray Parameter = extract_Parameter(lines);
	optsArray Equations = extract_Equations(lines);
	optsArray Functions = extract_Functions(lines);
	optsArray Settings;

	for(unsigned i=0; i<lines.size(); ++i) {
		cout << lines[i] << endl;
	}
	return 0;
}

void remove_comments(vector<string> &lines) {
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

optsArray extract_Constants(vec_str &lines) {
	unsigned i = 0;
	optsArray Constants;
	string key = "!";
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
	return Constants;
}

optsArray extract_Parameter(vec_str &lines) {
	unsigned i = 0;
	optsArray Parameter;
	string key = "param";
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
	return Parameter;
}

optsArray extract_InitConds(vec_str &lines) {
	unsigned i = 0;
	optsArray Parameter;
	string key = "init";
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
	return Parameter;
}

optsArray extract_Equations(vec_str &lines) {
	unsigned i=0;
	optsArray Equation;
	vec_str keywords = {"'", "(t)", "/dt"};
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
				Equation.push_back(opt);
				lines.erase(lines.begin() + i);
				break;
			}
			if(j==2) {
				i++;
			}
		}
	}
	return Equation;
}


optsArray extract_Functions(vec_str &lines) {
	unsigned i = 0;
	optsArray Functions;
	string key1 = "(";
	string key2 = ")=";
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
	return Functions;
}

void remove_spaces(vec_str &str) {
	for(unsigned i=0; i<str.size(); i++) {
		str[i].erase(remove_if(str[i].begin(), str[i].end(), [](char x){return std::isspace(x);}),
					 str[i].end());
	}
}



int parse_file(vec_str &lines, string &fileName) {
	/* Open file for parsing */
	ifstream fileStream(fileName.c_str(), std::ios::in);
	if(fileStream==NULL) {
		return -1;
	}
	/* Parse in nonempty lines */
	string temp;
	while(getline(fileStream, temp)) {
		if(temp.length()) {
			lines.push_back(temp);
		}
	}
	return lines.size();
}


void split_lines(vec_str &str, vec_str &keywords) {
	unsigned i=0;
	while(i<str.size()) {
		vec_str temp();
		i++;
	}
}
