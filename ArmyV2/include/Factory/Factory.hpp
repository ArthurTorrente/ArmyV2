#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "IA/TreeIA.hpp"

namespace Factory
{
    TreeIa treeFromCode(const std::string& code);
    std::string codeFromTree(const std::unique_ptr<TreeIa>& tree);
}

#endif //_FACTORY_H_