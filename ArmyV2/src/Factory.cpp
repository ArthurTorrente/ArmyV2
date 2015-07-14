#include "Factory/Factory.hpp"

#include <sstream>

#include "Factory/NodeFactory.hpp"

#include "Factory/FactoryException.hpp"

namespace Factory
{
    TreeIa treeFromCode(const std::string& code)
    {
        std::stringstream ss(code);

        ss.exceptions(std::stringstream::eofbit);

        try
        {
            INodeUPtr root(node::getNode(ss));

            return TreeIa(code, root);
        }
        catch (...)
        {
            throw FactoryException("Error during TreeIA construction");
        }
    }

    std::string codeFromTree(const std::unique_ptr<TreeIa>& tree)
    {
        return std::string();
    }
}