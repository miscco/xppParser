#ifndef XPPMAINSETTINGS_H
#define XPPMAINSETTINGS_H

#include <string>
#include <vector>

#include "xppColor.h"
#include "xppPlots.h"

class xppMainSettings {
public:
    /**
     * @brief xppMainSettings Default constructor.
     */
    xppMainSettings() {}
private:
    bool quietMode      = false;
    bool useGradient    = false;
    bool runNow         = false;
    bool simPlot        = false;
    bool is3D           = false;

    unsigned seedRNG    = 0;
    unsigned numPlots   = 1;
    unsigned numJump    = 1;
    unsigned numMesh    = 40;
    unsigned gridDF     = 10;
    unsigned maxStore   = 5000;

    double	tStart      = 0.0;
    double  tEnd        = 20.0;
    double	trans       = 0.0;

    double  bound       = 100.;

    std::string	logFile		= "";
    std::string outputFile  = "";

    xppMainColor color = xppMainColor();
    std::vector<xppPlotAxes>  axes  = {xppPlotAxes()};
    friend class xppSettings;
};

#endif // XPPMAINSETTINGS_H
