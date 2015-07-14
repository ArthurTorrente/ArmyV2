#ifndef _INODE_H_
#define _INODE_H_

#pragma warning(push, 0)
#include <memory>
#include <functional>
#pragma warning(pop)

#include "Actions/Action.hpp"
#include "Army.hpp"

class INode
{
public:
    virtual std::unique_ptr<Action> getValue(const UnitSPtr& unit, const ArmyPtr& a, const ArmyPtr& b) = 0;

    const std::string& getIaCode() const
    {
        return m_iaCode;
    }

    void setIaCode(const std::string& iaCode)
    {
        m_iaCode = iaCode;
    }

protected:
    std::string m_iaCode;
};

typedef std::unique_ptr<INode> INodeUPtr;
typedef std::unique_ptr<INode> INodeSPtr;

#endif //_INODE_H_