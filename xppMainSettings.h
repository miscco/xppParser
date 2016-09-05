#ifndef XPPMAINSETTINGS_H
#define XPPMAINSETTINGS_H

#include <string>
#include <vector>

#include "xppColor.h"
#include "xppSolver.h"
#include "parser/xppParser.h"


class xppMainSettings {
public:
	xppMainSettings(const xppParser &parser);

	void setOption (const std::string &key, const std::string &value);
	template<typename T>
	T getOption (const std::string &key) const;

private:
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

	double	tStart	= 0.0;
	double	trans	= 0.0;

	std::string	logFile		= "";
	std::string outputFile  = "";

	xppMainColor color = xppMainColor();

	void setOptions		(const optsArray &options);
};

#endif // XPPMAINSETTINGS_H
