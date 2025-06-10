#include "syntaxBuilder.h"

using namespace std;
using namespace qc;

const bool EofRule::parse(TokenizerIntf &tokenizer, ParseContext &context) const
{
    return tokenizer.currentToken().type() == TokenType::eofTokenType;
}

const bool EmptyRule::parse(TokenizerIntf &tokenizer, ParseContext &context) const
{
    auto &token = tokenizer.currentToken();
    context.join(token, *this);
    return true;
}

const bool TerminalRule::parse(TokenizerIntf &tokenizer, ParseContext &context) const
{
    auto &token = tokenizer.currentToken();
    if (match(token))
    {
        const Token *tmpTok = token.clone();
        tokenizer.popToken();
        context.join(*tmpTok, *this);
        delete tmpTok;
        return true;
    }
    return false;
}

const bool RequireRule::parse(TokenizerIntf &tokenizer, ParseContext &context) const
{
    Token *ruleToken = tokenizer.currentToken().clone();

    for (const SyntaxRule *rule : rules())
    {
        const Token &token = tokenizer.currentToken();
        auto ex = token.context().parseException("Missing " + rule->id());
        if (!rule->parse(tokenizer, context))
        {
            delete ruleToken;
            throw ex;
        }
    }
    context.join(*ruleToken, *this);
    delete ruleToken;
    return true;
}

const bool OptionsRule::parse(TokenizerIntf &tokenizer, ParseContext &context) const
{
    Token *ruleToken = tokenizer.currentToken().clone();

    for (const SyntaxRule *rule : rules())
    {
        const Token &token = tokenizer.currentToken();
        if (rule->parse(tokenizer, context))
        {
            context.join(*ruleToken, *this);
            delete ruleToken;
            return true;
        }
    }
    delete ruleToken;
    return false;
}

const bool OptRule::parse(TokenizerIntf &tokenizer, ParseContext &context) const
{
    const vector<SyntaxRule *> deps = rules();
    const SyntaxRule *cond = deps[0];

    const Token *ruleToken = tokenizer.currentToken().clone();
    if (!cond->parse(tokenizer, context))
    {
        delete ruleToken;
        return false;
    }

    for (int i = 1; i < deps.size(); i++)
    {
        const SyntaxRule *rule = deps[i];
        const Token &token = tokenizer.currentToken();
        auto ex = token.context().parseException("Missing " + rule->id());
        if (!rule->parse(tokenizer, context))
        {
            delete ruleToken;
            throw ex;
        }
    }
    context.join(*ruleToken, *this);
    delete ruleToken;
    return true;
}

const bool RepeatRule::parse(TokenizerIntf &tokenizer, ParseContext &context) const
{
    const vector<SyntaxRule *> deps = rules();
    const SyntaxRule *cond = deps[0];
    do
    {
    } while (cond->parse(tokenizer, context));
    return true;
}

const bool IntRule::match(const Token &token) const
{
    return token.type() == TokenType::integerTokenType;
}

const bool RealRule::match(const Token &token) const
{
    return token.type() == TokenType::realTokenType;
}

const bool OperRule::match(const Token &token) const
{
    return token.type() == TokenType::opTokenType && token.token() == id();
}

const bool IdRule::match(const Token &token) const
{
    return token.type() == TokenType::identifierTokenType && token.token() == id();
}

const bool IdInRule::match(const Token &token) const
{
    return token.type() == TokenType::identifierTokenType && _idents.count(token.token()) != 0;
}

const bool IdNotInRule::match(const Token &token) const
{
    return token.type() == TokenType::identifierTokenType && _idents.count(token.token()) == 0;
}

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
