#ifndef _DECISIONNODE_H_
#define _DECISIONNODE_H_

#include <memory>
#include <functional>

#include "INode.hpp"

#include "ActionNode.hpp"
#include "Army.hpp"

class DecisionNode : INode
{
public:
    DecisionNode();
    
    std::unique_ptr<Action> getValue();
protected:
    std::unique_ptr<INode> m_left;
    std::unique_ptr<INode> m_right;

    /**
     * Prédicat de choix récupération valeur de droite ou de gauche
     */
    std::function<bool()> m_goToLeftPredicate;
    /** Ou simplement une méthode vituel pur qui devra être redéfini par les class héritant de décision Node*/
    virtual bool operator()() = 0;
    /* Voir le plus simple */
};

#endif //_DECISIONNODE_H_