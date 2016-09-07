#include "xppSettings.h"

xppSettings::xppSettings(const optsArray &options)
{
    boolMap.insert(bPair("QUIET",   &mainSettings.quietMode));
    boolMap.insert(bPair("GRADS",   &mainSettings.useGradient));
    boolMap.insert(bPair("RUNNOW",  &mainSettings.runNow));
    boolMap.insert(bPair("SIMPLOT", &mainSettings.simPlot));
    boolMap.insert(bPair("AXES",    &mainSettings.is3D));

    uintMap.insert(uPair("NTST", &autoSettings.Ntst));
    uintMap.insert(uPair("NMAX", &autoSettings.Nmax));
    uintMap.insert(uPair("NPR",  &autoSettings.NPr));
    uintMap.insert(uPair("NCOL", &autoSettings.NCol));
    uintMap.insert(uPair("SEC", (unsigned*)&autoSettings.color.stableEquilibriumColor));
    uintMap.insert(uPair("UEC", (unsigned*)&autoSettings.color.unstableEquilibriumColor));
    uintMap.insert(uPair("SPC", (unsigned*)&autoSettings.color.stablePeriodicColor));
    uintMap.insert(uPair("UPC", (unsigned*)&autoSettings.color.unstablePeriodicColor));

    uintMap.insert(uPair("SEED",    &mainSettings.seedRNG));
    uintMap.insert(uPair("NPLOT",   &mainSettings.numPlots));
    uintMap.insert(uPair("NJMP",    &mainSettings.numJump));
    uintMap.insert(uPair("NMESH",   &mainSettings.numMesh));
    uintMap.insert(uPair("DFGRID",  &mainSettings.gridDF));
    uintMap.insert(uPair("SEED",    &mainSettings.seedRNG));
    uintMap.insert(uPair("MAXSTOR", &mainSettings.maxStore));
    uintMap.insert(uPair("SMC", (unsigned*)&mainSettings.color.stableManifoldColor));
    uintMap.insert(uPair("UMC", (unsigned*)&mainSettings.color.unstableManifoldColor));
    uintMap.insert(uPair("XNC", (unsigned*)&mainSettings.color.xNullclineColor));
    uintMap.insert(uPair("YNC", (unsigned*)&mainSettings.color.yNullclineColor));

    uintMap.insert(uPair("VMAXPTS",  &solverSettings.maxPoints));
    uintMap.insert(uPair("BANDUP",  &solverSettings.BANDUP));
    uintMap.insert(uPair("BANDLO",  &solverSettings.BANDLO));
    uintMap.insert(uPair("METHOD",  (unsigned*)&solverSettings.method));

    doubleMap.insert(dPair("EPSL",  &autoSettings.EPSL));
    doubleMap.insert(dPair("EPSU",  &autoSettings.EPSU));
    doubleMap.insert(dPair("EPSS",  &autoSettings.EPSS));
    doubleMap.insert(dPair("IAD",   &autoSettings.IAD));
    doubleMap.insert(dPair("MXBF",  &autoSettings.MXBF));
    doubleMap.insert(dPair("IID",   &autoSettings.IID));
    doubleMap.insert(dPair("ITMX",  &autoSettings.ITMX));
    doubleMap.insert(dPair("ITNW",  &autoSettings.ITNW));
    doubleMap.insert(dPair("IADS",  &autoSettings.IADS));
    doubleMap.insert(dPair("DS",    &autoSettings.Ds));
    doubleMap.insert(dPair("DSMAX", &autoSettings.DsMax));
    doubleMap.insert(dPair("DSMIN", &autoSettings.DsMin));
    doubleMap.insert(dPair("PARMAX",&autoSettings.ParMax));
    doubleMap.insert(dPair("PARMIN",&autoSettings.ParMin));
    doubleMap.insert(dPair("NORMMAX",&autoSettings.NormMax));
    doubleMap.insert(dPair("NORMMIN",&autoSettings.NormMin));

    doubleMap.insert(dPair("TRANS", &mainSettings.trans));
    doubleMap.insert(dPair("T0",    &mainSettings.tStart));

    doubleMap.insert(dPair("JAC_EPS",   &solverSettings.JAC_EPS));
    doubleMap.insert(dPair("NEWT_TOL",  &solverSettings.NEWT_TOL));
    doubleMap.insert(dPair("NEWT_ITER", &solverSettings.NEWT_ITER));
    doubleMap.insert(dPair("DT",        &solverSettings.dt));
    doubleMap.insert(dPair("DTMAX",     &solverSettings.dtMax));
    doubleMap.insert(dPair("DTMIN",     &solverSettings.dtMin));
    doubleMap.insert(dPair("DELAY",     &solverSettings.maxDelay));

    stringMap.insert(sPair("LOGFILE", &mainSettings.logFile));
    stringMap.insert(sPair("OUTPUT", &mainSettings.outputFile));

    for (const opts &opt : options) {
        setOption(opt);
    }
}
/**
 * @brief getOption Set the option defined in the opts structure
 * @param opt The opts structure containing the information.
 */
template<typename T>
T xppSettings::getOption(const std::string &key) {
    if (boolMap.find(key) != boolMap.end()) {
        return *boolMap.at(key);
    } else if (doubleMap.find(key) != doubleMap.end()) {
        return *doubleMap.at(key);
    } else if (uintMap.find(key) != uintMap.end()) {
        return *uintMap.at(key);
    } else if (stringMap.find(key) != stringMap.end()) {
        return *stringMap.at(key);
    } else {
        throw std::runtime_error("Unknown option\n");
    }
}

/**
 * @brief setOption Set the option defined in the opts structure
 * @param opt The opts structure containing the information.
 */
void xppSettings::setOption(const opts &opt) {
    if (boolMap.find(opt.Name) != boolMap.end()) {
        *boolMap.at(opt.Name) = (bool) std::stoi(opt.Expr);
    } else if (doubleMap.find(opt.Name) != doubleMap.end()) {
        *doubleMap.at(opt.Name) = std::stod(opt.Expr);
    } else if (uintMap.find(opt.Name) != uintMap.end()) {
        *uintMap.at(opt.Name) = (unsigned) std::stoi(opt.Expr);
    } else if (stringMap.find(opt.Name) != stringMap.end()) {
        *stringMap.at(opt.Name) = opt.Expr;
    } else {
        std::cout << "Name: " << opt.Name << std::endl;
        throw std::runtime_error("Unknown option\n");
    }
}
