#ifndef XPPPARSERDEFINES_H
#define XPPPARSERDEFINES_H

#include <set>
#include <string>
#include <utility>
#include <vector>

/* Basic structure that contains the textual information of an expression*/
struct opts {
    unsigned					Line;
    std::string					Name;
    std::string					Expr;
    std::vector<std::string>	Args;

    explicit opts () {}
    explicit opts (const unsigned line) : Line(line) {}
    explicit opts (const opts &opt)
        : Line(opt.Line), Name(opt.Name), Expr(opt.Expr), Args(opt.Args) {}
};

/* Array of opts structures */
typedef std::vector<opts> optsArray;

/* Array of strings */
typedef std::vector<std::string> stringList;

/* Pair containing a parsed line and the original line number in the ode file.
 * This is mainly usefull for debugging if an error is thrown.
 */
typedef std::pair<std::string, unsigned> lineNumber;

/* Vector containing the keywords utilized by xppaut. We cannot use a set here,
 * as that would change the ordering of the symbols
 */
static const std::vector<std::string> xppKeywords = {
    "!",
    "(t+1)",
    "'",
    "/dt",
    "(t)",
    "volt",
    // "markov", /* Handled separately */
    "aux",
    "par",
    "number",
    "(",
    //"table",  /* This is handeled separately */
    //"wiener", /* Handled separately */
    //"global",
    "init",
    "(0)",
    "bdry",
    "0=",
    "solve",
    "special",
    "set",
    "@",
    "export"
};

/* Set of the valid mathematical operators */
static const std::set<std::string> xppOperators {
    "+",
    "-",
    "*",
    "/",
    "^",
    "**"
};

/* Set of the reserved function names */
static const std::set<std::string> xppReservedNames {
    "sin",
    "cos",
    "tan",
    "atan",
    "atan2",
    "sinh",
    "cosh",
    "tanh",
    "exp",
    "delay",
    "ln",
    "log",
    "log10",
    "t",
    "pi",
    "if",
    "then",
    "else",
    "asin",
    "acos",
    "heav",
    "sign",
    "mod",
    "flr",
    "ran",
    "abs",
    "del_shft",
    "max",
    "min",
    "normal",
    "besselj",
    "bessely",
    "besseli",
    "erf",
    "erfc",
    "poisson",
    "lgamma",
    "shift",
    "sum",
    "int",
    "not"
};

/* Set of the options that can be modified in an ode file */
static const std::set<std::string> xppOptionNames {
    "QUIET",
    "LOGFILE",
    "OUTPUT",
    "MAXSTOR",
    "SMC",
    "UMC",
    "XNC",
    "YNC",
    "SEC",
    "UEC",
    "SPC",
    "UPC",
    "COLORMAP",
    "COLORIZE",
    "COLORVIA",
    "COLORLO",
    "COLORHI",
    "LT",
    "NPLOT",
    "MULTIWIN",
    "SIMPLOT",
    "XP",
    "YP",
    "ZP",
    "XP2",
    "YP2",
    "ZP2",
    "XP3",
    "YP3",
    "ZP3",
    "XP4",
    "YP4",
    "ZP4",
    "XP5",
    "YP5",
    "ZP5",
    "XP6",
    "YP6",
    "ZP6",
    "XP7",
    "YP7",
    "ZP7",
    "XP8",
    "YP8",
    "ZP8",
    "XLO",
    "YLO",
    "XHI",
    "YHI",
    "XLO2",
    "YLO2",
    "XHI2",
    "YHI2",
    "XLO3",
    "YLO3",
    "XHI3",
    "YHI3",
    "XLO4",
    "YLO4",
    "XHI4",
    "YHI4",
    "XLO5",
    "YLO5",
    "XHI5",
    "YHI5",
    "XLO6",
    "YLO6",
    "XHI6",
    "YHI6",
    "XLO7",
    "YLO7",
    "XHI7",
    "YHI7",
    "XLO8",
    "YLO8",
    "XHI8",
    "YHI8",
    "XMAX",
    "XMIN",
    "YMAX",
    "YMIN",
    "ZMAX",
    "ZMIN",
    "AXES",
    "PHI",
    "THETA",
    "SEED",
    "TOTAL",
    "DT",
    "DTMIN",
    "DTMAX",
    "NJMP",
    "NOUT",
    "T0",
    "TRANS",
    "NMESH",
    "DFGRID",
    "METH",
    "BANDUP",
    "BANDLO",
    "VMAXPTS",
    "JAC_EPS",
    "NEWT_TOL",
    "NEWT_ITER",
    "ATOLER",
    "TOLER",
    "BOUND",
    "DELAY",
    "AUTOEVAL",
    "POIMAP",
    "POIVAR",
    "POIPLN",
    "POISGN",
    "POISTOP",
    "RANGE",
    "RANGEOVER",
    "RANGESTEP",
    "RANGELOW",
    "RANGEHIGH",
    "RANGERESET",
    "RANGEOLDIC",
    "TOR_PER",
    "FOLD",
    "NTST",
    "NMAX",
    "NPR",
    "DSMIN",
    "DSMAX",
    "DS",
    "EPSS",
    "EPSL",
    "EPSU",
    "PARMIN",
    "PARMAX",
    "NORMMIN",
    "NORMMAX",
    "AUTOXMIN",
    "AUTOXMAX",
    "AUTOYMIN",
    "AUTOYMAX",
    "AUTOVAR",
    "BELL"
};

#endif // XPPPARSERDEFINES_H

