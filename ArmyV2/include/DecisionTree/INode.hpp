#ifndef _INODE_H_
#define _INODE_H_

#pragma warning(push, 0)
#include <memory>
#include <functional>
#pragma warning(pop)

#include "Actions/Action.hpp"
#include "IACodeGetter.hpp"

class Army;
class Unit;

typedef std::shared_ptr<Army> ArmySPtr;
typedef std::shared_ptr<Unit> UnitSPtr;

class INode : public IACodeGetter
{
public:
    virtual std::unique_ptr<Action> getValue(const UnitSPtr& unit, const ArmySPtr& a, const ArmySPtr& b) = 0;
};

typedef std::unique_ptr<INode> INodeUPtr;
typedef std::unique_ptr<INode> INodeSPtr;

#endif //_INODE_H_