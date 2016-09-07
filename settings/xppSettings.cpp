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
    typedef std::pair<std::string, bool*>        bPair;
    typedef std::pair<std::string, double*>      dPair;
    typedef std::pair<std::string, std::string*> sPair;
    typedef std::pair<std::string, unsigned*>    uPair;

    std::map<std::string, bool*>        boolMap;
    std::map<std::string, double*>      doubleMap;
    std::map<std::string, std::string*> stringMap;
    std::map<std::string, unsigned*>    uintMap;

    boolMap.insert(bPair("QUIET",   &mainSettings.quietMode));
    boolMap.insert(bPair("GRADS",   &mainSettings.useGradient));
    boolMap.insert(bPair("RUNNOW",  &mainSettings.runNow));
    boolMap.insert(bPair("SIMPLOT", &mainSettings.simPlot));
    boolMap.insert(bPair("AXES",    &mainSettings.is3D));

    uintMap.insert(uPair("NTST",    &autoSettings.Ntst));
    uintMap.insert(uPair("NMAX",    &autoSettings.Nmax));
    uintMap.insert(uPair("NPR",     &autoSettings.NPr));
    uintMap.insert(uPair("NCOL",    &autoSettings.NCol));

    uintMap.insert(uPair("SEED",    &mainSettings.seedRNG));
    uintMap.insert(uPair("NPLOT",   &mainSettings.numPlots));
    uintMap.insert(uPair("NJMP",    &mainSettings.numJump));
    uintMap.insert(uPair("NMESH",   &mainSettings.numMesh));
    uintMap.insert(uPair("DFGRID",  &mainSettings.gridDF));
    uintMap.insert(uPair("SEED",    &mainSettings.seedRNG));
    uintMap.insert(uPair("MAXSTOR", &mainSettings.maxStore));

    uintMap.insert(uPair("VMAXPTS", &solverSettings.maxPoints));
    uintMap.insert(uPair("BANDUP",  &solverSettings.BANDUP));
    uintMap.insert(uPair("BANDLO",  &solverSettings.BANDLO));

    uintMap.insert(uPair("SEC", (unsigned*)&autoSettings.color.stableEquilibriumColor));
    uintMap.insert(uPair("SPC", (unsigned*)&autoSettings.color.stablePeriodicColor));
    uintMap.insert(uPair("SMC", (unsigned*)&mainSettings.color.stableManifoldColor));
    uintMap.insert(uPair("UEC", (unsigned*)&autoSettings.color.unstableEquilibriumColor));
    uintMap.insert(uPair("UPC", (unsigned*)&autoSettings.color.unstablePeriodicColor));
    uintMap.insert(uPair("UMC", (unsigned*)&mainSettings.color.unstableManifoldColor));
    uintMap.insert(uPair("XNC", (unsigned*)&mainSettings.color.xNullclineColor));
    uintMap.insert(uPair("YNC", (unsigned*)&mainSettings.color.yNullclineColor));
    uintMap.insert(uPair("METH",(unsigned*)&solverSettings.method));

    doubleMap.insert(dPair("EPSL",      &autoSettings.EPSL));
    doubleMap.insert(dPair("EPSU",      &autoSettings.EPSU));
    doubleMap.insert(dPair("EPSS",      &autoSettings.EPSS));
    doubleMap.insert(dPair("IAD",       &autoSettings.IAD));
    doubleMap.insert(dPair("MXBF",      &autoSettings.MXBF));
    doubleMap.insert(dPair("IID",       &autoSettings.IID));
    doubleMap.insert(dPair("ITMX",      &autoSettings.ITMX));
    doubleMap.insert(dPair("ITNW",      &autoSettings.ITNW));
    doubleMap.insert(dPair("IADS",      &autoSettings.IADS));
    doubleMap.insert(dPair("DS",        &autoSettings.Ds));
    doubleMap.insert(dPair("DSMAX",     &autoSettings.DsMax));
    doubleMap.insert(dPair("DSMIN",     &autoSettings.DsMin));
    doubleMap.insert(dPair("PARMAX",    &autoSettings.ParMax));
    doubleMap.insert(dPair("PARMIN",    &autoSettings.ParMin));
    doubleMap.insert(dPair("NORMMAX",   &autoSettings.NormMax));
    doubleMap.insert(dPair("NORMMIN",   &autoSettings.NormMin));
    doubleMap.insert(dPair("AUTOXMAX",  &autoSettings.xMax));
    doubleMap.insert(dPair("AUTOXMIN",  &autoSettings.xMin));
    doubleMap.insert(dPair("AUTOYMAX",  &autoSettings.yMax));
    doubleMap.insert(dPair("AUTOYMIN",  &autoSettings.yMin));

    doubleMap.insert(dPair("TRANS",     &mainSettings.trans));
    doubleMap.insert(dPair("T0",        &mainSettings.tStart));
    doubleMap.insert(dPair("TOTAL",     &mainSettings.tEnd));
    doubleMap.insert(dPair("BOUND",     &mainSettings.bound));
    doubleMap.insert(dPair("XHI",       &mainSettings.xMax));
    doubleMap.insert(dPair("XLO",       &mainSettings.xMin));
    doubleMap.insert(dPair("YHI",       &mainSettings.yMax));
    doubleMap.insert(dPair("YLO",       &mainSettings.yMin));

    doubleMap.insert(dPair("JAC_EPS",   &solverSettings.JAC_EPS));
    doubleMap.insert(dPair("NEWT_TOL",  &solverSettings.NEWT_TOL));
    doubleMap.insert(dPair("NEWT_ITER", &solverSettings.NEWT_ITER));
    doubleMap.insert(dPair("DT",        &solverSettings.dt));
    doubleMap.insert(dPair("DTMAX",     &solverSettings.dtMax));
    doubleMap.insert(dPair("DTMIN",     &solverSettings.dtMin));
    doubleMap.insert(dPair("DELAY",     &solverSettings.maxDelay));

    stringMap.insert(sPair("LOGFILE",   &mainSettings.logFile));
    stringMap.insert(sPair("OUTPUT",    &mainSettings.outputFile));

    for (const opts &opt : options) {
        if (boolMap.find(opt.Name) != boolMap.end()) {
            *boolMap.at(opt.Name) = (bool) std::stoi(opt.Expr);
        } else if (doubleMap.find(opt.Name) != doubleMap.end()) {
            *doubleMap.at(opt.Name) = std::stod(opt.Expr);
        } else if (uintMap.find(opt.Name) != uintMap.end()) {
            *uintMap.at(opt.Name) = (unsigned) std::stoi(opt.Expr);
        } else if (stringMap.find(opt.Name) != stringMap.end()) {
            *stringMap.at(opt.Name) = opt.Expr;
        } else {
            throw std::runtime_error("Unknown option\n");
        }
    }
}
