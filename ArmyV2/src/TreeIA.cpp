#include "IA/TreeIA.hpp"

TreeIa::TreeIa(const std::string& iaCode, INodeUPtr& root)
    : m_iaCode(iaCode),
    m_root(std::move(root))
{}

TreeIa::TreeIa(TreeIa&& t)
    : m_iaCode(std::move(t.m_iaCode)),
    m_root(std::move(t.m_root))
{}

std::unique_ptr<Action> TreeIa::operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
{
    return m_root->getValue(unit, allies, opponent);
}

const std::string TreeIa::getIaCode() const
{
    return m_iaCode;
}

const INodeUPtr& TreeIa::getRoot() const
{
    return m_root;
}

void TreeIa::setIaCode(const std::string& iaCode)
{
    m_iaCode = iaCode;
}

void TreeIa::setRoot(std::unique_ptr<INode>& root)
{
    m_root = std::move(root);
}