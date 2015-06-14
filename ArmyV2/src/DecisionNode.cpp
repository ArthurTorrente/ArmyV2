#include "DecisionTree/DecisionNode.hpp"

DecisionNode::DecisionNode()
{}

std::unique_ptr<Action> DecisionNode::getValue()
{
    if (m_goToLeftPredicate())
    {
        return m_left->getValue();
    }

    return m_right->getValue();
}