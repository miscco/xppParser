#ifndef XPPSOLVER_H
#define XPPSOLVER_H

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
     * @brief xppSolverSettings::setOption sets option given by key.
     * @param key The string identifying the option to be set.
     * @param value The new value of the option identified by key.
     */
    void setOption (const std::string &key, const std::string &value) {
        try {
            if (key == "VMAXPTS") {
                maxPoints = std::stoi(value.c_str());
            } else if (key == "JAC_EPS") {
                BANDUP = std::stoi(value.c_str());
            } else if (key == "JAC_EPS") {
                BANDLO = std::stoi(value.c_str());
            } else if (key == "JAC_EPS") {
                JAC_EPS = std::stod(value.c_str());
            } else if (key == "NEWT_TOL") {
                NEWT_TOL = std::stod(value.c_str());
            } else if (key == "NEWT_ITER") {
                NEWT_ITER = std::stod(value.c_str());
            } else if (key == "DT") {
                dt = std::stod(value.c_str());
            } else if (key == "DTMAX") {
                dtMax = std::stod(value.c_str());
            } else if (key == "DTMIN") {
                dtMin = std::stod(value.c_str());
            } else if (key == "DELAY") {
                maxDelay = std::stod(value.c_str());
            }
        } catch (const std::invalid_argument& ia) {
            //std::cerr << "Invalid argument: " << ia.what() << '\n';
        }
    }

    /**
     * @brief xppSolverSettings::getOption return the value of given option.
     * @param key The string identifying the option to be returned.
     * @return The value of the option identified by key.
     */
    template<typename T>
    T getOption (const std::string &key) const {
        if (key == "VMAXPTS") {
            return maxPoints;
        } else if (key == "JAC_EPS") {
            return BANDUP;
        } else if (key == "JAC_EPS") {
            return BANDLO;
        } else if (key == "JAC_EPS") {
            return JAC_EPS;
        } else if (key == "NEWT_TOL") {
            return NEWT_TOL;
        } else if (key == "NEWT_ITER") {
            return NEWT_ITER;
        } else if (key == "DT") {
            return dt;
        } else if (key == "DTMAX") {
            return dtMax;
        } else if (key == "DTMIN") {
            return dtMin;
        } else if (key == "DELAY") {
            return maxDelay;
        }
    }

    /**
     * @brief setMethod Sets the solver method to meth.
     * @param meth The new solver method.
     */
    void setMethod (const xppMethod &meth) {method = meth;}

    /**
     * @brief getMethod getter for the solver method.
     * @return The current sovler method.
     */
    xppMethod getMethod (void) const {return method;}

private:
    double	JAC_EPS		= 1E-5;
    int		BANDUP		= 100;
    int		BANDLO		= 10;
    double	NEWT_TOL	= 1E-5;
    double	NEWT_ITER	= 1E-5;

    double	ATOLER		= 1E-5;
    double	TOLER		= 1E-5;

    int		maxPoints   = 4000;
    double	maxDelay	= 0.0;

    double	dt			= 0.05;
    double	dtMax		= 20.0;
    double	dtMin		= 0.0;

    xppMethod method	= METHOD_RK4;

};

#endif // XPPSOLVER_H

