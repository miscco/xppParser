#ifndef XPPSETTINGS_H
#define XPPSETTINGS_H

#include <stdexcept>
#include <string>

#include "parser/xppParserDefines.h"
#include "settings/xppAutoSettings.h"
#include "settings/xppMainSettings.h"
#include "settings/xppSolverSettings.h"

class xppSettings
{
public:
    xppSettings(const optsArray& options);

    void initializeOptions(const optsArray& options);

private:
    xppMethod setMethod(const std::string& key);

    xppAutoSettings   autoSettings;
    xppMainSettings   mainSettings;
    xppSolverSettings solverSettings;
};

#endif // XPPSETTINGS_H
