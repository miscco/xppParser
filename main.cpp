#include <iostream>
#include <string>
#include "xppParser.h"

int main(int argc, char** argv)
{
	std::string filename;
	if (argc < 2) {
		filename = "../Test.ode";
	} else {
		filename = std::string(argv[1]);
	}
	xppParser ParseOde(filename);

	return 0;
}
