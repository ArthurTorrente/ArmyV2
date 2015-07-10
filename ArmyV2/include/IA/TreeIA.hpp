#ifndef _TREEIA_H_
#define _TREEIA_H_

#pragma warning(push, 0)
#include <memory>
#include <string>
#pragma warning(pop)

#include "Actions/Action.hpp"
#include "Army.hpp"

class TreeIa
{
public:
    /**
     * Compute a decision tree with codeIa passed in constructor
     */
    TreeIa(const std::string& codeIa);

    /**
     * Return the action computed by the decision tree
     */
    std::unique_ptr<Action> operator()(const Unit&, const Army&, const Army&);
};

#endif //