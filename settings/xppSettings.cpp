#include "xppSettings.h"

/**
 * @brief xppSettings::xppSettings Default constructor of the settings class.
 * @param options Parsed information from the ode file about nondefault settings.
 */
xppSettings::xppSettings(const optsArray &options)
{
    initializeOptions(options);
}

/**
 * @brief initializeOptions Set non default options specified in the ode file.
 * @param options Parsed information from the ode file about nondefault settings.
 */
void xppSettings::initializeOptions(const optsArray& options) {
    for (const opts &opt : options) {
        if (opt.Name == "QUIET") {
            mainSettings.quietMode = (bool) std::stoi(opt.Expr);
        } else if (opt.Name == "GRADS") {
            mainSettings.useGradient = (bool) std::stoi(opt.Expr);
        } else if (opt.Name == "RUNNOW") {
            mainSettings.runNow = (bool) std::stoi(opt.Expr);
        } else if (opt.Name == "SIMPLOT") {
            mainSettings.simPlot = (bool) std::stoi(opt.Expr);
        } else if (opt.Name == "AXES") {
            mainSettings.is3D = (bool) std::stoi(opt.Expr);
        } else if (opt.Name == "NTST") {
            autoSettings.Ntst = std::stoi(opt.Expr);
        } else if (opt.Name == "NMAX") {
            autoSettings.Nmax = std::stoi(opt.Expr);
        } else if (opt.Name == "NPR") {
            autoSettings.NPr = std::stoi(opt.Expr);
        } else if (opt.Name == "NCOL") {
            autoSettings.NCol = std::stoi(opt.Expr);
        } else if (opt.Name == "SEED") {
            mainSettings.seedRNG = std::stoi(opt.Expr);
        } else if (opt.Name == "NPLOT") {
            mainSettings.numPlots = std::stoi(opt.Expr);
        } else if (opt.Name == "NJMP") {
            mainSettings.numJump = std::stoi(opt.Expr);
        } else if (opt.Name == "NMESH") {
            mainSettings.numMesh = std::stoi(opt.Expr);
        } else if (opt.Name == "DFGRID") {
            mainSettings.gridDF = std::stoi(opt.Expr);
        } else if (opt.Name == "MAXSTOR") {
            mainSettings.maxStore = std::stoi(opt.Expr);
        } else if (opt.Name == "VMAXPTS") {
            solverSettings.maxPoints = std::stoi(opt.Expr);
        } else if (opt.Name == "BANDUP") {
            solverSettings.BANDUP = std::stoi(opt.Expr);
        } else if (opt.Name == "BANDLO") {
            solverSettings.BANDLO = std::stoi(opt.Expr);
        } else if (opt.Name == "SEC") {
            autoSettings.color.stableEquilibriumColor = xppColor(std::stoi(opt.Expr));
        } else if (opt.Name == "SPC") {
            autoSettings.color.stablePeriodicColor = xppColor(std::stoi(opt.Expr));
        } else if (opt.Name == "SMC") {
            mainSettings.color.stableManifoldColor = xppColor(std::stoi(opt.Expr));
        } else if (opt.Name == "UEC") {
            autoSettings.color.unstableEquilibriumColor = xppColor(std::stoi(opt.Expr));
        } else if (opt.Name == "UPC") {
            autoSettings.color.unstablePeriodicColor = xppColor(std::stoi(opt.Expr));
        } else if (opt.Name == "UMC") {
            mainSettings.color.unstableManifoldColor = xppColor(std::stoi(opt.Expr));
        } else if (opt.Name == "XNC") {
            mainSettings.color.xNullclineColor = xppColor(std::stoi(opt.Expr));
        } else if (opt.Name == "YNC") {
            mainSettings.color.yNullclineColor = xppColor(std::stoi(opt.Expr));
        } else if (opt.Name == "METH") {
            solverSettings.setMethod(opt.Expr);
        } else if (opt.Name == "EPSL") {
            autoSettings.EPSL = std::stod(opt.Expr);
        } else if (opt.Name == "EPSU") {
            autoSettings.EPSU = std::stod(opt.Expr);
        } else if (opt.Name == "EPSS") {
            autoSettings.EPSS = std::stod(opt.Expr);
        } else if (opt.Name == "IAD") {
            autoSettings.IAD = std::stod(opt.Expr);
        } else if (opt.Name == "IADS") {
            autoSettings.IADS = std::stod(opt.Expr);
        } else if (opt.Name == "IID") {
            autoSettings.IID = std::stod(opt.Expr);
        } else if (opt.Name == "ITMX") {
            autoSettings.ITMX = std::stod(opt.Expr);
        } else if (opt.Name == "ITNW") {
            autoSettings.ITNW = std::stod(opt.Expr);
        } else if (opt.Name == "MXBF") {
            autoSettings.MXBF = std::stod(opt.Expr);
        } else if (opt.Name == "DS") {
            autoSettings.Ds = std::stod(opt.Expr);
        } else if (opt.Name == "DSMAX") {
            autoSettings.DsMax = std::stod(opt.Expr);
        } else if (opt.Name == "DSMIN") {
            autoSettings.DsMin = std::stod(opt.Expr);
        } else if (opt.Name == "PARMAX") {
            autoSettings.ParMax = std::stod(opt.Expr);
        } else if (opt.Name == "PARMIN") {
            autoSettings.ParMin = std::stod(opt.Expr);
        } else if (opt.Name == "NORMMAX") {
            autoSettings.NormMax = std::stod(opt.Expr);
        } else if (opt.Name == "NORMMIN") {
            autoSettings.NormMin = std::stod(opt.Expr);
        } else if (opt.Name == "AUTOXMAX") {
            autoSettings.axes.xMax = std::stod(opt.Expr);
        } else if (opt.Name == "AUTOXMIN") {
            autoSettings.axes.xMin = std::stod(opt.Expr);
        } else if (opt.Name == "AUTOYMAX") {
            autoSettings.axes.yMax = std::stod(opt.Expr);
        } else if (opt.Name == "AUTOYMIN") {
            autoSettings.axes.yMin = std::stod(opt.Expr);
        } else if (opt.Name == "TRANS") {
            mainSettings.trans = std::stod(opt.Expr);
        } else if (opt.Name == "T0") {
            mainSettings.tStart = std::stod(opt.Expr);
        } else if (opt.Name == "TOTAL") {
            mainSettings.tEnd = std::stod(opt.Expr);
        } else if (opt.Name == "BOUND") {
            mainSettings.bound = std::stod(opt.Expr);
        } else if (opt.Name == "XHI") {
            mainSettings.axes.at(0).xMax = std::stod(opt.Expr);
        } else if (opt.Name == "XLO") {
            mainSettings.axes.at(0).xMin = std::stod(opt.Expr);
        } else if (opt.Name == "YHI") {
            mainSettings.axes.at(0).yMax = std::stod(opt.Expr);
        } else if (opt.Name == "YLO") {
            mainSettings.axes.at(0).yMin = std::stod(opt.Expr);
        } else if (opt.Name == "JAC_EPS") {
            solverSettings.JAC_EPS = std::stod(opt.Expr);
        } else if (opt.Name == "NEWT_TOL") {
            solverSettings.NEWT_TOL = std::stod(opt.Expr);
        } else if (opt.Name == "NEWT_ITER") {
            solverSettings.NEWT_ITER = std::stod(opt.Expr);
        } else if (opt.Name == "DT") {
            solverSettings.dt = std::stod(opt.Expr);
        } else if (opt.Name == "DTMAX") {
            solverSettings.dtMax = std::stod(opt.Expr);
        } else if (opt.Name == "DTMIN") {
            solverSettings.dtMin = std::stod(opt.Expr);
        } else if (opt.Name == "DELAY") {
            solverSettings.maxDelay = std::stod(opt.Expr);
        } else if (opt.Name == "LOGFILE") {
            mainSettings.logFile = opt.Expr;
        } else if (opt.Name == "OUTPUT") {
            mainSettings.outputFile = opt.Expr;
        } else {
            throw std::runtime_error("Unknown option\n");
        }
    }
}
