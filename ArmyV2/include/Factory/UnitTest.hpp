#ifndef _UNITTESTFACTORY_H_
#define _UNITTESTFACTORY_H_

#include "Factory.hpp"
#include "FactoryException.hpp"

bool codeToTree()
{
    std::string code(
        "?C1LDOPU<C4U!ALDOPU?aDOPU<a5O!EBO!N"
        );

    std::stringstream ss;

    ss << std::setprecision(4) << std::fixed;
    ss << "?V";
    ss << 10.0f;
    ss << "=V";
    ss << 11.0f;
    ss << "!AU!N";

    std::string predefineValue(ss.str());

    std::string errorNodeCode(
        "VNTREZ"
        );

    std::string errorExtractorCode(
        "?V10=Vab"
        );

    try
    {
        auto tree = Factory::treeFromCode(code);

        std::string treeCode(tree.getIaCode());

        if (treeCode != code)
        {
            std::cout << "IACode is different !!" << std::endl;
            return false;
        }
    }
    catch (Factory::FactoryException& fe)
    {
        std::cout << fe.what() << std::endl;
        return false;
    }

    try
    {
        auto tree = Factory::treeFromCode(predefineValue);
        auto treeCode = tree.getIaCode();

        if (treeCode != predefineValue)
        {
            std::cout << "IACode is different for predefine value" << std::endl;
            return false;
        }
    }
    catch (Factory::FactoryException& fe)
    {
        std::cout << fe.what() << std::endl;
        return false;
    }

    try
    {
        auto tree = Factory::treeFromCode(errorNodeCode);

        std::cout << "No error found in error node code" << std::endl;

        return false;
    }
    catch (Factory::FactoryException&)
    {
        std::cout << "Error found in error node code (it's normal)" << std::endl;
    }

    try
    {
        auto tree = Factory::treeFromCode(errorExtractorCode);

        std::cout << "No error found in error extractor code" << std::endl;

        return false;
    }
    catch (Factory::FactoryException&)
    {
        std::cout << "Error found in error extractor code (it's normal)" << std::endl;
    }

    return true;
}

bool randomIa()
{
    try
    {
        auto tree = Factory::randomIa();

        auto randomIaCode = Factory::codeFromTree(tree);
    }
    catch (Factory::FactoryException& fe)
    {
        std::cout << fe.what() << std::endl;
    }

    return true;
}

bool unitTest_Factory()
{
    return codeToTree() && randomIa();
}

#endif //_UNITTESTFACTORY_H_