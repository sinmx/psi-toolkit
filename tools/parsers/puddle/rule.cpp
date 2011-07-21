
#include "rule.hpp"

#include <iostream>

#include "group_action.hpp"
#include <exception>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

Rule::Rule()
{
//    tokensPatterns = new std::vector<std::string>;
//    tokensModifiers = new std::vector<std::string>;
//    tokensRequired = new std::vector<bool>;
//    matchedTokensSize = new std::vector<int>;
//    matchedIndexes = new std::vector<int>;
    actions = ActionsPtr( new Actions );
//    actions = new Actions;
    pattern = PatternPtr( new Pattern );
    //pattern = new Pattern;
    repeat = false;

    left_ = "";
    match_ = "";
    right_ = "";
}

Rule::Rule(std::string aName, std::string aCompiled, int aLeftCount, int aMatchCount, int aRightCount, ActionsPtr aActions, std::vector<std::string> aTokensPatterns, std::vector<std::string> aTokensModifiers, std::vector<bool> aTokensRequired, std::vector<int> aMatchedIndexes, bool aRepeat, std::string aLeft, std::string aMatch, std::string aRight)
{
    tokensPatterns.insert(tokensPatterns.begin(), aTokensPatterns.begin(), aTokensPatterns.end());// = new std::vector<std::string>(aTokensPatterns);
    tokensModifiers.insert(tokensModifiers.begin(), aTokensModifiers.begin(), aTokensModifiers.end());// = new std::vector<std::string>(aTokensModifiers);
    tokensRequired.insert(tokensRequired.begin(), aTokensRequired.begin(), aTokensRequired.end());// = new std::vector<bool>(aTokensRequired);
//    matchedTokensSize = new std::vector<int>;
    matchedIndexes.insert(matchedIndexes.begin(), aMatchedIndexes.begin(), aMatchedIndexes.end());// = new std::vector<int>(aMatchedIndexes);
//    std::cerr << "3" << std::endl;

    pattern = PatternPtr( new Pattern );
    //pattern = new Pattern;
    actions = aActions;
//    actions = new Actions(aActions);

    name = aName;
    setPattern(aCompiled);
//    actions = aActions;
    leftCount = aLeftCount;
    matchCount = aMatchCount;
    rightCount = aRightCount;

    repeat = aRepeat;

    left_ = aLeft;
    match_ = aMatch;
    right_ = aRight;

//    tokensPatterns = aTokensPatterns;
//    tokensModifiers = aTokensModifiers;
//    tokensRequired = aTokensRequired;
//    matchedIndexes = aMatchedIndexes;
    matchedTokensSize.assign(tokensPatterns.size(), 0);
//    matchedTokensSize.assign(tokensPatterns.size(), 0);
}

Rule::~Rule()
{
//    Actions::iterator a = actions->begin();
//    while (a != actions->end())
//    {
//        if ((*a)->getType() == "group")
//            delete ((GroupAction*)(*a));
//        else if ((*a)->getType() == "delete")
//            delete ((DeleteAction*)(*a));
//        else if ((*a)->getType() == "add")
//            delete ((AddAction*)(*a));
//        else if ((*a)->getType() == "unify")
//            delete ((UnifyAction*)(*a));
//        else if ((*a)->getType() == "syntok")
//            delete ((SyntokAction*)(*a));
//        else if ((*a)->getType() == "join")
//            delete ((JoinAction*)(*a));
//        else if ((*a)->getType() == "attach")
//            delete ((AttachAction*)(*a));
//        else if ((*a)->getType() == "transform")
//            delete ((TransformAction*)(*a));
//        else
//            delete *a;
//        a ++;
//    }
//    delete actions;
//    std::cerr << "destruktor wywoluje" << std::endl;
//    delete tokensPatterns;
//    delete tokensModifiers;
//    delete tokensRequired;
//    delete matchedTokensSize;
//    delete matchedIndexes;
  //  delete pattern;
}

