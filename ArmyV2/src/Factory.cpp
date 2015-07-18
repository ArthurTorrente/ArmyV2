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

            return TreeIa(root);
        }
        catch (...)
        {
            throw FactoryException("Error during TreeIA construction");
        }
    }

    std::string codeFromTree(const std::unique_ptr<TreeIa>& tree)
    {
        return tree->getIaCode();
    }

    static void makeFloatExtractor(std::stringstream& ss);
    static void makeUnitExtractor(std::stringstream& ss);
    static void makePointExtractor(std::stringstream& ss);
    static void makeSetExtractor(std::stringstream& ss);


    static void makeFloatExtractor(std::stringstream& ss)
    {
        int exChoose = rand() % 7;
        
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

        case 6:
        {
            ss << "V";
            float randomValue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
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

                if (rand() % 2)
                {
                    ss << "D";
                }
                else
                {
                    ss << static_cast<unsigned int>(rand() % 7);
                }
            }
            else
            {
                ss << "H";

                if (rand() % 2)
                {
                    ss << "D";
                }
                else
                {
                    ss << static_cast<unsigned int>(rand() % 7);
                }
            }

            ss << static_cast<unsigned int>(rand());
            makeSetExtractor(ss);
        }
            break;

        case 3:
            ss << "T";
            break;
        }
    }

    static void makeActionNode(std::stringstream& ss)
    {
        /*
        typeid(EmptyAction), std::string("N")
        typeid(MoveAction), std::string("M")
        typeid(EscapeAction), std::string("E")
        typeid(ShootAction), std::string("A") 
        */
        
        int choose = rand() % IACodeCollection.size();
        auto it = IACodeCollection.begin();

        while (choose)
        {
            it = ++it;
        }

        auto& sChoose(it->second);

        ss << "!";

        if (sChoose == "N")
        {
            ss << "N";
        }
        else if (sChoose == "M")
        {
            ss << "M";
            makePointExtractor(ss);
        }
        else if (sChoose == "E")
        {
            ss << "E";
            makePointExtractor(ss);
        }
        else if (sChoose == "A")
        {
            ss << "A";
            makeUnitExtractor(ss);
        }
    }

    static void makeComparator(std::stringstream& ss)
    {
        static char validComparator[] = {
            '>', '<', '=', '!'
        };

        int comparatorChoos = rand() % sizeof(validComparator);

        ss << validComparator[comparatorChoos];
    }

    static void makeDecisionNode(std::stringstream& ss)
    {
        /**
        * Make a decision
        *
        * rand for leftNode
        * rand for rightNode
        */

        makeFloatExtractor(ss);
        makeComparator(ss);
        makeFloatExtractor(ss);

        for (unsigned int i = 0; i < 2; ++i)
        {
            if (rand() % 2)
                makeDecisionNode(ss);
            else
                makeActionNode(ss);
        }
    }

    TreeIa randomIa()
    {
        std::string randomIa;
        std::stringstream ssRandom(randomIa);

        if (rand() % 2)
            makeDecisionNode(ssRandom);
        else
            makeActionNode(ssRandom);


        return treeFromCode(randomIa);
    }
}