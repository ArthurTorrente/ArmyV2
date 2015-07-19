#include "DecisionTree/DecisionNode.hpp"

DecisionNode::DecisionNode(std::unique_ptr<INode>& left, std::unique_ptr<INode>& right, std::function<bool(float, float)> comparator, FloatExtractorUPtr& leftEx, FloatExtractorUPtr& rightEx, const std::string& comparatorCode)
    : m_left(std::move(left)),
    m_right(std::move(right)),
    m_comparator(comparator),
    m_leftExtractor(std::move(leftEx)),
    m_rightExtractor(std::move(rightEx)),
    m_comparatorCode(comparatorCode)
{}

std::unique_ptr<Action> DecisionNode::getValue(Unit& unit, Army& a, Army& b)
{
    if (m_comparator((*m_leftExtractor)(unit, a, b), (*m_rightExtractor)(unit, a, b)))
        return m_left->getValue(unit, a, b);
    
    return m_right->getValue(unit, a, b);
}

std::string DecisionNode::getCode() const
{
    std::string code("?");

    code += m_leftExtractor->getCode();
    code += m_comparatorCode;
    code += m_rightExtractor->getCode();
    code += m_left->getCode();
    code += m_right->getCode();

    return code;
}