bool Rule::apply(std::string &sentence, Entities &entities, Edges &edges, int currentEntity)
{
    bool ret = false;
    Actions::iterator i = actions->begin();
    while (i != actions->end())
    {
//        Action *act = *i
//
//        std::cerr << "akcja: " << (*i)->getType() << std::endl;
        if ((*i)->apply(entities, edges, currentEntity, matchedTokensSize))
        {
            sentence = "";
  //          if (beforeMatch != "")
    //            sentence += beforeMatch;
            Entities::iterator e = entities.begin();// + currentEntity;
            while (e != entities.end())
            {
                sentence += (*e)->getCompiled();
                e ++;
            }
//            std::cout << "Sentencja po zmianie: " << sentence << std::endl;
            ret = true;
        }
        i ++;
    }
//    std::cout << "wychodze z reguly z wartoscia: " << (ret ? "tru" : "fols") << std::endl;
    return ret;
}

int Rule::matchPattern(std::string &sentence, int matchNumber, std::string &beforeMatched)
{
    std::string::const_iterator start = sentence.begin();
    std::string::const_iterator end = sentence.end();
    boost::match_results<std::string::const_iterator> matched;

    int matchCount = 0;
    boost::match_flag_type flags = boost::match_default | boost::match_not_dot_newline | boost::match_not_dot_null;
    std::string before;

//    std::cerr << "macz namber: " << matchNumber << std::endl;

//        std::cout << "Zdanie: " << sentence << std::endl;
//    std::cerr << "wzorzec reguly felernyj" << compiled << std::endl;
    try
    {
    while (boost::u32regex_search(start, end, matched, *pattern, flags))
    {
//        std::cerr << "Zdanie: " << sentence << std::endl;
//        std::cerr << "Dopasowadlo: " << matched[0] << std::endl;
        before += matched.prefix(); //before + matched.prefix();
        start = matched[0].second;
        flags |= boost::match_prev_avail;
        flags |= boost::match_not_bob;
        if (matchCount == matchNumber)
        {
            matching = matched[0];
            match = matched;
            if (matching == "")
                return -1;
            beforeMatched = before;
//            std::cerr << "jedynka: " << matched[1] << std::endl;
//            std::cout << "dwojka: " << matched[3] << std::endl;
//            std::string before = matched.prefix();
//            std::cerr << "Dopasowalem regule: " << name << std::endl;
//            std::cerr << "Dopasowalem wzorzec: " << compiled << std::endl;
//            std::cerr << "Dopasowane: " << matching << std::endl;
//            std::cout << "Przed: " << before << std::endl;
//            std::cout << "Zdanie: " << sentence << std::endl;
//            std::cerr << "before: " << before << std::endl;
//            std::cerr << "Zwracam: " << countEntities(before) << std::endl;
            return countEntities(before);
        }
        else
        {
            before += matched[0];
        }
        matchCount ++;
    }
    }
    catch (std::exception &e)
    {
//        std::cerr << "jakis wyjatek zlapalem: " << e.what() << std::endl;
        return -1;
    }
//    std::cout << "juz nie pasuje ta regula" << std::endl;

//    std::cout << "Nie dopasowalem reguly: " << name << std::endl;
//    std::cout << "Nie dopasowalem wzorca: " << compiled << std::endl;
//    std::cout << "Zdanie: " << sentence << std::endl;

//    if (matchCount > 0)
//    {
//        std::cout << "Tyle razy dopasowalem: " << matchCount << " regule: " << name << std::endl;
//    }

    return -1;
}

