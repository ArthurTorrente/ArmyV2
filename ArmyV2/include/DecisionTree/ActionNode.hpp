#ifndef _ACTIONNODE_H_
#define _ACTIONNODE_H_

#pragma warning(push, 0)
#include <memory>
#pragma warning(pop)

#include "INode.hpp"
#include "IACodeCollection.hpp"

template<typename OUT, typename EXTRACTOR>
class ActionNode : public INode
{
public:
    ActionNode(EXTRACTOR& ex)
        : m_extractor(std::move(ex))
    {}

    std::unique_ptr<Action> getValue(Unit& unit, Army& a, Army& b)
    {
        OUT* action;

        action = new OUT(unit, (*m_extractor)(unit, a, b));

        return std::unique_ptr<Action>(action);
    }

    void setExtractor(EXTRACTOR& ex)
    {
        m_extractor = std::move(ex);
    }

    std::string getCode() const
    {
        std::string code("!");

        code += IACodeCollection[typeid(OUT)];
        code += m_extractor->getCode();

        return code;
    }

protected:
    EXTRACTOR m_extractor;
};

template<typename OUT>
class ActionNodeWithOutExtractor : public INode
{
public:
    ActionNodeWithOutExtractor()
    {}

    std::unique_ptr<Action> getValue(Unit& unit, Army& a, Army& b)
    {
        tools::unusedArg(a, b);

        OUT* action;

        action = new OUT(unit);

        return std::unique_ptr<Action>(action);
    }

    std::string getCode() const
    {
        std::string code("!");

        code += IACodeCollection[typeid(OUT)];
        return code;
    }
};

template<typename ACTIONTYPE, typename EXTRACTOR>
ActionNode<ACTIONTYPE, EXTRACTOR>* getAction(EXTRACTOR& ex)
{
    return new ActionNode<ACTIONTYPE, EXTRACTOR>(ex);
}

template<typename ACTIONTYPE>
ActionNodeWithOutExtractor<ACTIONTYPE>* getAction()
{
    return new ActionNodeWithOutExtractor<ACTIONTYPE>();
}
#endif //_ACTIONNODE_H_