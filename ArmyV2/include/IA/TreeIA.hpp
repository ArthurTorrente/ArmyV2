#ifndef _TREEIA_H_
#define _TREEIA_H_

#pragma warning(push, 0)
#include <memory>
#include <string>
#pragma warning(pop)

#include "Actions/Action.hpp"
#include "DecisionTree/INode.hpp"
#include "Army.hpp"

class TreeIa
{
public:
    /**
     * Assign iaCode and root node to the new instance of TreeIA
     */
    TreeIa(INodeUPtr& root);

    /**
     * Move constructor of TreeIa
     */
    TreeIa(TreeIa&&);

    /**
     * Return the action computed by the decision tree
     */
    std::unique_ptr<Action> operator()(const UnitSPtr&, const ArmyPtr&, const ArmyPtr&);

    /**
     * Getter of IACode
     */
    const std::string getIaCode() const;

    /**
    * Getter of root
    */
    const INodeUPtr& getRoot() const;

    /**
    * Setter of root
    */
    void setRoot(INodeUPtr& root);

protected:

    /**
     * Root node of tree
     */
    INodeUPtr m_root;
};

#endif //