#include <iostream>
#include <string>

#include "parser/xppEvaluator.h"
#include "parser/xppParser.h"
#include "settings/xppSettings.h"

int main(int argc, char** argv)
{
    std::string filename;
    if (argc < 2) {
        filename = "../Test.ode";
    } else {
        filename = std::string(argv[1]);
    }
    xppParser parseOde(filename);

    xppSettings(parseOde.getOptions());

    xppEvaluator evaluateOde(parseOde);

    return 0;
}
