#ifndef XPPAUTOSETTINGS_H
#define XPPAUTOSETTINGS_H

#include <string>
#include <vector>

#include "xppColor.h"
#include "xppPlots.h"

class xppAutoSettings {
public:
    /**
     * @brief xppMainSettings Default constructor.
     */
    xppAutoSettings() {}

private:
    unsigned Ntst	= 50;
    unsigned Nmax	= 300;
    unsigned NPr	= 50;
    unsigned NCol	= 4;

    double  EPSL    = 1E-7;
    double  EPSU    = 1E-7;
    double  EPSS    = 1E-7;

    double  IAD     = 3.;
    double  MXBF    = 5.;
    double  IID     = 2.;
    double  ITMX    = 8.;
    double  ITNW    = 7.;
    double  NWTN    = 3.;
    double  IADS    = 1.;

    double  Ds      = 1E-3;
    double  DsMax   = 0.5;
    double  DsMin   = 1E-5;

    double  ParMax  = 10.0;
    double  ParMin  = 0.0;
    double  NormMax = 1000.0;
    double  NormMin = 0.0;

    xppAutoColor color = xppAutoColor();
    xppPlotAxes  axes  = xppPlotAxes();

    friend class xppSettings;
};

#endif // XPPAUTOSETTINGS_H
