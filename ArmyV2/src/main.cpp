#pragma warning(push, 0)
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#pragma warning(pop)

#include "Army.hpp"
#include "training.hpp"
#include "Config.hpp"

#include "DecisionTree/DecisionNode.hpp"
#include "DecisionTree/ActionNode.hpp"
#include "Actions/MoveAction.hpp"

//#define UNITTEST

#ifdef UNITTEST
    #include "Extractor/UnitTest.hpp"
    #include "Factory/UnitTest.hpp"
#endif

void computeArg(int argc, char** argv)
{
    auto config = Config::getInstance();

    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            ++i;
            unsigned int h = std::strtol(argv[i], nullptr, 10);
            config->setHeight(h);
        }
        else if (strcmp(argv[i], "-w") == 0)
        {
            ++i;
            unsigned int w = std::strtol(argv[i], nullptr, 10);
            config->setWidth(w);
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            config->setLog(true);
        }
        else if (strcmp(argv[i], "-I") == 0)
        {
            ++i;
            config->setI(static_cast<unsigned int>(strtoul(argv[i], nullptr, 10)));
        }
        else if (strcmp(argv[i], "-T") == 0)
        {
            ++i;
            config->setT(static_cast<unsigned int>(strtoul(argv[i], nullptr, 10)));
        }
        else if (strcmp(argv[i], "-N") == 0)
        {
            ++i;
            config->setN(static_cast<unsigned int>(strtoul(argv[i], nullptr, 10)));
        }
        else if (strcmp(argv[i], "-X") == 0)
        {
            ++i;
            config->setX(static_cast<unsigned int>(strtoul(argv[i], nullptr, 10)));
        }
        else if (strcmp(argv[i], "-Y") == 0)
        {
            ++i;
            config->setY(static_cast<unsigned int>(strtoul(argv[i], nullptr, 10)));
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            ++i;
            config->setChooseNodeRatio(static_cast<unsigned int>(strtoul(argv[i], nullptr, 10)));
        }
        else if (strcmp(argv[i], "-m") == 0)
        {
            ++i;
            config->setMaxNodeRatio(static_cast<unsigned int>(strtoul(argv[i], nullptr, 10)));
        }
    }
}

void unitTest()
{
#ifdef UNITTEST
    if (!unitTest_Factory())
    {
        std::cout << "One unit test for factory failed" << std::endl;
    }
    else
    {
        std::cout << "Factory unitTest success" << std::endl;
    }

    if (!unitTest_ValueExtractor() ||
        !unitTest_PointExtractor() ||
        !unitTest_UnitExtractor() ||
        !unitTest_SetExtractor())
    {
        std::cout << "One extractor unit test has failed!" << std::endl;
    }
    else
    {
        std::cout << "All extractor test are OK !" << std::endl;
    }

    std::vector<Unit*> unitList;

    unitList.reserve(100);

    std::generate_n(std::back_inserter(unitList), 100, []()
    {
        return new Unit(5);
    });

    tools::unusedArg(unitList);

    for (auto u : unitList)
    {
        std::cout << u->getTree().getIaCode() << std::endl << std::endl;
        delete u;
    }
    
#endif
}

int main(int argc, char** argv)
{
#ifdef UNITTEST
    tools::unusedArg(argc, argv);
    unitTest();

#else
    if (argc > 1)
        computeArg(argc-1, &(argv[1]));

    std::srand(static_cast<unsigned int>(time(nullptr)));
    std::vector<std::unique_ptr<Army> > champions;
    try
    {
        std::ifstream in("Army_10_100.save");
        Army army = Army::load(in);
        std::cout << army << std::endl;
        champions.push_back(std::unique_ptr<Army>(new Army(army)));
    }
    catch(...)
    {
    }

    auto config = Config::getInstance();

    std::unique_ptr<Army> army = train(config->I(), config->T(), config->N(), config->X(), config->Y(), champions);
    
    std::cout << *army << std::endl;
    std::ofstream out("Army_10_100.save");
    army->save(out);
    out.flush();
    out.close();
#endif

    std::cout << "Fin generation" << std::endl;
    std::cin.ignore();

    return 0;
}