#ifndef _ACTIONNODE_H_
#define _ACTIONNODE_H_

#pragma warning(push, 0)
#include <memory>
#pragma warning(pop)

#include "INode.hpp"

class ActionNode : INode
{
public:
    ActionNode();

    std::unique_ptr<Action> getValue();
};

#endif //_ACTIONNODE_H_