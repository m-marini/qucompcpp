#include "syntaxBuilder.h"

using namespace std;
using namespace qc;

RuleMap::~RuleMap()
{
    for (const auto &[k, v] : _map)
    {
        delete v;
    }
}

const RuleMap SyntaxBuilder::build(void)
{
    map<const string, SyntaxRule *> result;
    // First pass generate rules
    for (const auto &[id, builder] : _builders)
    {
        result[id] = builder();
    }
    // Second pass bind rules
    for (const auto &[id, deps] : _binders)
    {
        vector<SyntaxRule *> rules;
        for (const string &ruleId : deps)
        {
            if (result.count(ruleId) == 0)
            {
                throw logic_error("Missing dependant rule " + ruleId + " for rule " + id);
            }
            SyntaxRule *rule = result[ruleId];
            rules.push_back(rule);
        }
        SyntaxRule *rule = result.at(id);
        rule->bind(rules);
    }

    return RuleMap(result);
}

SyntaxBuilder &SyntaxBuilder::empty(const string &id)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new EmptyRule(id); };

    _builders[id] = builder;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::eof(const string &id)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new EofRule(id); };

    _builders[id] = builder;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::intLiteral(const string &id)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new IntRule(id); };

    _builders[id] = builder;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::realLiteral(const string &id)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new RealRule(id); };

    _builders[id] = builder;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::oper(const string &id)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new OperRule(id); };

    _builders[id] = builder;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::id(const string &id)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new IdRule(id); };

    _builders[id] = builder;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::idIn(const string &id, const set<string> &idents)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id, idents]()
    { return new IdInRule(id, idents); };

    _builders[id] = builder;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::idNotIn(const string &id, const set<string> &idents)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id, idents]()
    { return new IdNotInRule(id, idents); };

    _builders[id] = builder;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::require(const string &id, const vector<string> &ruleIds)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new RequireRule(id); };

    _builders[id] = builder;
    _binders[id] = ruleIds;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::opt(const string &id, const vector<string> &ruleIds)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    if (ruleIds.size() < 1)
    {
        throw invalid_argument("Missing condition for rule " + id);
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new OptRule(id); };

    _builders[id] = builder;
    _binders[id] = ruleIds;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::options(const string &id, const vector<string> &ruleIds)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new OptionsRule(id); };

    _builders[id] = builder;
    _binders[id] = ruleIds;

    return *this;
}

SyntaxBuilder &SyntaxBuilder::repeat(const string &id, const string &ruleId)
{
    if (_builders.count(id) > 0)
    {
        throw invalid_argument("Rule " + id + " already defined");
    }
    function<SyntaxRule *(void)> builder = [id]()
    { return new RepeatRule(id); };

    _builders[id] = builder;
    _binders[id] = {ruleId};

    return *this;
}
