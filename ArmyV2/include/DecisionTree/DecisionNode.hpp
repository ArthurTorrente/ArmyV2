#ifndef _DECISIONNODE_H_
#define _DECISIONNODE_H_

#include <memory>
#include <functional>

#include "INode.h"

#include "ActionNode.h"
#include "Action.hpp"
#include "Army.hpp"

class DecisionNode : INode
{
public:
    DecisionNode();
    
    std::unique_ptr<Action> getValue();
protected:
    std::function<bool(const Unit&, const Army&, const Army&)> m_goToLeftPredicate;
};

#endif //_DECISIONNODE_H_