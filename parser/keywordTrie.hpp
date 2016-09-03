/*
* Copyright (C) 2016 Michael Schellenberger Costa.
*
* This code is based on a C-implementation of the keyword trie construction,
* preprocessing and text search by Bernhard Haubold.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef KEYWORDTRIE_HPP
#define KEYWORDTRIE_HPP
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace keywordTrie {

/**
 * @brief The node struct containing the information of a trie node.
 */
struct node {
    int id              = -1;		/**< Keyword index */
    unsigned depth      = 0;		/**< Depth in the trie*/
    char c              = '\0';		/**< Character labelling the incoming edge */
    const node *parent	= nullptr;	/**< Parent node */
    node *failure       = nullptr;	/**< Failure link */
    node *output        = nullptr;	/**< Output link */
    std::vector<node*> children;	/**< Child nodes */

    explicit node () {}
    explicit node (int d, const char &character, const node *par, node *root)
        : depth(d), c(character), parent(par), failure(root), output(root) {}
};

/**
 * @brief The result struct containing the information about matches during a
 * search.
 */
struct result {
    std::string keyword;	/**< The found keyword */
    int         id;			/**< The index of the keyword in the keyword list*/
    int			start;		/**< The starting position of the match */
    int			end;		/**< The end position of the match */

    explicit result (const std::string &key, int id)
        : keyword(key), id(id) {}
    explicit result (const result &res, int endPos)
        : keyword(res.keyword), id(res.id), start(endPos-res.keyword.size()+1),
          end(endPos) {}
};

typedef std::vector<keywordTrie::result> resultCollection;
typedef std::vector<resultCollection>	 resultTable;

/**
 * @brief The trie class representing the keyword trie.
 */
class trie
{
private:
    node *root		   = nullptr;   /**< The root node */
    std::vector<node*>	trieNodes;	/**< Container of the node pointers */
    resultCollection	keywords;	/**< Container of the result stubs */
    bool caseSensitive = true;      /**< Flag for case sensitivity */
    bool wholeWords    = false;     /**< Flag for result validity */

public:
    /**
     * @brief trie Initializes the trie structure with its root node.
     */
    trie() {
        root = new node();
        root->parent  = root;
        root->failure = root;
        root->output  = root;
        trieNodes.push_back(root);
    }
    /**
     * @brief trie Copy an existing keyword trie.
     */
    trie(const trie &Trie)
        : trie() {
        for (const result &res : Trie.keywords) {
            addString(res.keyword, false);
        }
        addFailureLinks();
    }
    /**
     * @brief ~trie Destructor of the trie that frees nodes from heap.
     */
    ~trie() {for (node* N : trieNodes) delete N;}

    /**
     * @brief addString Insert a new keyword into the keyword trie.
     * @param key The new keyword to be inserted.
     * @param addFailure Flag to signal whether the failure links should
     * immediately be updated.
     */
    void addString (const std::string &key, bool addFailure) {
        if (key.empty()) {
            return;
        }
        node *current = root;
        for (const char &character : key) {
            const char c = caseSensitive ? character : std::tolower(character);
            current = addChild(current, c);
        }
        if (current->id != -1) {
            throw std::runtime_error(
                        "Attempted to add two identical strings to the keyword tree.");
        }
        current->id = keywords.size();
        keywords.push_back(result(key, keywords.size()));

        if (addFailure) {
            addFailureLinks();
        }
    }
    /**
     * @brief addString Wrapper around addString(std::string, bool).
     * @param key The new keyword to be inserted.
     */
    void addString (const std::string &key) {addString(key, true);}

    /**
     * @brief addStrings Wrapper around addString(std::string, bool) to add a
     * set of strings.
     * @param keyList The set containing the keys.
     */
    void addString(const std::set<std::string> &keyList) {
        for (const std::string &key : keyList) {
            addString(key, false);
        }
        addFailureLinks();
    }

