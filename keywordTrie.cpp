#include "keywordTrie.h"
/**
 * @brief keywordTrie::keywordTrie Initialize the trie structure with its root
 * node.
 */
keywordTrie::keywordTrie()
{
	trieNodes.push_back(node());
	root = &trieNodes.at(0);
	root->failure = root;
}

/**
 * @brief keywordTrie::addFailureLinks Utilize a breadth first search to get the
 * failure links.
 */
void keywordTrie::addFailureLinks() {
	std::queue<node*> q;
	node *temp;

	q.push(root);

	while (!q.empty()) {
		temp = q.front();
		for (node *child : temp->children) {
			q.push(child);
		}
		/* A failure link with just one less charater is the optimum and will
		 * never change.
		 */
		if (temp->failure->depth < temp->depth - 1) {
			/* Parse the children of the parrents failure link */
			for (node *failchild : temp->parent->failure->children) {
				if (failchild->c == temp->c) {
					temp->failure = failchild;
				}
			}
		}
		q.pop();
	}
}

/**
 * @brief keywordTrie::addChild Add a child node to the trie
 * @param parrent The parrent node of the new one.
 * @param character The character on the edge to the new node
 */
void keywordTrie::addChild (node *parent, char &character) {
	trieNodes.push_back(node(parent->depth+1, character, parent, root));
	parent->children.push_back(&trieNodes.back());
	parent = &trieNodes.back();
}

/**
 * @brief keywordTrie::addString Insert a new keyword into the keyword trie.
 * @param key The new keyword to be inserted.
 */
void keywordTrie::addString (std::string &key) {
	if (key.empty()) {
		return;
	}
	node *current = root;
	for (char character : key) {
		findChild(current, character, true);
	}

	/* Check if the keyword was already found */
	if (current->id != -1) {
		throw std::runtime_error("Attempted to add two identical strings to keyword tree.");
	}
	keywords.push_back(key);
	current->id = keywords.size();
	addFailureLinks();
}

/**
 * @brief keywordTrie::findChild Searches for a child node with given character
 * @param current The current node
 * @param character The character that we are searching
 * @param addWord Flag sign to decide whether a new node should be added
 */
void keywordTrie::findChild (node *parent, char &character, bool addWord) {
	/* Traverse the children of the node to check for existing character. */
	for (node *child : parent->children) {
		if (child->c == character) {
			parent = child;
			return;
		}
	}
	if (addWord) {
		addChild(parent, character);
	} else {
		parent = parent->failure;
	}
}

/**
 * @brief keywordTrie::parseText Parses a text with the trie
 * @param text The text to be parsed
 * @return Returns a vector with all matches
 */
std::vector<result> keywordTrie::parseText (std::string &text) {
	std::vector<result> results;
	if (text.empty()) {
		return results;
	}
	node* current= root;
	for (unsigned i=0; i < text.size(); i++) {
		findChild(current, text.at(i), false);
		if (current->id != -1) {
			results.push_back(result(keywords.at(current->id),
									 current->id,
									 i - current->depth + 1));
		}
	}
	return results;
}
