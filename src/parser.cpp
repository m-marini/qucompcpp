#include <iostream>
#include <sstream>
#include <map>

#include "parser.h"

using namespace std;
using namespace qg;
using namespace qp;

/*
const gates_t qp::parseArgs(const int argc, const char **argv)
{
    int counter = 1;
    function<const optional<string>(void)> f = [argc, argv, &counter](void)
    {
        if (counter < argc)
        {
            return optional(string(argv[counter++]));
        }
        return (const optional<string>)nullopt;
    };

    return Parser(f).parse();
}

*/
const gates_t qp::parseStream(istream &in)
{
    Tokenizer t(in);
    Parser p(t);
    return p.parse();
}

const gates_t qp::parseString(const std::string &text)
{
    auto is = istringstream(text);
    return parseStream(is);
}

/**
 * <gates> ::= <gate> <opt-gate>*
 * <opt-gate> ::= "" | <gate>
 *
 * <gate> :: = <unary-gate> | <binary-gate> | <ternary-gate>
 *
 * <unary-gate> := <unary-id> "(" <number>")"
 * <unary-id> := "i" | "h" | "x" | "y" | "z" | "s" | "t"
 *
 * <binary-gate> := <binary-id> "(" <number> "," <number> ")"
 * <binary-id> := "cnot" | "swap"
 *
 * <ternary-gate> := <ternary-gate> "(" <number> "," <number> "," <number> ")"
 * <ternary-id> := "ccnot"
 */

const gates_t Parser::parse(void)
{
    auto gateOpt = parseGateOpt();
    if (!gateOpt)
    {
        reportError("Missing gate");
    }
    gates_t result;
    do
    {
        result.insert(result.end(), gateOpt.value());
    } while (gateOpt = parseGateOpt());
    if (_tokenizer.currentToken())
    {
        reportError("Unexpected token " + _tokenizer.currentToken().value_or("EOF"));
    }
    return result;
}

const optional<QuGate> Parser::parseGateOpt(void)
{
    optional<QuGate> result = parseUnaryGate();
    if (!result)
    {
        if (!(result = parseBinaryGate()))
        {
            result = parseTrinaryGate();
        }
    }
    return result;
}

/**
 * <unary-gate> := <unary-id> "(" <number>")"
 * <unary-id> := "i" | "h" | "x" | "y" | "z" | "s" | "t"
 */
const optional<QuGate> Parser::parseUnaryGate(void)
{
    /**
     * Dictionary of gate factories
     */
    static map<string, function<QuGate(const size_t)>> map1bit{
        {"i", QuGate::i},
        {"h", QuGate::h},
        {"s", QuGate::s},
        {"t", QuGate::t},
        {"x", QuGate::x},
        {"y", QuGate::y},
        {"z", QuGate::z}};

    auto &tokOpt = _tokenizer.currentToken();
    if (tokOpt)
    {
        auto gateId = tokOpt.value();
        if (map1bit.count(gateId) > 0)
        {
            auto builder = map1bit[gateId];
            _tokenizer.popToken();
            requiredToken("(");
            const int bit = parseNumber();
            requiredToken(")");
            return optional(builder(bit));
        }
    }
    return nullopt;
}

/**
 * <binary-gate> := <binary-id> "(" <number> "," <number> ")"
 * <binary-id> := "cnot" | "swap"
 */
const optional<QuGate> Parser::parseBinaryGate(void)
{
    static map<string, function<QuGate(const size_t, const size_t)>> map2bit{
        {"swap", QuGate::swap},
        {"cnot", QuGate::cnot}};

    auto tokOpt = _tokenizer.currentToken();
    if (tokOpt)
    {
        auto gateId = tokOpt.value();
        if (map2bit.count(gateId) > 0)
        {
            auto builder = map2bit[gateId];
            _tokenizer.popToken();
            requiredToken("(");
            const int bit0 = parseNumber();
            requiredToken(",");
            const int bit1 = parseNumber();
            requiredToken(")");
            return optional(builder(bit0, bit1));
        }
    }
    return nullopt;
}

/**
 * <ternary-gate> := <ternary-gate> "(" <number> "," <number> "," <number> ")"
 * <ternary-id> := "ccnot"
 */
const optional<QuGate> Parser::parseTrinaryGate(void)
{
    static map<string, function<QuGate(const size_t, const size_t, const size_t)>> map3bit{
        {"ccnot", QuGate::ccnot}};

    auto tokOpt = _tokenizer.currentToken();
    if (tokOpt)
    {
        auto gateId = tokOpt.value();
        if (map3bit.count(gateId) > 0)
        {
            auto builder = map3bit[gateId];
            _tokenizer.popToken();
            requiredToken("(");
            const int bit0 = parseNumber();
            requiredToken(",");
            const int bit1 = parseNumber();
            requiredToken(",");
            const int bit2 = parseNumber();
            requiredToken(")");
            return optional(builder(bit0, bit1, bit2));
        }
    }
    return nullopt;
}

const int Parser::parseNumber(void)
{
    auto &tokOpt = _tokenizer.currentToken();
    if (!tokOpt || !isdigit(tokOpt.value().at(0)))
    {
        reportError("Expected digit");
    }
    const int result = stoi(tokOpt.value());
    _tokenizer.popToken();
    return result;
}

void Parser::reportError(const string &msg) const
{
    string lineNum = to_string(_tokenizer.tokenLineNum());
    string lineNumGap = string().assign(lineNum.length(), ' ');
    string skipGap = string().assign(_tokenizer.tokenPos(), '-');
    cerr << lineNum << ":" << _tokenizer.tokenLine().value_or("") << endl
         << lineNumGap << ":" << skipGap << "^ " << msg << endl;
    throw invalid_argument(msg);
}

void Parser::requiredToken(const string &token)
{
    auto &tokOpt = _tokenizer.currentToken();
    if (!tokOpt || tokOpt.value() != token)
    {
        reportError("Expected " + token);
    }
    _tokenizer.popToken();
}
