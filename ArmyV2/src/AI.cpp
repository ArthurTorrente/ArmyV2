#pragma warning(push, 0)
#include <iostream>
#include <memory>
#pragma warning(pop)

#include "IA/AI.hpp"
#include "Actions/Action.hpp"
#include "Actions/MoveAction.hpp"
#include "Actions/ShootAction.hpp"
#include "Actions/EmptyAction.hpp"
#include "Unit.hpp"
#include "Army.hpp"

//Parenthesis overloading for applying the AI on the unit provided in parameter
//Return the action which have to be done by the unit in this context
std::unique_ptr<Action> AI::operator()(Unit& unit, Army& allies, Army& opponents)
{
    tools::unusedArg(allies);

    try {
        if(unit.getFirerate().canShoot())
        {
            std::string code = unit.getIACode();
            Unit* target;
            if(code[1] == 'D')
            {
                if(code[0]=='L')
                    target = &(opponents.getNearestUnit(unit.getPosition()));
                else
                    target = &(opponents.getFurthestUnit(unit.getPosition()));
            }
            else
            {
                if(code[0]=='L')
                    target = &(opponents.getLowestUnit(static_cast<unsigned int>(code[1] - '0')));
                
                else
                    target = &(opponents.getHigestUnit(static_cast<unsigned int>(code[1] - '0')));
            }
            
            if(target->getPosition().distance(unit.getPosition())>unit.getRange().getValue())
            {
                return std::unique_ptr<Action>(new MoveAction(unit, target->getPosition()));
            }
            else
            {
                return std::unique_ptr<Action>(new ShootAction(unit, *target));
            }
        }
        else
        {
            Unit& target = opponents.getNearestUnit	(unit.getPosition());
            Point p = (2*unit.getPosition()) - target.getPosition();
            return std::unique_ptr<Action>(new MoveAction(unit, 1000*p));
        }
    }
    catch(std::invalid_argument&)
    {
        //std::cout<<e.what()<<std::endl;
        return std::unique_ptr<Action>(new EmptyAction(unit));
    }
}
