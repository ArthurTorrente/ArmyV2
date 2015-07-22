#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "IA/TreeIA.hpp"

namespace Factory
{
    /* Make a tree with a code */
    TreeIa treeFromCode(const std::string& code);

    /* Make a string with with tree*/
    std::string codeFromTree(const TreeIa& tree);

    /**
     * Make a random ia
     */
    std::string randomIa();

    /* Make a random decision node */
    void makeDecisionNode(std::stringstream& ss);

    /* Make an action node */
    void makeActionNode(std::stringstream& ss);

    /**
     * Return the end of the branch it begin at startBranch
     */
    std::string::iterator getBranch(std::string::iterator startBranch, const std::string::iterator& endOfString);
}

#endif //_FACTORY_H_