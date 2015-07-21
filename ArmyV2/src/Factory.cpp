#include "Factory/Factory.hpp"

#include <sstream>

#include "Factory/NodeFactory.hpp"
#include "Factory/FactoryException.hpp"
#include "IACodeCollection.hpp"

namespace Factory
{
    TreeIa treeFromCode(const std::string& code)
    {
        std::stringstream ss(code);

        ss.exceptions(std::stringstream::eofbit | std::stringstream::badbit | std::stringstream::failbit);

        try
        {
            INodeUPtr root(node::getNode(ss));

            return TreeIa(root, code);
        }
        catch (...)
        {
            throw FactoryException("Error during TreeIA construction");
        }
    }

    std::string codeFromTree(const TreeIa& tree)
    {
        return tree.getIaCode();
    }

    static void makeFloatExtractor(std::stringstream& ss);
    static void makeUnitExtractor(std::stringstream& ss);
    static void makePointExtractor(std::stringstream& ss);
    static void makeSetExtractor(std::stringstream& ss);


    static void makeFloatExtractor(std::stringstream& ss)
    {
        int exChoose = rand() % 6;
        
        switch (exChoose)
        {
        case 0:
        {
            ss << "C";
            unsigned int capIndex = rand() % 7;
            ss << capIndex;
            makeUnitExtractor(ss);
        }

            break;

        case 1:
            ss << "D";
            makeUnitExtractor(ss);
            makePointExtractor(ss);
            break;

        case 2:
            ss << "M";
            {
                if (rand() % 2)
                {
                    ss << "D";
                    makeSetExtractor(ss);
                    makePointExtractor(ss);
                }
                else
                {
                    unsigned int capIndex = rand() % 7;
                    ss << capIndex;
                    makeSetExtractor(ss);
                }
            }
            break;

        case 3:
            ss << "m";
            {
                if (rand() % 2)
                {
                    ss << "D";
                    makeSetExtractor(ss);
                    makePointExtractor(ss);
                }
                else
                {
                    unsigned int capIndex = rand() % 7;
                    ss << capIndex;
                    makeSetExtractor(ss);
                }
            }
            break;

        case 4:
            ss << "a";
            {
                if (rand() % 2)
                {
                    ss << "D";
                    makeSetExtractor(ss);
                    makePointExtractor(ss);
                }
                else
                {
                    unsigned int capIndex = rand() % 7;
                    ss << capIndex;
                    makeSetExtractor(ss);
                }
            }
            break;

        case 5:
        {
            ss << "V";
            float randomValue = tools::randomFloat();
            ss << std::setprecision(4) << std::fixed;
            ss << randomValue;
            break;
        }
        }
    }

    static void makeUnitExtractor(std::stringstream& ss)
    {
        int exChoose = rand() % 3;

        switch (exChoose)
        {
        case 0:
            ss << "U";
            break;

        case 1:
        {
            ss << "L";

            if (rand() % 2)
            {
                ss << "D";
                makeSetExtractor(ss);
                makePointExtractor(ss);
            }
            else
            {
                unsigned int capIndex = rand() % 7;
                ss << capIndex;

                makeSetExtractor(ss);
            }
        }
            break;

        case 2:
        {
            ss << "H";

            if (rand() % 2)
            {
                ss << "D";
                makeSetExtractor(ss);
                makePointExtractor(ss);
            }
            else
            {
                unsigned int capIndex = rand() % 7;
                ss << capIndex;

                makeSetExtractor(ss);
            }
        }
            break;
        }
    }

    static void makePointExtractor(std::stringstream& ss)
    {
        int exChoose = rand() % 2;

        switch (exChoose)
        {
        case 0:
            ss << "B";
            makeSetExtractor(ss);
            break;

        case 1:
            ss << "P";
            makeUnitExtractor(ss);
            break;
        }
    }

