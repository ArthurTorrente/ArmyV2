#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "IA/TreeIA.hpp"

namespace Factory
{
    TreeIa treeFromCode(const std::string& code);
    std::string codeFromTree(const TreeIa& tree);

    /**
     * Make a random ia
     */
    TreeIa randomIa();

    void makeDecisionNode(std::stringstream& ss);
    void makeActionNode(std::stringstream& ss);

    /**
     * Return the end of the branch it begin at startBranch
     */
    std::string::iterator getBranch(std::string::iterator startBranch, const std::string::iterator& endOfString);
}

#endif //_FACTORY_H_