bool Rule::test(std::string &sentence, Entities &entities, int currentEntity)
{
//    std::cerr << "testuje regule: " << name << std::endl;
//    std::cerr << "ten luj: " << match[0] << std::endl;
    ///std::string::const_iterator start = sentence.begin();
    ///std::string::const_iterator end = sentence.end();
    ///boost::match_results<std::string::const_iterator> matched;
//    std::cout << "Zdanie: " << sentence << std::endl << "Wzor: " << pattern << std::endl;
    ///if (!boost::u32regex_search(start, end, matched, pattern, boost::match_default))
    ///{
    ///    return false;
    ///}

    ///if (countEntities(matched.prefix()) != currentEntity)
    ///{
    ///    return false;
    ///}

    ///std::string matching = matched[0];

//    matchedTokensSize.assign(tokensPatterns.size(), 0);
    //delete matchedTokensSize;
    //matchedTokensSize = new std::vector<int>;
    matchedTokensSize.clear();

    matchedTokensSize.assign(matchedIndexes.size(), 0);

//    std::cerr << "patterns: " << tokensPatterns.size() << std::endl;
//    std::cerr << "modifs: " << tokensModifiers.size() << std::endl;
//    std::cerr << "reqs: " << tokensRequired.size() << std::endl;
//    std::vector<std::string>::iterator xx = tokensPatterns.begin();
//    std::vector<std::string>::iterator yy = tokensModifiers.begin();
//    std::vector<bool>::iterator zz = tokensRequired.begin();
//    while (xx != tokensPatterns.end())
//    {
//        std::cerr << *xx << " " << *yy << " " << *zz << std::endl;
//        xx ++;
//        yy ++;
//        zz ++;
//    }
//    std::cout << "Maczing przed: " << matching << std::endl;
//    std::vector<std::string>::iterator it = tokensPatterns.begin();
    std::vector<int>::iterator it = matchedIndexes.begin();
    int i = 0;
    int j = 0;
//    while (it != tokensPatterns.end())
//    {
//        if (tokensRequired[i] == false)
//        {
//            i++;
//            it ++;
//            continue;
//        }
////        std::cout << "Runda pierwsza: " << *it << std::endl;
//
//        boost::u32regex expr = boost::make_u32regex(".*" + *it + ".*");
//        boost::u32regex exprReplace = boost::make_u32regex("" + *it);
//
////        std::cout << "pattern sratern: " << *it << std::endl;
//        std::string mod = tokensModifiers[i];
////        std::cout << "TAKI MOD: " << mod << "." << std::endl;
//        if (mod == "")
//        {
////            std::cout << "pasuje to gowno bez niczego" << std::endl;
//            //matchedTokensSize[i] = 1;
//            boost::smatch what;
//            boost::u32regex_match(matching, what, expr);
//            std::string ma = what[1];
//            matchedTokensSize[i] = countEntities(ma);
//            matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
//        }
//        else if (mod == "+")
//        {
////            std::cout << "pasuje to gowno z plusem" << std::endl;
////            std::cout << "To wycinam: " << *it << " z " << matching << std::endl;
//            if (boost::u32regex_match(matching, expr))
//            {
//                matchedTokensSize[i] ++;
////                std::cout << "Przed: " << matching << std::endl;
//                matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
////                std::cout << "Po: " << matching << std::endl;
//         //       continue;
//            }
//        }
//        i ++;
//        it ++;
//    }
//
////    std::cout << "Maczing po pierwszej rundzie: " << matching << std::endl;
//
//    it = tokensPatterns.begin();
//    i = 0;
//    while (it != tokensPatterns.end())
    while (it != matchedIndexes.end())
    {
//        std::cerr << "Numerex: " << *it << std::endl;
//        std::cerr << "a dwojka: " << match[0] << std::endl;
//        std::cerr << "i: " << i << "; Macz: " << match[*it] << std::endl;
        std::string part = match[*it];
        if ((part == "") && (tokensRequired[i]))
        {
//            std::cerr << "part pusty" << std::endl;
            return false;
        }
        matchedTokensSize.at(i) = countEntities(part);
        if (tokensRequired[i] && matchedTokensSize[i] == 0)
        {
//            std::cerr << "obowiazkowy element " << i << " dlaczemus jest pusty. nie bawie sie tak" << std::endl;
            return false;
        }
        j += matchedTokensSize.at(i);
//        boost::u32regex expr = boost::make_u32regex("^(" + *it + ").*");
//        boost::u32regex exprReplace = boost::make_u32regex("^" + *it);
//
//    //    std::cout << "pattern sratern: " << *it << std::endl;
//        std::string mod = tokensModifiers[i];
//        if (mod == "")
//        {
//            //matchedTokensSize[i] = 1;
//            boost::smatch what;
//            boost::u32regex_match(matching, what, expr);
//            std::string ma = what[1];
//            matchedTokensSize[i] = countEntities(ma);
//            matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
//        }
//        else if (mod == "?")
//        //if (mod == "?")
//        {
//            if (boost::u32regex_match(matching, expr))
//            {
//                matchedTokensSize[i] = 1;
//                matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
//            }
//            else
//            {
//                matchedTokensSize[i] = 0;
//            }
//        }
//        else if (mod == "+")
//        {
////            std::cout << "To wycinam: " << *it << " z " << matching << std::endl;
//            if (boost::u32regex_match(matching, expr))
//            {
//                matchedTokensSize[i] ++;
////                std::cout << "Przed: " << matching << std::endl;
//                matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
////                std::cout << "Po: " << matching << std::endl;
//                continue;
//            }
//        }
//        else if (mod == "*")
//        {
//            if (boost::u32regex_match(matching, expr))
//            {
//                matchedTokensSize[i] ++;
//                matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
//                continue;
//            }
//        }
        i ++;
        it ++;
    }
    if (j == 0)
    {
        return false;
    }
    //if (matched.prefix() != "")
    //{
//        beforeMatch = matched.prefix();
    //    return false;
    //}

//    std::cout << "Pasuje: " << matched[0] << " do: " << pattern << std::endl;
//    std::cout << "Natomiast liczebnosc dopasowanych elementow prezentuje sie nastepujaco:" << std::endl;
//    std::vector<int>::iterator im = matchedTokensSize->begin();
//    int x = 0;
//    while (im != matchedTokensSize->end())
//    {
////        std::cout << "Element " << x << ": " << *im << " klamotow" << std::endl;
//        x ++;
//        im ++;
//    }

//        std::cout << "pasuje mi: " << sentence << " do: " << pattern << std::endl;

    Actions::iterator ia = actions->begin();
    bool ret = true;
    while (ia != actions->end())
    {
//        Action *act = new Action(*ia);
        if ((*ia)->test(entities, currentEntity, matchedTokensSize) == false)
        {
            int limit;
            if (tokensModifiers.at(leftCount + matchCount - 1) == "+" || tokensModifiers.at(leftCount + matchCount - 1) == "")
                limit = 1;
            else
            {
                limit = 0;
            }
            if (matchedTokensSize.at(leftCount + matchCount - 1) > limit)
            {
                matchedTokensSize.at(leftCount + matchCount - 1) --;
                continue;
            }
            ret = false;
            break;
        }
//        delete *act;

        ia ++;
    }

//    if (ret)
//        std::cerr << "wychodze z testu z prawda" << std::endl;
//    else
//        std::cerr << "wychodze z testu z klamstwem" << std::endl;

    return ret;
}

