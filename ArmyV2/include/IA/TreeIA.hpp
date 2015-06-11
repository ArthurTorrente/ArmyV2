#ifndef _TREEIA_H_
#define _TREEIA_H_

#include <memory>
#include <string>

#include "Action.hpp"
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