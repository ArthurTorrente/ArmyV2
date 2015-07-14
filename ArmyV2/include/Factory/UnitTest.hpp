#ifndef _UNITTESTFACTORY_H_
#define _UNITTESTFACTORY_H_

#include "Factory.hpp"

bool unitTest_Factory()
{
    std::string code(
        "?ABCS>ABCD"
        );

    auto tree = Factory::treeFromCode(code);

    std::cout << tree.getIaCode() << std::endl;

    return false;
}

#endif //_UNITTESTFACTORY_H_