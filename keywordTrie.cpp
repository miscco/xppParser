#include "keywordTrie.h"

namespace keywordTrie {

/**
 * @brief trie::trie Initialize the trie structure with its root node.
 */
trie::trie() {
	trieNodes.push_back(node());
	root = &trieNodes.at(0);
	root->failure = root;
}

/**
 * @brief trie::addChild Add a child node to the trie
 * @param parrent The parrent node of the new one.
 * @param character The character on the edge to the new node
 */
void trie::addChild (node *parent, char &character) {
	trieNodes.push_back(node(parent->depth+1, character, parent, root));
	parent->children.push_back(&trieNodes.back());
	parent = &trieNodes.back();
}

/**
 * @brief trie::addFailureLinks Utilize a breadth first search to generate the
 * failure links.
 */
void trie::addFailureLinks() {
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
 * @brief trie::addString Wrapper around addString(std::string) to add
 * containers of strings
 * @param keyList The container with the keys.
 */
template<template<class, class> class TContainer>
void trie::addString(const TContainer<std::string, std::allocator<std::string*>> &keyList) {
	for (const std::string &key : keyList)
		addString(key, false);
	addFailureLinks();
}

/**
 * @brief trie::addString Insert a new keyword into the keyword trie.
 * @param key The new keyword to be inserted.
 */
void trie::addString (const std::string &key, bool addFailure = true) {
	if (key.empty()) {
		return;
	}
	node *current = root;
	for (char character : key)
		findChild(current, character, true);

	/* Check if the keyword was already found */
	if (current->id != -1) {
		throw std::runtime_error(
					"Attempted to add two identical strings to keyword tree.");
	}
	keywords.push_back(key);
	current->id = keywords.size();
	if (addFailure)
		addFailureLinks();
}

/**
 * @brief trie::findChild Searches for a child node with given character
 * @param current The current node
 * @param character The character that we are searching
 * @param addWord Flag sign to decide whether a new node should be added
 */
void trie::findChild (node *parent, char &character, bool addWord) {
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
 * @brief trie::parseText Parses a text with the trie
 * @param text The text to be parsed
 * @return Returns a vector with all matches
 */
std::vector<result> trie::parseText (std::string &text) {
	std::vector<result> results;
	if (text.empty()) {
		return results;
	}
	node *current= root;
	node *temp;
	for (unsigned i=0; i < text.size(); i++) {
		findChild(current, text.at(i), false);
		if (current->id != -1) {
			results.push_back(result(keywords.at(current->id),
									 current->id,
									 i - current->depth + 1));
		}
		temp = temp->failure;
		/* Process the failure links for possible additional matches */
		while (temp->depth > 0) {
			if (temp->id != -1) {
				results.push_back(result(keywords.at(temp->id),
										 temp->id,
										 i - temp->depth + 1));
			}
			temp = temp->failure;
		}
	}
	return results;
}

} // namespace keywordTrie
