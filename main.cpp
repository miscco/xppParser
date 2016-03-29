#include <iostream>
#include <string>
#include "parser.h"

int main()
{
	std::string filename = "../test_parser/Thalamus.ode";
	parser ParseOde(filename);

	int err = ParseOde.parse_file();
	if(err == -1) {
		std::cout << "Could not open file " << filename << std::endl;
		return -1;
	} else if (err==0) {
		std::cout << "Empty ode file " << filename << std::endl;
		return 0;
	}
	ParseOde.remove_comments();
	ParseOde.remove_spaces();
	ParseOde.split_lines();

	ParseOde.extract_Constants();
	ParseOde.extract_Equations();
	ParseOde.extract_Functions();
	ParseOde.extract_InitConds();
	ParseOde.extract_Parameter();

	for(unsigned i=0; i<ParseOde.lines.size(); ++i) {
		std::cout << ParseOde.lines[i] << std::endl;
	}
	return 0;
}
