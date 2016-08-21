#ifndef KEYWORDTRIE_H
#define KEYWORDTRIE_H

#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

struct node {
  int id		= -1;			/* Keyword id */
  int depth		= 0;			/* Depth in the trie*/
  char c		= '\0';			/* Character labelling incoming edge */
  node *parent  = nullptr;		/* Parent node */
  node *failure = nullptr;		/* Failure link */
  std::vector<node*> children;	/* Child nodes */
  explicit node (int d, char character, node *par, node *root)
	  : depth(d), c(character), parent(par), failure(root) {}
  explicit node () {}

};

struct result {
	std::string keyword;
	int			keyID;
	int			position;

	explicit result (std::string &key, int id, int pos)
		:keyword(key), keyID(id), position(pos) {}
};

class keywordTrie
{
public:
	keywordTrie();

	void addString	(std::string &key);

	std::vector<result> parseText(std::string &text);

private:
	node *root;
	std::vector<node>		 trieNodes;
	std::vector<std::string> keywords;

	void addChild		(node *current, char &character);
	void findChild		(node *current, char &character, bool addWord);
	void addFailureLinks();
};

#endif // KEYWORDTRIE_H