int Rule::countEntities(std::string matched)
{
    int i = 0;
    int p = 0;
    int brackets = 0;

    while (i < matched.length())
    {
        if (matched[i] == '(')
            brackets ++;
        if (matched[i] == ')')
            brackets --;
        if (matched[i] == '<')
            if (i < matched.length())
            {
                if (matched[i + 1] == '<')
                {
                    if (brackets == 0)
                    {
                        p ++;
                        i ++;
                    }
                }
                else if (i > 0)
                {
                    if (matched[i - 1] == '(')
                    {
                        brackets --;
                        i ++;
                    }
                    else if (matched[i - 1] == ')')
                    {
                        brackets ++;
                        i ++;
                    }
                }
            }
        i ++;
    }

    return p;
//    std::string s = matched;
//    while (s.find("<<", p) != std::string::npos)
//    {
//        //if (s[p+2] == 'g')
//        //{
//        //     i ++;
//        //     p = s.find("<<t", p) + 3;
//        //     continue;
//        //}
//        i ++;
//        p = s.find("<<", p) + 2;
//    }
//    return i;
}

std::string Rule::getName()
{
    return name;
}

PatternPtr Rule::getPattern()
{
    return pattern;
}

std::string Rule::getCompiled()
{
    return compiled;
}

int Rule::getLeftCount()
{
    return leftCount;
}

int Rule::getMatchCount()
{
    return matchCount;
}

int Rule::getRightCount()
{
    return rightCount;
}

ActionsPtr Rule::getActions()
{
    return actions;
}

