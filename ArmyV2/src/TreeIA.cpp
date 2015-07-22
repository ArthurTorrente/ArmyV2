#include "IA/TreeIA.hpp"

#include <sstream>
#include <vector>

#include "Factory/Factory.hpp"

TreeIa::TreeIa()
{
    m_iaCode = Factory::randomIa();
    //(*this) = std::move();
    *this = Factory::treeFromCode(m_iaCode);
}

TreeIa::TreeIa(INodeUPtr& root, const std::string& codeIa)
    : m_iaCode(codeIa),
    m_root(std::move(root))
{
    if (m_root == nullptr)
    {
        m_iaCode = Factory::randomIa();
        *this = Factory::treeFromCode(m_iaCode);
        //(*this) = std::move(Factory::randomIa());
    }
}

TreeIa::TreeIa(TreeIa&& t)
    : m_iaCode(std::move(t.m_iaCode)),
    m_root(std::move(t.m_root))
{
    if (m_root == nullptr)
    {
        m_iaCode = Factory::randomIa();
        
        *this = Factory::treeFromCode(m_iaCode);
        //(*this) = std::move(Factory::randomIa());
    }
}

TreeIa& TreeIa::operator=(TreeIa&& tree)
{
    if (this != &tree)
    {
        m_root.release();
        m_root = std::move(tree.m_root);
        m_iaCode = std::move(tree.m_iaCode);

        if (m_root == nullptr)
        {
            m_iaCode = Factory::randomIa();
            *this = Factory::treeFromCode(m_iaCode);
        }
    }

    return *this;
}

std::unique_ptr<Action> TreeIa::operator()(Unit& unit, Army& allies, Army& opponent)
{
    return m_root->getValue(unit, allies, opponent);
}


std::string TreeIa::mutate() const
{
    /**
     * Mutation change one branch of Tree
     *
     * It gets all the decision node into the tree and take a random decision node
     * it generates a new decision branch for replace the older
     */
    std::string sIaCode(getIaCode());
    std::stringstream ss;
    std::vector<std::string::iterator> decisionNode;
    /* MUTATE */

    /* If only action node regenerate new action node */
    if (sIaCode[0] == '!')
        Factory::makeActionNode(ss);
    
    else
    {
        auto sBegin = sIaCode.begin();

        /* Get all decision node */
        while (sBegin != sIaCode.end())
        {
            sBegin = std::find(sBegin, sIaCode.end(), '?');
            
            if (sBegin != sIaCode.end())
            {
                decisionNode.push_back(sBegin);
                ++sBegin;
            }
        }

        if (decisionNode.size() == 0)
            throw std::exception("Ia code don't have a decision node");

        /* take random */
        auto dChoose = static_cast<unsigned int>(std::rand()) % decisionNode.size();

        auto dChooseIt = decisionNode[dChoose];

        /* copy from begin to choose decision node */
        if (dChooseIt != sIaCode.begin())
        {
            std::string tmp(sIaCode.begin(), dChooseIt);
            ss << tmp;
        }

        auto endBranch = Factory::getBranch(dChooseIt, sIaCode.end());

        /* generate newer decision node */
        Factory::makeDecisionNode(ss);

        /* copy the rest of current iaCode */
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
    std::string sIaCode(getIaCode());
    std::string sOtherIaCode(t.getIaCode());
    std::stringstream ss;

    /* CROSS OVER */
    /**
    * 3 cas
    *
    * 2 Action : random entre les deux
    * 1 action 1 Decision : Decision en root et placement de l'action soit sur le node finale de gauche soit de droite
    * 2 Decision : random choix du root et du noeud gauche et noeud droit de l'autre tree
    */

    if (sIaCode[0] == '!' && sOtherIaCode[0] == '!')
    {
        return (std::rand() % 2 == 0 ? sIaCode : sOtherIaCode);
    }
    else if (sIaCode[0] != sOtherIaCode[0])
    {
        std::string* actionNode = &sIaCode;
        std::string* decisionNode = &sOtherIaCode;

        if (sIaCode[0] == '?')
            std::swap(decisionNode, actionNode);

        auto childFinder = decisionNode->begin() + 1;

        auto decisionNodeFinder = std::find(childFinder, decisionNode->end(), '?');
        auto actionNodeFinder = std::find(childFinder, decisionNode->end(), '!');

        auto startChild = decisionNodeFinder < actionNodeFinder ? decisionNodeFinder : actionNodeFinder;

        auto endChild = Factory::getBranch(startChild, decisionNode->end());

        if (rand() % 2 == 0)
        {
            //actionNode is left child of decisionNode
            std::string tmp(decisionNode->begin(), startChild);

            ss << tmp;
            ss << *actionNode;

            tmp.assign(endChild, decisionNode->end());
            ss << tmp;
        }
        else
        {
            //actionNode is right child of decisionNode
            std::string tmp(decisionNode->begin(), endChild);
            
            ss << tmp;
            ss << *actionNode;
        }
    }
    else
    {

        std::string* leftNode;
        std::string* rightNode;

        if (rand() % 2)
        {
            leftNode = &sIaCode;
            rightNode = &sOtherIaCode;
        }
        else
        {
            leftNode = &sOtherIaCode;
            rightNode = &sIaCode;
        }

        auto leftNodeStart = leftNode->begin() + 1;
        auto rightNodeStart = rightNode->begin() + 1;

        auto fDecisionNode = std::find(leftNodeStart, leftNode->end(), '?');
        auto fActionNode = std::find(leftNodeStart, leftNode->end(), '!');
        auto startLeftBranch = fDecisionNode < fActionNode ? fDecisionNode : fActionNode;

        {
            auto endBranch = Factory::getBranch(startLeftBranch, leftNode->end());
            std::string tmp(leftNode->begin(), endBranch);
            ss << tmp;
        }

        fDecisionNode = std::find(rightNodeStart, rightNode->end(), '?');
        fActionNode = std::find(rightNodeStart, rightNode->end(), '!');
        startLeftBranch = (fDecisionNode < fActionNode ? fDecisionNode : fActionNode) + 1;

        auto startRightBranch = Factory::getBranch(startLeftBranch, rightNode->end());

        {
            std::string tmp(startRightBranch, rightNode->end());
            ss << tmp;
        }
    }

    return ss.str();
}

std::string TreeIa::getIaCode() const
{
    return m_iaCode;
    //return m_root->getCode();
}

const INodeUPtr& TreeIa::getRoot() const
{
    return m_root;
}

INodeUPtr& TreeIa::getRoot()
{
    return m_root;
}

void TreeIa::setRoot(std::unique_ptr<INode>& root)
{
    m_root = std::move(root);
    m_iaCode = Factory::codeFromTree(*this);
}