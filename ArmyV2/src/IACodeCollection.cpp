#include "IACodeCollection.hpp"

#include "Actions/AllActions.hpp"

std::unordered_map<std::type_index, std::string> IACodeCollection = {
    { typeid(EmptyAction), std::string("N") },
    { typeid(MoveAction), std::string("M") },
    { typeid(EscapeAction), std::string("E") },
    { typeid(ShootAction), std::string("A") }
};