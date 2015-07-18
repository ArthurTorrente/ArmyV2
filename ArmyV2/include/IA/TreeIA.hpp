#ifndef _TREEIA_H_
#define _TREEIA_H_

#pragma warning(push, 0)
#include <memory>
#include <string>
#pragma warning(pop)

#include "Actions/Action.hpp"
#include "DecisionTree/INode.hpp"

class TreeIa
{
public:
    /**
     * Assign iaCode and root node to the new instance of TreeIA
     */
    TreeIa(INodeUPtr& root = INodeUPtr(nullptr));

    /**
     * Move constructor of TreeIa
     */
    TreeIa(TreeIa&&);

    /**
     * Operator =
     * Get the root node of TreeIa parameter and release the internal node if TreeIa has one
     */
    TreeIa& operator=(TreeIa&&);


    /**
     * Return the action computed by the decision tree
     */
    std::unique_ptr<Action> operator()(const UnitSPtr&, const ArmySPtr&, const ArmySPtr&);

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