void Rule::setName(std::string aName)
{
    name = aName;
}

void Rule::setPattern(std::string aCompiled)
{
    //delete pattern;
    //pattern = new Pattern;

    *pattern = boost::make_u32regex(utf8converter::ToUtf8(utf8converter::FromUtf8(aCompiled)));
    //64: *pattern = boost::make_u32regex(aCompiled);
    compiled = aCompiled;
}

void Rule::setLeftCount(int aCount)
{
    leftCount = aCount;
}

void Rule::setMatchCount(int aCount)
{
    matchCount = aCount;
}

void Rule::setRightCount(int aCount)
{
    rightCount = aCount;
}

void Rule::setActions(Actions aActions)
{
    *actions = aActions;
}

void Rule::setRepeat(bool aRepeat)
{
    repeat = aRepeat;
}

bool Rule::getRepeat()
{
    return repeat;
}

void Rule::setMatch(std::string aMatch)
{
    match_ = aMatch;
}

void Rule::setLeft(std::string aLeft)
{
    left_ = aLeft;
}

void Rule::setRight(std::string aRight)
{
    right_ = aRight;
}

void Rule::setTokensPatterns(std::vector<std::string> aTokensPatterns)
{
    tokensPatterns.clear();
    tokensPatterns.insert(tokensPatterns.begin(), aTokensPatterns.begin(), aTokensPatterns.end());
//    delete tokensPatterns;
//    tokensPatterns = new std::vector<std::string>(aTokensPatterns);
    //delete matchedTokensSize;
    //matchedTokensSize = new std::vector<int>;
    matchedTokensSize.clear();
    matchedTokensSize.assign(tokensPatterns.size(), 0);
}

void Rule::setTokensModifiers(std::vector<std::string> aTokensModifiers)
{
    tokensModifiers.clear();
    tokensModifiers.insert(tokensModifiers.begin(), aTokensModifiers.begin(), aTokensModifiers.end());
//    delete tokensModifiers;
//    tokensModifiers = new std::vector<std::string>(aTokensModifiers);
}

void Rule::setTokensRequired(std::vector<bool> aTokensRequired)
{
    tokensRequired.clear();
    tokensRequired.insert(tokensRequired.begin(), aTokensRequired.begin(), aTokensRequired.end());
//    delete tokensRequired;
//    tokensRequired = new std::vector<bool>(aTokensRequired);
}

void Rule::setMatchedIndexes(std::vector<int> aMatchedIndexes)
{
    matchedIndexes.clear();
    matchedIndexes.insert(matchedIndexes.begin(), aMatchedIndexes.begin(), aMatchedIndexes.end());
//    delete matchedIndexes;
//    matchedIndexes = new std::vector<int>(aMatchedIndexes);
}

std::string Rule::getMatch()
{
    return match_;
}

std::string Rule::getLeft()
{
    return left_;
}

std::string Rule::getRight()
{
    return right_;
}

void Rule::addAction(ActionPtr action)
{
    actions->push_back(action);
}

void Rule::deleteAction(int index)
{
    if (index < actions->size())
        actions->erase(actions->begin() + index);
}

