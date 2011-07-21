
#include "transform_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

//#include "TransitionGraph.hpp"
//#include "ParseGraph.hpp"

namespace poleng
{
namespace bonsai
{
    namespace puddle
    {

TransformAction::TransformAction(std::string aGroup, int aElement, std::string aRuleName)
{
    group = aGroup;
    element = aElement; // - 1;
//    std::cout << "Grupe zalozylem od " << start << " do " << end << " z glowa w: " << head << " dla reguly: " << aRuleName <<  std::endl;
    type = "transform";
    ruleName = aRuleName;
    verbose = false;
}

TransformAction::~TransformAction()
{
}

bool TransformAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
{


    int before = 0;
    int i = 0;
    while (i < (element - 1))
    {
        before += matchedTokensSize[i];
        i ++;
    }

    Group *gr = (Group*)(entities[currentEntity + before]);

    gr->setRuleName(ruleName);
    gr->setGroupType(group);

    Edges::iterator e = edges.begin();
    int index = -1;
    i = 0;
    while (e != edges.end())
    {
        if ((*e)->getId() == gr->getId())
        {
            index = i;
            (*e)->setLabel(group);
            break;
        }
        i ++;
        e ++;
    }

        if (index < 0)
        {
            std::cerr << "Could not find edge to transform. Id: " << gr->getId() << "." << std::endl;
            return false;
        }

    return true;
}

bool TransformAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
{
    if (entities.size() < element)
    {
        return false;
    }
    if (matchedTokensSize[element - 1] == 0)
    {
        std::cerr << "Element transform: " << element - 1 << " empty!" << std::endl;
        return false;
    }

    int before = 0;
    int i = 0;
    while (i < (element - 1))
    {
        before += matchedTokensSize[i];
        i ++;
    }

    if (entities[currentEntity + before]->getType() != "group")
    {
        return false;
    }
    return true;
}

std::string TransformAction::getGroup()
{
    return group;
}

int TransformAction::getElement()
{
    return element;
    //return (head + 1);
}

void TransformAction::setGroup(std::string aGroup)
{
    group = aGroup;
}

void TransformAction::setElement(int aElement)
{
    element = aElement; // - 1;
}

std::string TransformAction::getRuleName()
{
    return ruleName;
}

void TransformAction::setRuleName(std::string aRuleName)
{
    ruleName = aRuleName;
}

}

}

}
