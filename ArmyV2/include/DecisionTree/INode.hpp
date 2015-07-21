#ifndef _INODE_H_
#define _INODE_H_

#pragma warning(push, 0)
#include <memory>
#include <functional>
#pragma warning(pop)

#include "Actions/Action.hpp"
#include "IACodeGetter.hpp"

/**
 * Forward declaration of Army and unit
 */
class Army;
class Unit;

typedef std::shared_ptr<Army> ArmySPtr;
typedef std::shared_ptr<Unit> UnitSPtr;

/**
 * Base of class of Node
 * It use for store any node into a TreeIa
 *
 * Implement getValue method into derived class
 */
class INode : public IACodeGetter
{
public:
    virtual std::unique_ptr<Action> getValue(Unit& unit, Army& a, Army& b) = 0;
};

/**
 * Different typedef for redefinition of type
 */
typedef std::unique_ptr<INode> INodeUPtr;
typedef std::unique_ptr<INode> INodeSPtr;

#endif //_INODE_H_