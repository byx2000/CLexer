#include "ParserCombinator.h"

using namespace std;
using namespace byx;

// Parser

byx::Parser::~Parser() {}

// Empty

ParseResult byx::Empty::parse(const std::string& input)
{
    return ParseResult::CreateSucceed("", input);
}

// Any

ParseResult byx::Any::parse(const std::string& input)
{
    if (input.size() == 0)
    {
        return ParseResult::CreateFail();
    }
    return ParseResult::CreateSucceed({ input[0] }, input.substr(1));
}

// Char

byx::Char::Char(char ch) : ch(ch) {}

ParseResult byx::Char::parse(const std::string& input)
{
    if (input.size() == 0 || input[0] != ch)
    {
        return ParseResult::CreateFail();
    }
    return ParseResult::CreateSucceed({ input[0] }, input.substr(1));
}

// CharSet

byx::CharSet::CharSet(const std::string& chs) : chs(chs) {}

ParseResult byx::CharSet::parse(const std::string& input)
{
    if (input.size() == 0)
    {
        return ParseResult::CreateFail();
    }
    for (size_t i = 0; i < chs.size(); ++i)
    {
        if (input[0] == chs[i])
        {
            return ParseResult::CreateSucceed({ input[0] }, input.substr(1));
        }
    }
    return ParseResult::CreateFail();
}

// Exclude

byx::Exclude::Exclude(const std::string& chs) : chs(chs) {}

ParseResult byx::Exclude::parse(const std::string& input)
{
    if (input.size() == 0)
    {
        return ParseResult::CreateFail();
    }
    for (size_t i = 0; i < chs.size(); ++i)
    {
        if (input[0] == chs[i])
        {
            return ParseResult::CreateFail();
        }
    }
    return ParseResult::CreateSucceed({ input[0] }, input.substr(1));
}

// Range

byx::Range::Range(char c1, char c2) : c1(c1), c2(c2) {}

ParseResult byx::Range::parse(const std::string& input)
{
    if (input.size() == 0)
    {
        return ParseResult::CreateFail();
    }
    if (((int)input[0] - (int)c1) * ((int)input[0] - (int)c2) <= 0)
    {
        return ParseResult::CreateSucceed({ input[0] }, input.substr(1));
    }
    return ParseResult::CreateFail();
}

// Prefix

byx::Prefix::Prefix(const std::string& pre) : pre(pre) {}

ParseResult byx::Prefix::parse(const std::string& input)
{
    for (size_t i = 0; i < pre.size(); ++i)
    {
        if (i >= input.size() || input[i] != pre[i])
        {
            return ParseResult::CreateFail();
        }
    }
    return ParseResult::CreateSucceed(pre, input.substr(pre.size()));
}

// Concat

byx::Concat::Concat(const std::vector<std::shared_ptr<Parser>>& parsers) : parsers(parsers) {}

byx::Concat::Concat(const std::initializer_list<std::shared_ptr<Parser>>& parsers)
{
    for (auto item : parsers)
    {
        this->parsers.push_back(item);
    }
}

ParseResult byx::Concat::parse(const std::string& input)
{
    string recognized = "", remaining = input;
    for (size_t i = 0; i < parsers.size(); ++i)
    {
        ParseResult res = parsers[i]->parse(remaining);
        if (!res)
        {
            return ParseResult::CreateFail();
        }
        recognized += res.getRecognized();
        remaining = res.getRemaining();
    }
    return ParseResult::CreateSucceed(recognized, remaining);
}

// ChooseOne

byx::ChooseOne::ChooseOne(const std::vector<std::shared_ptr<Parser>>& parsers) : parsers(parsers) {}

byx::ChooseOne::ChooseOne(const std::initializer_list<std::shared_ptr<Parser>>& parsers)
{
    for (auto item : parsers)
    {
        this->parsers.push_back(item);
    }
}

ParseResult byx::ChooseOne::parse(const std::string& input)
{
    for (size_t i = 0; i < parsers.size(); ++i)
    {
        ParseResult res = parsers[i]->parse(input);
        if (res)
        {
            return res;
        }
    }
    return ParseResult::CreateFail();
}

// ZeroOrMore

byx::ZeroOrMore::ZeroOrMore(const std::shared_ptr<Parser>& parser) : parser(parser) {}

ParseResult byx::ZeroOrMore::parse(const std::string& input)
{
    string recognized = "", remaining = input;
    ParseResult res = parser->parse(remaining);
    while (res)
    {
        recognized += res.getRecognized();
        remaining = res.getRemaining();
        res = parser->parse(remaining);
    }
    return ParseResult::CreateSucceed(recognized, remaining);
}

// OneOrMore

byx::OneOrMore::OneOrMore(const std::shared_ptr<Parser>& parser) : parser(parser) {}

ParseResult byx::OneOrMore::parse(const std::string& input)
{
    ParseResult res = parser->parse(input);
    if (!res)
    {
        return ParseResult::CreateFail();
    }

    string recognized = "";
    string remaining = "";
    while (res)
    {
        recognized += res.getRecognized();
        remaining = res.getRemaining();
        res = parser->parse(remaining);
    }
    return ParseResult::CreateSucceed(recognized, remaining);
}

// PrefixSet

byx::PrefixSet::PrefixSet(const std::vector<std::string>& pres) : pres(pres) {}

byx::PrefixSet::PrefixSet(const std::initializer_list<std::string>& pres)
{
    for (auto item : pres)
    {
        this->pres.push_back(item);
    }
}

ParseResult byx::PrefixSet::parse(const std::string& input)
{
    vector<shared_ptr<Parser>> parsers;
    for (size_t i = 0; i < pres.size(); ++i)
    {
        parsers.push_back(make_shared<Prefix>(pres[i]));
    }
    return ChooseOne(parsers).parse(input);
}

// CallbackOnSuccess

byx::CallbackOnSuccess::CallbackOnSuccess(const std::shared_ptr<Parser>& parser, const std::function<void(ParseResult)>& callback)
    : parser(parser), callback(callback) {}

ParseResult byx::CallbackOnSuccess::parse(const std::string& input)
{
    ParseResult res = parser->parse(input);
    if (res)
    {
        callback(res);
    }
    return res;
}

// CallbackOnFail

byx::CallbackOnFail::CallbackOnFail(const std::shared_ptr<Parser>& parser, const std::function<void(ParseResult)>& callback)
    : parser(parser), callback(callback) {}

ParseResult byx::CallbackOnFail::parse(const std::string& input)
{
    ParseResult res = parser->parse(input);
    if (!res)
    {
        callback(res);
    }
    return res;
}

// Lazy

byx::Lazy::Lazy(const std::function<std::shared_ptr<Parser>(void)> getParser) : getParser(getParser) {}

ParseResult byx::Lazy::parse(const std::string& input)
{
    return getParser()->parse(input);
}
