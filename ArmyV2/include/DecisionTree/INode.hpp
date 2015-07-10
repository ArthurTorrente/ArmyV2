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
    virtual std::unique_ptr<Action> getValue() = 0;
};

#endif //_INODE_H_