std::string Rule::makeReadable()
{
    std::stringstream ss;
    ss << "Rule \"" << name << "\"" << std::endl;

    if (leftCount > 0)
    {
        ss << left_ << std::endl;
    }
    ss << match_ << std::endl;
    if (rightCount > 0)
    {
        ss << right_ << std::endl;
    }
    ss << "Eval: ";
    for (Actions::iterator a = actions->begin(); a != actions->end(); a ++)
    {
        if ((*a)->getType() == "delete")
            ss << "delete(" << (boost::dynamic_pointer_cast<DeleteAction>(*a))->getUPattern() << ", " << (boost::dynamic_pointer_cast<DeleteAction>(*a))->getTokenIndex() << ");" << std::endl;
        else if ((*a)->getType() == "add")
        {
            ss << "add(" << (boost::dynamic_pointer_cast<AddAction>(*a))->getUInterpretation() << ", " << "\"";
            if ((boost::dynamic_pointer_cast<AddAction>(*a))->getBase() != "[^<>]+")
                ss << (boost::dynamic_pointer_cast<AddAction>(*a))->getBase();
            else
                ss << "base";
            ss << "\"" << ", " << (boost::dynamic_pointer_cast<AddAction>(*a))->getTokenIndex() << ");" << std::endl;
        }
        else if ((*a)->getType() == "unify")
        {
            ss << "unify(";
            std::vector<std::string> attributes = (boost::dynamic_pointer_cast<UnifyAction>(*a))->getUAttributes();
            std::vector<int> indexes = (boost::dynamic_pointer_cast<UnifyAction>(*a))->getTokenIndexes();

            for (std::vector<std::string>::iterator it = attributes.begin(); it != attributes.end(); it ++)
            {
                if (it != attributes.begin())
                    ss << " ";
                ss << *it;
            }
            for (std::vector<int>::iterator it = indexes.begin(); it != indexes.end(); it ++)
                ss << ", " << *it;
            ss << ");" << std::endl;
        }
        else if ((*a)->getType() == "syntok")
        {
            ss << "syntok(\"" << (boost::dynamic_pointer_cast<SyntokAction>(*a))->getUMorphology() << "\");" << std::endl;
        }
        else if ((*a)->getType() == "group")
            ss << "group(" << (boost::dynamic_pointer_cast<GroupAction>(*a))->getGroup() << ", " << (boost::dynamic_pointer_cast<GroupAction>(*a))->getHead() << ");" << std::endl;
        else if ((*a)->getType() == "join")
            ss << "join(" << (boost::dynamic_pointer_cast<JoinAction>(*a))->getGroup() << ", " << (boost::dynamic_pointer_cast<JoinAction>(*a))->getHead() << ");" << std::endl;
        else if ((*a)->getType() == "attach")
            ss << "attach(" << (boost::dynamic_pointer_cast<AttachAction>(*a))->getGroup() << ", " << (boost::dynamic_pointer_cast<AttachAction>(*a))->getHead() << ");" << std::endl;
        else if ((*a)->getType() == "transform")
            ss << "transform(" << (boost::dynamic_pointer_cast<TransformAction>(*a))->getElement() << ", " << (boost::dynamic_pointer_cast<TransformAction>(*a))->getGroup() << ");" << std::endl;
    }

    return ss.str();
}

