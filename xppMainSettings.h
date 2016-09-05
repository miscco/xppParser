#ifndef XPPMAINSETTINGS_H
#define XPPMAINSETTINGS_H

#include <string>
#include <vector>

#include "xppColor.h"
#include "xppSolver.h"
#include "parser/xppParser.h"


class xppMainSettings {
public:
    /**
     * @brief xppMainSettings Default constructor taking the parsed information.
     * @param parser The parser object containing the non default options.
     */
    xppMainSettings(const xppParser &parser) {setOptions(parser.Options);}

    /**
     * @brief setOptions Setter of the non default options defined in optsArray.
     * @param options The opts array containing the parsed information.
     */
    void setOptions (const optsArray &options) {
        for (const opts &opt : options) {
            setOption(opt.Name, opt.Expr);
        }
    }

    /**
     * @brief setOption sets option given by key.
     * @param key The string identifying the option to be set.
     * @param value The value option "key" is set to.
     */
    void setOption (const std::string &key, const std::string &value) {
        try {
            if (key == "QUIET") {
                quietMode = std::stoi(value.c_str());
            } else if (key == "GRADS") {
                useGradient = std::stoi(value.c_str());
            } else if (key == "RUNNOW") {
                runNow = std::stoi(value.c_str());
            } else if (key == "SIMPLOT") {
                simPlot = std::stoi(value.c_str());
            } else if (key == "AXES") {
                is3D = std::stoi(value.c_str()) == 3 ? true : false;
            } else if (key == "SEED") {
                seedRNG = std::stoi(value.c_str());
            } else if (key == "NPLOT") {
                numPlots = std::stoi(value.c_str());
            } else if (key == "NJMP") {
                numJump = std::stoi(value.c_str());
            } else if (key == "NMESH") {
                numMesh = std::stoi(value.c_str());
            } else if (key == "DFGRID") {
                gridDF = std::stoi(value.c_str());
            } else if (key == "TRANS") {
                trans = std::stod(value.c_str());
            } else if (key == "T0") {
                tStart = std::stod(value.c_str());
            }
        } catch (const std::invalid_argument& ia) {
            std::cerr << "Invalid argument: " << ia.what() << '\n';
        }
    }

    /**
     * @brief getOption Getter of the option identified by key.
     * @param key The string identifying the option to be returned.
     * @return The value of the option identified by key.
     */
    template<typename T>
    T getOption (const std::string &key) const {
        if (key == "QUIET") {
            return quietMode;
        } else if (key == "GRADS") {
            return useGradient;
        } else if (key == "RUNNOW") {
            return runNow;
        } else if (key == "SIMPLOT") {
            return simPlot;
        } else if (key == "AXES") {
            return is3D;
        } else if (key == "SEED") {
            return seedRNG;
        } else if (key == "NPLOT") {
            return numPlots;
        } else if (key == "NJMP") {
            return numJump;
        } else if (key == "NMESH") {
            return numMesh;
        } else if (key == "DFGRID") {
            return gridDF;
        } else if (key == "TRANS") {
            return trans;
        } else if (key == "T0") {
            return tStart;
        }
    }

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
};

#endif // XPPMAINSETTINGS_H
