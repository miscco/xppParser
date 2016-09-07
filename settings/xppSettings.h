#ifndef XPPSETTINGS_H
#define XPPSETTINGS_H

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "parser/xppParserDefines.h"
#include "settings/xppAutoSettings.h"
#include "settings/xppMainSettings.h"
#include "settings/xppSolverSettings.h"

class xppSettings
{
    typedef std::pair<std::string, bool*>        bPair;
    typedef std::pair<std::string, double*>      dPair;
    typedef std::pair<std::string, std::string*> sPair;
    typedef std::pair<std::string, unsigned*>    uPair;
public:
    xppSettings(const optsArray& options);

    template<typename T>
    T getOption(const std::string &key);

    void setOption(const opts &opt);

private:
    std::map<std::string, bool*>        boolMap;
    std::map<std::string, double*>      doubleMap;
    std::map<std::string, std::string*> stringMap;
    std::map<std::string, unsigned*>    uintMap;

    xppAutoSettings   autoSettings;
    xppMainSettings   mainSettings;
    xppSolverSettings solverSettings;
};

#endif // XPPSETTINGS_H