    /**
     * @brief addStrings Wrapper around addString(std::string, bool) to add a
     * vector of strings.
     * @param keyList The vector containing the keys.
     */
    void addString(const std::vector<std::string> &keyList) {
        for (const std::string &key : keyList) {
            addString(key, false);
        }
        addFailureLinks();
    }

    /**
     * @brief parseText Parses a text with the trie.
     * @param text The text to be parsed.
     * @return Returns a vector with all matches.
     */
    resultCollection parseText (const std::string &text) const {
        resultCollection results;
        if (text.empty()) {
            return results;
        }
        node *current= root;
        for (unsigned i=0; i < text.size(); i++) {
            const char c = caseSensitive ? text.at(i) : std::tolower(text.at(i));
            current = findChild(current, c);
            if (current->id != -1) {
                if (!wholeWords || current->depth == text.size()) {
                    results.push_back(result(keywords.at(current->id), i));
                }
            }
            /* Process the output links for possible additional matches */
            if (!wholeWords) {
                node *temp = current->output;
                while (temp != root) {
                    results.push_back(result(keywords.at(temp->id), i));
                    temp = temp->output;
                }
            }
        }
        return results;
    }

    /**
     * @brief setCaseSensitivity Set the case sensitivity flag.
     * @param flag The new flag.
     */
    void setCaseSensitivity (bool flag) {
        caseSensitive = flag;
        if (!keywords.empty()) {
            throw std::runtime_error("Switching case sensitivity with existing "
                                     "trie might lead to invalid results");
        }
    }

    /**
     * @brief setWholeWords Defines whether partial matches a valid.
     * @param flag The new flag.
     */
    void setWholeWords (bool flag) {
        wholeWords = flag;
    }

private:
    /**
     * @brief addChild Add a child node to the trie.
     * @param parrent The pointer to the parrent node of the new one.
     * @param character The character on the edge to the new node.
     * @return The pointer to the newly created node.
     */
    node* addChild (node *current, const char &character) {
        for (node *child : current->children) {
            if (child->c == character) {
                return child;
            }
        }
        trieNodes.push_back(new node(current->depth+1, character, current, root));
        current->children.push_back(trieNodes.back());
        return trieNodes.back();
    }

    /**
     * @brief findChild Searches for a child node with given character or adds one.
     * @param current The pointer to the current node.
     * @param character The character that is searched.
     * @param addWord Flag sign to decide whether a new node should be added.
     * @return The pointer to the matching node (possibly after failure links),
     * root or the newly created one.
     */
    node* findChild (node *current, const char &character) const {
        for (node *child : current->children) {
            if (child->c == character) {
                return child;
            }
        }
        return traverseFail(current, character);
    }

    /**
     * @brief traverseFail Traverse the failure links during a search.
     * @param current The original node.
     * @param character The character that is beeing searched.
     * @return The pointer to the matching node after a failure link or root
     */
    node* traverseFail (const node *current, const char &character) const {
        node *temp = current->failure;
        while (temp != root) {
            for (node *failchild : temp->children) {
                if (failchild->c == character) {
                    return failchild;
                }
            }
            temp = temp->failure;
        }
        for (node *rootchild : root->children) {
            if (rootchild->c == character) {
                return rootchild;
            }
        }
        return root;
    }

    /**
     * @brief addFailureLinks Utilize a breadth first search to generate the
     * failure links.
     */
    void addFailureLinks() {
        std::queue<node*> q;
        q.push(root);
        while (!q.empty()) {
            node *temp = q.front();
            for (node *child : temp->children) {
                q.push(child);
            }
            /* A failure link with just one less charater is the optimum and will
             * never change.
             */
            if (temp->failure->depth < temp->depth - 1) {
                for (node *failchild : temp->parent->failure->children) {
                    if (failchild->c == temp->c) {
                        temp->failure = failchild;
                    }
                }
            }

            /* Process the failure links for possible additional matches */
            node *out = temp->failure;
            while (out != root) {
                if (out->id != -1) {
                    break;
                }
                out = out->failure;
            }
            temp->output = out;
            q.pop();
        }
    }
};

} // namespace keywordTrie
#endif // KEYWORDTRIE_HPP
