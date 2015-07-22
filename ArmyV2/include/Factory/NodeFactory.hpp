#ifndef _NODEFACTORY_H_
#define _NODEFACTORY_H_

#include <sstream>

#include "DecisionTree/DecisionNode.hpp"
#include "DecisionTree/ActionNode.hpp"

namespace Factory
{
    namespace node
    {
        /**
         * Get a node from code
         *
         * The node can be an action or a decision
         */
        INodeUPtr getNode(std::stringstream& code);
    }
}

#endif //_NODEFACTORY_H_