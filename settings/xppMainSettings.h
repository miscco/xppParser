#ifndef XPPMAINSETTINGS_H
#define XPPMAINSETTINGS_H

#include <string>
#include <vector>

#include "xppColor.h"

class xppMainSettings {
public:
    /**
     * @brief xppMainSettings Default constructor.
     */
    xppMainSettings() {}

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
            } else if (key == "SMC") {
                color.stableManifoldColor = (xppColor) std::stoi(value.c_str());
            } else if (key == "UMC") {
                color.unstableManifoldColor = (xppColor) std::stoi(value.c_str());
            } else if (key == "XNC") {
                color.xNullclineColor = (xppColor) std::stoi(value.c_str());
            } else if (key == "YNC") {
                color.xNullclineColor = (xppColor) std::stoi(value.c_str());
            } else if (key == "COLORMAP") {
                color.colorMap = (xppColorMap) std::stoi(value.c_str());
            }
        } catch (const std::invalid_argument& ia) {
            //std::cerr << "Invalid argument: " << ia.what() << '\n';
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
        } else if (key == "SMC") {
            return color.stableManifoldColor;
        } else if (key == "UMC") {
            return color.unstableManifoldColor;
        } else if (key == "XNC") {
            return color.xNullclineColor;
        } else if (key == "YNC") {
            return color.xNullclineColor;
        } else if (key == "COLORMAP") {
            return color.colorMap;
        }
    }

private:
    bool quietMode	= false;
    bool useGradient= false;
    bool runNow		= false;
    bool simPlot	= false;
    bool is3D		= false;

    unsigned seedRNG= 0;
    unsigned numPlots= 1;
    unsigned numJump= 1;
    unsigned numMesh= 40;
    unsigned gridDF	= 10;

    double	tStart	= 0.0;
    double	trans	= 0.0;

    std::string	logFile		= "";
    std::string outputFile  = "";

    xppMainColor color = xppMainColor();
    friend class xppSettings;
};

#endif // XPPMAINSETTINGS_H
