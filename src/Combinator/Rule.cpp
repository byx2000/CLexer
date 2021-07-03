#include "Rule.h"

using namespace byx;
using namespace std;

// Rule

byx::Rule::Rule(const std::shared_ptr<Parser>& p) : p(p) {}

Rule byx::Rule::Char(char c)
{
    return Rule(make_shared<byx::Char>(c));
}

Rule byx::Rule::CharSet(const std::string& chs)
{
    return Rule(make_shared<byx::CharSet>(chs));
}

Rule byx::Rule::Exclude(const std::string& chs)
{
    return Rule(make_shared<byx::Exclude>(chs));
}

Rule byx::Rule::Range(char c1, char c2)
{
    return Rule(make_shared<byx::Range>(c1, c2));
}

Rule byx::Rule::Prefix(const std::string& pre)
{
    return Rule(make_shared<byx::Prefix>(pre));
}

Rule byx::Rule::PrefixSet(const std::vector<std::string>& pres)
{
    return Rule(make_shared<byx::PrefixSet>(pres));
}

Rule byx::Rule::Lazy(const std::function<Rule(void)>& getRule)
{
    auto getParser = [getRule]()
    {
        return getRule().p;
    };

    return Rule(make_shared<byx::Lazy>(getParser));
}

Rule byx::Rule::zeroOrMore() const
{
    return Rule(make_shared<ZeroOrMore>(p));
}

Rule byx::Rule::oneOrMore() const
{
    return Rule(make_shared<OneOrMore>(p));
}

Rule byx::Rule::setCallback(const std::function<void(ParseResult)>& callback) const
{
    return Rule(make_shared<CallbackOnSuccess>(p, callback));
}

Rule byx::Rule::setCallbackOnFail(const std::function<void(ParseResult)>& callback) const
{
    return Rule(make_shared<CallbackOnFail>(p, callback));
}

Rule byx::Rule::operator+(const Rule& rule) const
{
    return Rule(make_shared<Concat>(vector<shared_ptr<Parser>>{ p, rule.p }));
}

Rule byx::Rule::operator|(const Rule& rule) const
{
    return Rule(make_shared<ChooseOne>(vector<shared_ptr<Parser>>{ p, rule.p }));
}

ParseResult byx::Rule::parse(const std::string& input)
{
    return p->parse(input);
}
