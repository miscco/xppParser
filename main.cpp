#include <iostream>
#include <string>
#include "parser.h"

int main()
{
	std::string filename = "../test_parser/Thalamus.ode";
	parser ParseOde(filename);

	for(unsigned i=0; i<ParseOde.lines.size(); ++i) {
		std::cout << ParseOde.lines[i] << std::endl;
	}
	return 0;
}
