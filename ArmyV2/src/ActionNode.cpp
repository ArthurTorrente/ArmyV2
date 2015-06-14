#include "DecisionTree/ActionNode.hpp"

#include "Actions/EmptyAction.hpp"

ActionNode::ActionNode()
{

}

std::unique_ptr<Action> ActionNode::getValue()
{
    return std::unique_ptr<Action>(nullptr);
}