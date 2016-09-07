#ifndef XPPSOLVERSETTINGS_H
#define XPPSOLVERSETTINGS_H

#include <stdexcept>
#include <string>

/**
 * @brief The xppMethod enum defines the possible solvers for ODEs
 */
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

/**
 * @brief The xppSolverSettings class stores solver related settings
 */
class xppSolverSettings {
public:
    /**
     * @brief xppSolverSettings default contructor.
     */
    xppSolverSettings() {}



    /**
     * @brief xppParser::setMethod Translate the string key into a xppMethod enum.
     * @param key The option expression containing the key.
     * @return The xppMethod of the solver method.
     */
    void setMethod(const std::string& key) {
        if (key == "discrete") {
            method = METHOD_DISCRETE;
        } else if (key == "euler") {
            method = METHOD_EULER;
        } else if (key == "modeuler") {
            method = METHOD_MODEULER;
        } else if (key == "rungekutta") {
            method = METHOD_RK4;
        } else if (key == "adams") {
            method = METHOD_ADAMS;
        } else if (key == "gear") {
            method = METHOD_GEAR;
        } else if (key == "volterra") {
            method = METHOD_VOLTERRA;
        } else if (key == "backeul") {
            method = METHOD_BACKEUL;
        } else if (key == "qualrk") {
            method = METHOD_RKQS;
        } else if (key == "stiff") {
            method = METHOD_STIFF;
        } else if (key == "cvode") {
            method = METHOD_CVODE;
        } else if (key == "5dp") {
            method = METHOD_DP5;
        } else if (key == "83dp") {
            method = METHOD_DP83;
        } else if (key == "2rb") {
            method = METHOD_RB23;
        } else if (key == "symp") {
            method = METHOD_SYMPLECT;
        } else {
            throw std::runtime_error("Unknown solver method!\n");
        }
    }

private:
    double   JAC_EPS	= 1E-5;
    unsigned BANDUP		= 100;
    unsigned BANDLO		= 10;
    double	 NEWT_TOL	= 1E-5;
    double	 NEWT_ITER	= 1E-5;

    double	 ATOLER		= 1E-5;
    double	 TOLER		= 1E-5;

    unsigned maxPoints   = 4000;
    double	 maxDelay	= 0.0;

    double	 dt			= 0.05;
    double	 dtMax		= 20.0;
    double	 dtMin		= 0.0;

    xppMethod method	= METHOD_RK4;
    friend class xppSettings;
};

#endif // XPPSOLVERSETTINGS_H

