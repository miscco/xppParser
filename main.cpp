#include <iostream>
#include <string>
#include "xppParser.h"

int main()
{
	std::string filename = "../Thalamus.ode";
	xppParser ParseOde(filename);

	for(unsigned i=0; i<ParseOde.lines.size(); ++i) {
		//std::cout << ParseOde.lines[i] << std::endl;
	}
	return 0;
}
