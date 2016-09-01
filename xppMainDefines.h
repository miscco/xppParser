#ifndef XPPMAINDEFINES_H
#define XPPMAINDEFINES_H

#include <string>
#include <vector>

#include "xppColor.h"

enum xppMethod {
	METHOD_DISCRETE = 0,
	METHOD_EULER,
	METHOD_MODEULER,
	METHOD_RK4,
	METHOD_ADAMS,
	METHOD_GEAR,
	METHOD_VOLTERRA,
	METHOD_BACKEUL,
	METHOD_RKQS,
	METHOD_STIFF,
	METHOD_CVODE,
	METHOD_DP5,
	METHOD_DP83,
	METHOD_RB23,
	METHOD_SYMPLECT,
	NUM_METHODS
};

struct xppMainSettings {
	bool quietMode	= false;
	bool useGradient= false;
	bool runNow		= false;
	bool simPlot	= false;
	bool is3D		= false;

	int seedRNG		= 0;
	int	numPlots	= 1;
	int numJump		= 1;
	int numMesh		= 40;
	int gridDF		= 10;
	int maxPointsVolt = 4000;

	double JAC_EPS  = 1E-5;
	double NEWT_TOL = 1E-5;
	double NEWT_ITER= 1E-5;

	double dt		= 0.05;
	double trans	= 0.0;

	double tMax     = 20.0;
	double tMin		= 0.0;
	double dtMax    = 20.0;
	double dtMin	= 0.0;

	std::string	logFile		= "";
	std::string outputFile  = "";
	std::string fontName	= "";
	std::string backImage	= "";

	xppMainColor color = xppMainColor();

	xppMethod method= METHOD_RK4;
};

#endif // XPPMAINDEFINES_H
