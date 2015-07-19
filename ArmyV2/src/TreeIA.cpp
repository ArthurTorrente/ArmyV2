#include "IA/TreeIA.hpp"

#include <sstream>
#include <vector>

#include "Factory/Factory.hpp"

TreeIa::TreeIa()
{
    (*this) = std::move(Factory::randomIa());
}

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

std::unique_ptr<Action> TreeIa::operator()(Unit& unit, Army& allies, Army& opponent)
{
    return m_root->getValue(unit, allies, opponent);
}

std::string TreeIa::mutate() const
{
    std::string sIaCode = Factory::codeFromTree(*this);
    std::stringstream ss;
    std::vector<std::string::iterator> decisionNode;
    /* MUTATE */
    
    if (sIaCode[0] == '!')
        Factory::makeActionNode(ss);
    
    else
    {
        auto sBegin = sIaCode.begin();

        while (sBegin != sIaCode.end())
        {
            sBegin = std::find(sBegin, sIaCode.end(), "?");

            decisionNode.push_back(sBegin);
        }

        if (decisionNode.size() == 0)
            throw std::exception("Ia code don't have a decision node");

        auto dChoose = static_cast<unsigned int>(std::rand()) % decisionNode.size();

        {
            std::string tmp(sIaCode.begin(), decisionNode[dChoose]);
            ss << tmp;
        }

        auto endBranch = Factory::getBranch(decisionNode[dChoose], sIaCode.end());

        
        Factory::makeDecisionNode(ss);

        if (endBranch != sIaCode.end())
        {
            std::string tmp(endBranch, sIaCode.end());
            ss << tmp;
        }
    }
    
    return ss.str();
}

std::string TreeIa::operator*(const TreeIa& t) const
{
    std::string sIaCode(Factory::codeFromTree(*this));
    std::string sOtherIaCode(Factory::codeFromTree(t));
    std::stringstream ss;

    /* CROSS OVER */

    if (sIaCode[0] == '!' && sOtherIaCode[0] == '!')
    {
        return (std::rand() % 2 == 0 ? sIaCode : sOtherIaCode);
    }



    return ss.str();
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