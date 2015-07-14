#include "Factory/NodeFactory.hpp"

#include <array>

#include "Factory/ExtractorFactory.hpp"
#include "Actions/EmptyAction.hpp"
#include "Factory/FactoryException.hpp"

namespace Factory
{
    namespace node
    {
        static bool isValidComparator(char comparator)
        {
            static char validComparator[] = {
                '>', '<', '=', '!'
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

            case '!':
                return[](float a, float b){
                    return a != b;
                };
                break;

            default:
                return std::function<bool(float, float)>(nullptr);
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

                    auto leftEx = ex::getFloatExtractor(code);

                    char comparator;
                    code.get(comparator);

                    if (!isValidComparator(comparator))
                        throw FactoryException("Invalid Comparator");

                    auto comparatorFunction = getFunctionComparator(comparator);

                    auto rightEx = ex::getFloatExtractor(code);

                    auto leftNode = getNode(code);

                    auto rightNode = getNode(code);

                    node.reset(
                        new DecisionNode(leftNode, rightNode, comparatorFunction, leftEx, rightEx)
                        );
                }
            break;

            case '!':
                {
                    /**
                     * Get Action Type
                     * Get extractor if is need
                     */
                }
            break;

            default:
                {
                    auto actionNode = getAction<EmptyAction>();

                    node.reset(actionNode);
                }
            }

            return node;
        }
    }
}