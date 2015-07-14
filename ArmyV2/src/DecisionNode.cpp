#include "DecisionTree/DecisionNode.hpp"

DecisionNode::DecisionNode(std::unique_ptr<INode>& left, std::unique_ptr<INode>& right, std::function<bool(float, float)> comparator, FloatExtractorUPtr& leftEx, FloatExtractorUPtr& rightEx)
    : m_left(std::move(left)),
    m_right(std::move(right)),
    m_comparator(comparator),
    m_leftExtractor(std::move(leftEx)),
    m_rightExtractor(std::move(rightEx))
{}

std::unique_ptr<Action> DecisionNode::getValue(const UnitSPtr& unit, const ArmyPtr& a, const ArmyPtr& b)
{
    if (m_comparator((*m_leftExtractor)(unit, a, b), (*m_rightExtractor)(unit, a, b)))
        return m_left->getValue(unit, a, b);
    
    return m_right->getValue(unit, a, b);
}