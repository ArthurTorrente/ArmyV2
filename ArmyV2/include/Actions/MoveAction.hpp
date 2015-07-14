#ifndef _MOVE_ACTION_HPP_
#define _MOVE_ACTION_HPP_

#pragma warning(push, 0)
#include <iostream>
#pragma warning(pop)

#include "Unit.hpp"
#include "Action.hpp"
#include "Point.hpp"



//Class representing the action to move a unit in direction of a specific position
class MoveAction : public Action {
private:
    Unit* unit_;
    Point position_;

public:

    //Constructor taking in paramters the unit to move and the destination
    MoveAction(Unit& unit, const Point& position)
        : unit_(&unit),
        position_(position)
    {}

    MoveAction(UnitSPtr unit, const Point& position)
        : unit_(unit.get()),
        position_(position)
    {}

    //Run the action
    //Log parameter indicate if we write something or not on the standard output
    void execute(bool log = false)
    {
        if(log)
            std::cout<<"Unit "<<unit_->getId()<<" move from "<<unit_->getPosition();
        
        unit_->moveToPosition(position_);

        if(log)
            std::cout<<" to "<<unit_->getPosition()<<std::endl;
    }
};

#endif