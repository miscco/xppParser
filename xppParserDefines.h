#ifndef XPPPARSERDEFINES_H
#define XPPPARSERDEFINES_H

#include <string>
#include <utility>
#include <vector>

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
	"(",		/* Handled separately to discriminate odes and volterra */
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

#endif // XPPPARSERDEFINES_H