std::string Rule::log()
{
    std::stringstream ss;

    ss << this->getName() << std::endl;
    ss << this->getCompiled() << std::endl;

    if (this->getLeftCount() > 0)
        ss << "Left: 1 - " << this->getLeftCount() << std::endl;
    ss << "Match: " << (this->getLeftCount() + 1) << " - " << (this->getLeftCount() + this->getMatchCount()) << std::endl;
    if (this->getRightCount() > 0)
        ss << "Right: " << (this->getLeftCount() + this->getMatchCount() + 1) << " - " << (this->getLeftCount() + this->getMatchCount() + this->getRightCount()) << std::endl;

    if (this->getRepeat())
        ss << "Allows repeating" << std::endl;
    ActionsPtr actions = this->getActions();
    for (Actions::iterator a = actions->begin(); a != actions->end(); a ++)
    {
        if ((*a)->getType() == "delete")
            ss << "Delete " << (boost::dynamic_pointer_cast<DeleteAction>(*a))->getTokenIndex() << " " << (boost::dynamic_pointer_cast<DeleteAction>(*a))->getPattern() << std::endl;
        else if ((*a)->getType() == "add")
        {
            //std::vector<InterpretationPair> interps = ((AddAction*)(*a))->getInterpretations();
            std::vector<InterpretationPair> interps = (boost::dynamic_pointer_cast<AddAction>(*a))->getInterpretations();
            std::vector<InterpretationPair>::iterator it = interps.begin();
            while (it != interps.end())
            {
                //logfile << "Add " << ((AddAction*)*a)->getTokenIndex() << " " << it->second << ((AddAction*)*a)->getBase() << std::endl;
                ss << "Add " << (boost::dynamic_pointer_cast<AddAction>(*a))->getTokenIndex() << " " << it->second << (boost::dynamic_pointer_cast<AddAction>(*a))->getBase() << std::endl;
                it ++;
            }
        }
        else if ((*a)->getType() == "group")
            //logfile << "Group " << ((GroupAction*)*a)->getGroup() << " " << (((GroupAction*)*a)->getStart() + 1) << " - " << (((GroupAction*)*a)->getEnd() + 1) << " head: " << ((GroupAction*)*a)->getHead() << std::endl;
            ss << "Group " << (boost::dynamic_pointer_cast<GroupAction>(*a))->getGroup() << " " << ((boost::dynamic_pointer_cast<GroupAction>(*a))->getStart() + 1) << " - " << ((boost::dynamic_pointer_cast<GroupAction>(*a))->getEnd() + 1) << " head: " << (boost::dynamic_pointer_cast<GroupAction>(*a))->getHead() << std::endl;
        else if ((*a)->getType() == "unify")
        {
            ss << "Unify ";
            std::vector<int> tokenIndexes = (boost::dynamic_pointer_cast<UnifyAction>(*a))->getTokenIndexes();
            std::vector<int> attributeIndexes = (boost::dynamic_pointer_cast<UnifyAction>(*a))->getAttributeIndexes();
            std::vector<int>::iterator it = tokenIndexes.begin();
            ss << "entities: ";
            while (it != tokenIndexes.end())
            {
                ss << *it;
                it ++;
                if (it != tokenIndexes.end())
                    ss << ", ";
            }
            it = attributeIndexes.begin();
            ss << " attributes: ";
            while (it != attributeIndexes.end())
            {
                ss << *it;
                it ++;
                if (it != attributeIndexes.end())
                    ss << ", ";
            }
            ss << std::endl;
        }
        else if ((*a)->getType() == "syntok")
        {
            ss << "Syntok ";
            std::vector<InterpretationPair> morphos = (boost::dynamic_pointer_cast<SyntokAction>(*a))->getMorphology();
            std::string m = morphos.front().first;
            ss << m.substr(m.find(":", 1) + 1, std::string::npos);
            //                logfile << " base=";
            //                std::vector<int> indexes = ((SyntokAction*)*a)->getTokenIndexes();
            //                std::vector<int>::iterator i = indexes.begin();
            //                while (i != indexes.end())
            //                {
            //                    logfile << *i;
            //                    i ++;
            //                    if (i != indexes.end())
            //                        logfile << ".";
            //                }
            ss << std::endl;
        }
        else if ((*a)->getType() == "join")
            ss << "Join " << (boost::dynamic_pointer_cast<JoinAction>(*a))->getGroup() << " " << ((boost::dynamic_pointer_cast<JoinAction>(*a))->getStart() + 1) << " - " << ((boost::dynamic_pointer_cast<JoinAction>(*a))->getEnd() + 1) << " head: " << (boost::dynamic_pointer_cast<JoinAction>(*a))->getHead() << std::endl;
        //logfile << "Join " << ((JoinAction*)*a)->getGroup() << " " << (((JoinAction*)*a)->getStart() + 1) << " - " << (((JoinAction*)*a)->getEnd() + 1) << " head: " << ((JoinAction*)*a)->getHead() << std::endl;
        else if ((*a)->getType() == "attach")
            ss << "Attach " << (boost::dynamic_pointer_cast<AttachAction>(*a))->getGroup() << " " << ((boost::dynamic_pointer_cast<AttachAction>(*a))->getStart() + 1) << " - " << ((boost::dynamic_pointer_cast<AttachAction>(*a))->getEnd() + 1) << " head: " << (boost::dynamic_pointer_cast<AttachAction>(*a))->getHead() << std::endl;
        else if ((*a)->getType() == "transform")
            ss << "Transform " << (boost::dynamic_pointer_cast<TransformAction>(*a))->getElement() << " " << " into " << (boost::dynamic_pointer_cast<TransformAction>(*a))->getGroup() << std::endl;
        else
            std::cerr << "Unknown action type: " << (*a)->getType() << "." << std::endl;
    }
//    ss << std::endl;
    return ss.str();
}


}

}

}

//int main(int argc, char **argv)
//{
//    return 0;
//}