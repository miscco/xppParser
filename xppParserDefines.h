#ifndef XPPPARSERDEFINES_H
#define XPPPARSERDEFINES_H

#include <string>
#include <utility>
#include <vector>

#include "aho_corasick/aho_corasick.hpp"


/* Basic structure that contains the textual information of an expression*/
typedef struct {
	unsigned					Line;
	std::string					Name;
	std::string					Expr;
	std::vector<std::string>	Args;
} opts;

/* Array of opts structures */
typedef std::vector<opts> optsArray;

/* Array of strings */
typedef std::vector<std::string> stringList;

/* Array of emit_collections for function tables */
typedef std::vector<aho_corasick::trie::emit_collection> functionTable;

/* Pair containing a parsed line and the original line number in the ode file.
 * This is mainly usefull for debugging if an error is thrown.
 */
typedef std::pair<std::string, int> lineNumber;

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

#endif // XPPPARSERDEFINES_H

