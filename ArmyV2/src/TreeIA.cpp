#include "IA/TreeIA.hpp"

#include "Factory/Factory.hpp"

TreeIa::TreeIa(INodeUPtr& root)
    : m_root(std::move(root))
{
    if (m_root == nullptr)
    {
        (*this) = std::move(Factory::randomIa());
    }
}

TreeIa::TreeIa(TreeIa&& t)
    : m_root(std::move(t.m_root))
{
    if (m_root == nullptr)
    {
        (*this) = std::move(Factory::randomIa());
    }
}

TreeIa& TreeIa::operator=(TreeIa&& tree)
{
    if (this != &tree)
    {
        m_root.release();
        m_root = std::move(tree.m_root);
    }

    return *this;
}

std::unique_ptr<Action> TreeIa::operator()(const UnitSPtr& unit, const ArmySPtr& allies, const ArmySPtr& opponent)
{
    return m_root->getValue(unit, allies, opponent);
}

const std::string TreeIa::getIaCode() const
{
    return m_root->getCode();
}

const INodeUPtr& TreeIa::getRoot() const
{
    return m_root;
}

void TreeIa::setRoot(std::unique_ptr<INode>& root)
{
    m_root = std::move(root);
}