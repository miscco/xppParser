#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
typedef std::vector<std::pair<std::string, std::string> > string_pair;

void remove_comments(vector<string> &);
string_pair extract_Constants(std::vector<std::string> &);
string_pair extract_Parameter(std::vector<std::string> &);
void remove_Spaces(string &);

int main()
{
	string filename = "/nfshome/schellen/workspace/test_parser/Thalamus.ode";
	ifstream fileStream(filename.c_str(), std::ios::in);
	vector<string> lines;
	string temp;
	while(getline(fileStream, temp)) {
		if(temp.length()) {
			lines.push_back(temp);
		}
	}
	remove_comments(lines);

	string_pair Constants = extract_Constants(lines);
	string_pair Parameter = extract_Parameter(lines);
	string_pair Equations;
	string_pair Functions;
	string_pair Settings;

	for(unsigned i=0; i<lines.size(); ++i) {
		cout << lines[i] << endl;
	}
	return 0;
}

void remove_comments(vector<string> &lines) {
	unsigned i = 0;
	while(i<lines.size()) {
		unsigned pos = lines[i].find_first_of("#");
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

string_pair extract_Constants(std::vector<std::string> &lines) {
	unsigned i = 0;
	string_pair Constants;
	string key = "!";
	while(i<lines.size()) {
		unsigned pos = lines[i].find(key);
		if(pos < lines[i].length()) {
			lines[i].erase(0, pos+key.length());
			pos = lines[i].find("=");
			string name = lines[i].substr(0,pos);
			string value = lines[i].substr(pos+1);
			Constants.push_back(pair<string, string>(name, value));
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
	return Constants;
}

string_pair extract_Parameter(std::vector<std::string> &lines) {
	unsigned i = 0;
	string_pair Parameter;
	string key = "param";
	while(i<lines.size()) {
		unsigned pos = lines[i].find(key);
		if(pos < lines[i].length()) {
			lines[i].erase(0, pos+key.length()+1);
			pos = lines[i].find("=");
			string name = lines[i].substr(0,pos);
			string value = lines[i].substr(pos+1);
			Parameter.push_back(pair<string, string>(name, value));
			lines.erase(lines.begin() + i);
		} else {
			i++;
		}
	}
	return Parameter;
}

void remove_Spaces(string &str) {
  str.erase(remove_if(str.begin(), str.end(), [](char x){return std::isspace(x);}),
			str.end());
}
