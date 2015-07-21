#include "Factory/NodeFactory.hpp"

#include <array>

#include "Factory/FactoryException.hpp"
#include "Factory/ExtractorFactory.hpp"
#include "Actions/AllActions.hpp"

namespace Factory
{
    namespace node
    {
        static bool isValidComparator(char comparator)
        {
            static char validComparator[] = {
                '>', '<', '=', '^'
            };
            
            auto compFound = std::find(validComparator, validComparator + 3, comparator);

            if (compFound == validComparator + 4)
                return false;

            return true;
        }

        static std::function<bool(float, float)> getFunctionComparator(char comparator)
        {
            switch (comparator)
            {
            case '>':
                return[](float a, float b){
                    return a > b;
                };
                break;

            case '<':
                return[](float a, float b){
                    return a < b;
                };
                break;

            case '=':
                return[](float a, float b){
                    return a == b;
                };
                break;

            case '^':
                return[](float a, float b){
                    return a != b;
                };
                break;

            default:
                throw Factory::FactoryException("Bad comparator");
            }
        }

        INodeUPtr getNode(std::stringstream& code)
        {
            INodeUPtr node;

            char c;
            code.get(c);

            switch (c)
            {
            case '?':
                {
                    /**
                     * Get Left extractor
                     * Get Comparator
                     * Get Right Extractor
                     * Get Left Node
                     * Get Right Node
                     */

                    FloatExtractorUPtr leftEx = ex::getFloatExtractor(code);

                    char comparator;
                    code.get(comparator);

                    if (!isValidComparator(comparator))
                        throw FactoryException("Invalid Comparator");

                    auto comparatorFunction = getFunctionComparator(comparator);

                    FloatExtractorUPtr rightEx = ex::getFloatExtractor(code);

                    INodeUPtr leftNode = getNode(code);

                    INodeUPtr rightNode = getNode(code);

                    node.reset(
                        new DecisionNode(leftNode, rightNode, comparatorFunction, leftEx, rightEx, std::string(1, comparator))
                        );
                }
            break;

            case '!':
                {
                    /**
                     * Get Action Type
                     * Get extractor if is need
                     */
                    char actionType;
                    code.get(actionType);

                    switch (actionType)
                    {
                    case 'M':
                    {
                        auto pex = ex::getPointExtractor(code);
                        node.reset(
                            getAction<MoveAction, PointExtractorUPtr>(pex)
                            );
                    }
                        break;

                    case 'A':
                    {
                        auto uex = ex::getUnitExtractor(code);
                        node.reset(
                            getAction<ShootAction, UnitExtractorUPtr>(uex)
                            );
                    }
                        break;

                    case 'E':
                    {
                        auto pex = ex::getPointExtractor(code);
                        node.reset(
                            getAction<EscapeAction, PointExtractorUPtr>(pex)
                            );
                    }
                        break;

                    default:
                    {
                        node.reset(
                            getAction<EmptyAction>()
                            );
                    }
                    }
                }
            break;

            default:
                throw FactoryException("Error node code");
            }

            return node;
        }
    }
}