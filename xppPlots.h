#ifndef XPPPLOTS
#define XPPPLOTS

#include <string>
#include <vector>

#include <xppColor.h>

enum xppLineStyle {
	lsNone,
	lsLine,
	lsStepLeft,
	lsStepRight,
	lsStepCenter,
	lsImpulse
};

enum xppMarkerStyle {
	msNone,
	msDot,
	msCross,
	msPlus,
	msCircle,
	msDisc,
	msSquare,
	msDiamond,
	msStar,
	msTriangle,
	msTriangleInverted,
	msCrossSquare,
	msPlusSquare,
	msCrossCircle,
	msPlusCircle,
	msPeace
};

struct xppLineOptions {
	double			lineWidth		= 1.0;
	double			markerSize		= 1.0;

	xppLineStyle	lineStyle		= lsLine;
	xppMarkerStyle	markerStyle		= msCross;

	xppColor		lineColor		= BLACK;
	xppColor		markerColor		= BLACK;
};

struct xppPlotAxes {
	std::string xLabel = "time";
	std::string	yLabel = "";
	std::string	zLabel = "";

	bool xGrid		= false;
	bool xGridMinor	= false;
	bool yGrid		= false;
	bool yGridMinor	= false;
	bool zGrid		= false;
	bool zGridMinor	= false;
	bool useXOrigin	= false;
	bool useYOrigin	= false;
	bool useZOrigin	= false;

	double xOrigin  = 0.0;
	double yOrigin  = 0.0;
	double zOrigin  = 0.0;

	double xMax		= 20.0;
	double xMin		= 0.0;
	double yMax		= 20.0;
	double yMin		= 0.0;
	double zMax		= 20.0;
	double zMin		= 0.0;

	std::vector<xppLineOptions> lineOptions;
};



#endif // XPPPLOTS

