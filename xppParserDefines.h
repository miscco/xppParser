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
typedef std::pair<std::string, int> lineNumber;

static const std::set<std::string> xppKeywords = {
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

static const std::set<std::string> xppOperators {
	"+",
	"-",
	"*",
	"/",
	"^",
	"**"
};

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

static const std::set<std::string> xppOptionNames {
	"MAXSTOR",
	"BACK",
	"SMALL",
	"BIG",
	"SMC",
	"UMC",
	"XNC",
	"YNC",
	"OUTPUT",
	"BUT",
	"LT",
	"XP",
	"YP",
	"ZP",
	"NPLOT",
	"XP2",
	"YP2",
	"ZP2",
	"AXES",
	"PHI",
	"THETA",
	"XLO",
	"YLO",
	"XHI",
	"YHI",
	"XMAX",
	"XMIN",
	"YMAX",
	"YMIN",
	"ZMAX",
	"ZMIN",
	"SEED",
	"TOTAL",
	"DT",
	"NJMP",
	"NOUT",
	"T0",
	"TRANS",
	"NMESH",
	"BANDUP",
	"BANDLO",
	"METH",
	"DTMIN",
	"DTMAX",
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
	"BELL",
	"COLORMAP"
};

#endif // XPPPARSERDEFINES_H

