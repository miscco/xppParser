#include <iostream>
#include <string>

#include "xppEvaluator.h"
#include "xppParser.h"

int main(int argc, char** argv)
{
	std::string filename;
	if (argc < 2) {
		filename = "../Test.ode";
	} else {
		filename = std::string(argv[1]);
	}
	xppParser parseOde(filename);

	xppEvaluator evaluateOde(parseOde);

	return 0;
}
