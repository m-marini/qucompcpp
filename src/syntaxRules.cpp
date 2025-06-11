#include "syntaxRules.h"

using namespace std;
using namespace qc;


const bool EofRule::parse(TokenProducer &tokenizer, ParseContext &context) const
{
    return tokenizer.currentToken().type() == TokenType::eofTokenType;
}

const bool EmptyRule::parse(TokenProducer &tokenizer, ParseContext &context) const
{
    auto &token = tokenizer.currentToken();
    context.join(token, *this);
    return true;
}

const bool TerminalRule::parse(TokenProducer &tokenizer, ParseContext &context) const
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

const bool RequireRule::parse(TokenProducer &tokenizer, ParseContext &context) const
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

const bool OptionsRule::parse(TokenProducer &tokenizer, ParseContext &context) const
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

const bool OptRule::parse(TokenProducer &tokenizer, ParseContext &context) const
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

const bool RepeatRule::parse(TokenProducer &tokenizer, ParseContext &context) const
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
