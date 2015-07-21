#ifndef _DECISIONNODE_H_
#define _DECISIONNODE_H_

#pragma warning(push, 0)
#include <memory>
#include <functional>
#pragma warning(pop)

#include "INode.hpp"

#include "ActionNode.hpp"
#include "Army.hpp"
#include "Extractor/IExtractor.hpp"

/**
 * Decision node take a decision
 * It use two value extractor and one comparator.
 * If the comparaison is true, it call the value of left node
 */

class DecisionNode : public INode
{
public:
    DecisionNode(std::unique_ptr<INode>& left, std::unique_ptr<INode>& right, std::function<bool(float, float)> comparator, FloatExtractorUPtr& leftEx, FloatExtractorUPtr& rightEx, const std::string& comparatorCode);
    
    std::unique_ptr<Action> getValue(Unit& unit, Army& a, Army& b);

    std::string getCode() const;

protected:
    std::unique_ptr<INode> m_left;
    std::unique_ptr<INode> m_right;

    std::function<bool(float, float)> m_comparator;
    FloatExtractorUPtr m_leftExtractor;
    FloatExtractorUPtr m_rightExtractor;

    std::string m_comparatorCode;
};

#endif //_DECISIONNODE_H_