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

