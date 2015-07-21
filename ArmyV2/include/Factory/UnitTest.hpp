#ifndef _UNITTESTFACTORY_H_
#define _UNITTESTFACTORY_H_

#include <iostream>
#include <sstream>
#include <iomanip>

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
        auto treeCode = Factory::randomIa();
        auto tree = Factory::treeFromCode(treeCode);
        auto randomIaCode = tree.getIaCode();;
    }
    catch (Factory::FactoryException& fe)
    {
        std::cout << fe.what() << std::endl;
    }

    return true;
}

bool mutate()
{
    TreeIa tree_1;
    TreeIa tree_2;

    std::string code_1(Factory::codeFromTree(tree_1));
    std::string code_2(Factory::codeFromTree(tree_2));

    std::string codeIa;

    while (code_1[0] != '?')
    {
        codeIa = Factory::randomIa();
        tree_1 = Factory::treeFromCode(codeIa);
        code_1 = tree_1.getIaCode();
    }
        

    while (code_2[0] != '!')
    {
        codeIa = Factory::randomIa();
        tree_2 = Factory::treeFromCode(codeIa);
        code_2 = tree_2.getIaCode();
    }


    auto mutate_code_1 = tree_1.mutate();
    auto mutate_code_2 = tree_2.mutate();

    auto mutateTree_1 = Factory::treeFromCode(mutate_code_1);
    auto mutateTree_2 = Factory::treeFromCode(mutate_code_2);

    auto mutateTree1IaCode = mutateTree_1.getIaCode();
    auto mutateTree2IaCode = mutateTree_2.getIaCode();

    bool tree1AIsGood = mutateTree1IaCode == mutate_code_1 && mutate_code_1 != code_1;
    bool tree2AIsGood = mutateTree2IaCode == mutate_code_2 && mutate_code_2 != code_2;
    
    return tree1AIsGood && tree2AIsGood;
}

bool crossOver()
{
    TreeIa t1, t2, t3, t4;

    std::string code_1(Factory::codeFromTree(t1)), code_2(Factory::codeFromTree(t2)), code_3(Factory::codeFromTree(t3)), code_4(Factory::codeFromTree(t4));

    std::string codeIa;
    while (code_1[0] != '?')
    {
        codeIa = Factory::randomIa();
        t1 = Factory::treeFromCode(codeIa);
        code_1 = t1.getIaCode();
    }


    while (code_2[0] != '!')
    {
        codeIa = Factory::randomIa();
        t2 = Factory::treeFromCode(codeIa);
        code_2 = t2.getIaCode();
    }

    while (code_3[0] != '?')
    {
        codeIa = Factory::randomIa();
        t3 = Factory::treeFromCode(codeIa);
        code_3 = t3.getIaCode();
    }


    while (code_4[0] != '!')
    {
        codeIa = Factory::randomIa();
        t4 = Factory::treeFromCode(codeIa);
        code_4 = t4.getIaCode();
    }

    auto doubleD = t1 * t3;
    auto doubleA = t2 * t4;
    auto da = t1 * t2;
    auto ad = t2 * t1;

    return true;
}

bool unitTest_randomGen()
{
    unsigned int cpt = 1000;
    std::string codeIa;
    std::string codeTree;
    while (cpt--)
    {
        try
        {
            codeIa = Factory::randomIa();
            auto tree = Factory::treeFromCode(codeIa);
            codeTree = tree.getIaCode();

            if (codeIa != codeTree)
                throw Factory::FactoryException("Code differents");
            
        }
        catch (Factory::FactoryException&)
        {
            std::cout << "f has an error" << std::endl;
        }
    }

    return true;
}

bool unitTest_Factory()
{
    return unitTest_randomGen() && codeToTree() && randomIa() && mutate() && crossOver();
}

#endif //_UNITTESTFACTORY_H_