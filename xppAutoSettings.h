#ifndef XPPAUTOSETTINGS_H
#define XPPAUTOSETTINGS_H

#include <string>
#include <vector>

#include "xppColor.h"
#include "parser/xppParser.h"

class xppAutoSettings {
public:
    /**
     * @brief xppMainSettings Default constructor taking the parsed information.
     * @param parser The parser object containing the non default options.
     */
    xppAutoSettings(const xppParser &parser) {setOptions(parser.Options);}

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
            if (key == "NTST") {
                Ntst = std::stoi(value.c_str());
            } else if (key == "NMAX") {
                Nmax = std::stoi(value.c_str());
            } else if (key == "NPR") {
                NPr = std::stoi(value.c_str());
            } else if (key == "NCOL") {
                NCol = std::stoi(value.c_str());
            } else if (key == "EPSL") {
                EPSL = std::stod(value.c_str());
            } else if (key == "EPSU") {
                EPSU = std::stod(value.c_str());
            } else if (key == "EPSS") {
                EPSS = std::stod(value.c_str());
            } else if (key == "IAD") {
                IAD = std::stod(value.c_str());
            } else if (key == "MXBF") {
                MXBF = std::stod(value.c_str());
            } else if (key == "IID") {
                IID = std::stod(value.c_str());
            } else if (key == "ITMX") {
                ITMX = std::stod(value.c_str());
            } else if (key == "ITNM") {
                ITNW = std::stod(value.c_str());
            } else if (key == "NWTN") {
                NWTN = std::stod(value.c_str());
            } else if (key == "IADS") {
                IADS = std::stod(value.c_str());
            } else if (key == "DS") {
                Ds = std::stod(value.c_str());
            } else if (key == "DSMAX") {
                DsMax = std::stod(value.c_str());
            } else if (key == "DSMIN") {
                DsMin = std::stod(value.c_str());
            } else if (key == "PARMAX") {
                ParMax = std::stod(value.c_str());
            } else if (key == "PARMIN") {
                ParMin = std::stod(value.c_str());
            } else if (key == "NORMMAX") {
                NormMax = std::stod(value.c_str());
            } else if (key == "NORMIN") {
                NormMin = std::stod(value.c_str());
            } else if (key == "SEC") {
                color.stableEquilibriumColor = std::stoi(value.c_str());
            } else if (key == "UEC") {
                color.unstableEquilibriumColor = std::stoi(value.c_str());
            } else if (key == "SPC") {
                color.stablePeriodicColor = std::stoi(value.c_str());
            } else if (key == "UPC") {
                color.unstablePeriodicColor = std::stoi(value.c_str());
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
        if (key == "NTST") {
            return Ntst;
        } else if (key == "NMAX") {
            return Nmax;
        } else if (key == "NPR") {
            return NPr;
        } else if (key == "NCOL") {
            return NCol;
        } else if (key == "EPSL") {
            return EPSL;
        } else if (key == "EPSU") {
            return EPSU;
        } else if (key == "EPSS") {
            return EPSS;
        } else if (key == "IAD") {
            return IAD;
        } else if (key == "MXBF") {
            return MXBF;
        } else if (key == "IID") {
            return IID;
        } else if (key == "ITMX") {
            return ITMX;
        } else if (key == "ITNM") {
            return ITNW;
        } else if (key == "NWTN") {
            return NWTN;
        } else if (key == "IADS") {
            return IADS;
        } else if (key == "DS") {
            return Ds;
        } else if (key == "DSMAX") {
            return DsMax;
        } else if (key == "DSMIN") {
            return DsMin;
        } else if (key == "PARMAX") {
            return ParMax;
        } else if (key == "PARMIN") {
            return ParMin;
        } else if (key == "NORMMAX") {
            return NormMax;
        } else if (key == "NORMIN") {
            return NormMin;
        } else if (key == "SEC") {
            return color.stableEquilibriumColor;
        } else if (key == "UEC") {
            return color.unstableEquilibriumColor;
        } else if (key == "SPC") {
            return color.stablePeriodicColor;
        } else if (key == "UPC") {
            return color.unstablePeriodicColor;
        }
    }
private:
    unsigned Ntst	= 50;
    unsigned Nmax	= 300;
    unsigned NPr	= 50;
    unsigned NCol	= 4;

    double	EPSL	= 1E-7;
    double	EPSU	= 1E-7;
    double	EPSS	= 1E-7;

    double	IAD		= 3;
    double	MXBF	= 5;
    double	IID		= 2;
    double	ITMX	= 8;
    double	ITNW	= 7;
    double	NWTN	= 3;
    double	IADS	= 1;

    double	Ds		= 1E-3;
    double  DsMax	= 0.5;
    double	DsMin	= 1E-5;

    double  ParMax	= 10.0;
    double	ParMin	= 0.0;
    double  NormMax	= 1000.0;
    double	NormMin	= 0.0;

    xppAutoColor color = xppAutoColor();
};

#endif // XPPAUTOSETTINGS_H