    static void makeSetExtractor(std::stringstream& ss)
    {
        int exChoose = rand() % 4;

        switch (exChoose)
        {
        case 0:
            ss << "A";
            break;

        case 1:
            ss << "O";
            break;

        case 2:
        {
            ss << "N";

            if (rand() % 2)
            {
                ss << "L";
            }
            else
            {
                ss << "H";
            }

            bool isDist = rand() % 2 == 0;

            if (isDist)
            {
                ss << "D";
            }
            else
            {
                ss << static_cast<unsigned int>(rand() % 7);
            }

            ss << static_cast<unsigned int>(rand());
            makeSetExtractor(ss);

            if (isDist)
                makePointExtractor(ss);

        }
            break;

        case 3:
        {
            ss << "T";

            if (rand() % 2)
            {
                ss << "H";
            }
            else
            {
                ss << "L";
            }

            bool isDist = rand() % 2 == 0;

            if (isDist)
            {
                ss << "D";
            }
            else
            {
                ss << static_cast<unsigned int>(rand() % 7);
            }
           
            float randomValue = tools::randomFloat();
            ss << std::setprecision(4) << std::fixed;
            ss << randomValue;

            makeSetExtractor(ss);

            if (isDist)
                makePointExtractor(ss);
        }
            break;
        }
    }

    void makeActionNode(std::stringstream& ss)
    {
        /*
        typeid(EmptyAction), std::string("N")
        typeid(MoveAction), std::string("M")
        typeid(EscapeAction), std::string("E")
        typeid(ShootAction), std::string("A") 
        */
        
        int choose = static_cast<unsigned int>(rand()) % 100;

        ss << "!";

        if (choose < 10)
        {
            ss << "N";
        }
        else if (choose < 40)
        {
            ss << "M";
            makePointExtractor(ss);
        }
        else if (choose < 70)
        {
            ss << "E";
            makePointExtractor(ss);
        }
        else
        {
            ss << "A";
            makeUnitExtractor(ss);
        }
    }

    static void makeComparator(std::stringstream& ss)
    {
        static char validComparator[] = {
            '>', '<', '=', '^'
        };

        int comparatorChoos = rand() % sizeof(validComparator);

        ss << validComparator[comparatorChoos];
    }

    void makeDecisionNode(std::stringstream& ss)
    {
        /**
        * Make a decision
        *
        * rand for leftNode
        * rand for rightNode
        */
        
        ss << "?";

        makeFloatExtractor(ss);
        makeComparator(ss);
        makeFloatExtractor(ss);

        auto config = Config::getInstance();

        unsigned int randomRatio = static_cast<unsigned int>(rand()) % config->getMaxNodeRatio();

        if (randomRatio <= config->getChooseNodeRatio())
            makeActionNode(ss);
        else
            makeDecisionNode(ss);

        randomRatio = static_cast<unsigned int>(rand()) % config->getMaxNodeRatio();

        if (randomRatio <= config->getChooseNodeRatio())
            makeActionNode(ss);
        else
            makeDecisionNode(ss);
    }

    std::string randomIa()
    {
        std::stringstream ssRandom;

        auto config = Config::getInstance();

        unsigned int randomRatio = static_cast<unsigned int>(rand()) % config->getMaxNodeRatio();
        
        if (randomRatio <= config->getChooseNodeRatio())
            makeActionNode(ssRandom);
        else
            makeDecisionNode(ssRandom);

        return ssRandom.str();
    }

    std::string::iterator getBranch(std::string::iterator startBranch, const std::string::iterator& endOfString)
    {
        if (startBranch == endOfString)
            return endOfString;

        if (*startBranch == '?')
        {
            int cpt = 0;
            ++startBranch;

            while (startBranch != endOfString)
            {
                auto d = std::find(startBranch, endOfString, '?');
                auto a = std::find(startBranch, endOfString, '!');

                if (d < a)
                {
                    --cpt;
                    startBranch = d + 1;
                }
                else
                {
                    ++cpt;
                    startBranch = a + 1;
                }

                if (cpt == 2)
                    break;
            }
        }
        else
        {
            ++startBranch;
        }

        auto decisionNode = std::find(startBranch, endOfString, '?');
        auto actionNode = std::find(startBranch, endOfString, '!');

        return decisionNode < actionNode ? decisionNode : actionNode;
    }
}