#include "Army.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "training.hpp"
#include "fstream"

#include "Config.h"

static unsigned int w = 10;
static unsigned int h = 10;

void computeArg(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            ++i;
            h = std::strtol(argv[i], nullptr, 10);
        }
        else if (strcmp(argv[i], "-w") == 0)
        {
            ++i;
            w = std::strtol(argv[i], nullptr, 10);
        }
    }
}

int main(int argc, char** argv)
{
    if (argc > 1)
        computeArg(argc, &(argv[1]));

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

    auto config = Config::getInsance();

    config->setWidth(w);
    config->setHeight(h);

    std::unique_ptr<Army> army = train(10, 100, 20, 10, 100, champions);
    std::cout << *army << std::endl;
    std::ofstream out("Army_10_100.save");
    army->save(out);
    out.flush();
    out.close();

    return 0;
}