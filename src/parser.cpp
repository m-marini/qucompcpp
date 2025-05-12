#include <iostream>
#include <map>

#include "parser.h"

using namespace std;
using namespace qg;
using namespace qp;

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

const gates_t qp::parseStream(istream &in)
{
    function<const optional<string>(void)> f = [&in](void)
    {
        char bfr[256];
        if (in.getline(bfr, sizeof(bfr)))
        {
            return optional(string(bfr));
        }
        return (const optional<string>)nullopt;
    };

    return Parser(f).parse();
}

const gates_t qp::parseString(const std::string &text)
{
    bool isFirstRead = true;
    function<const optional<string>(void)> f = [&isFirstRead, &text](void)
    {
        if (isFirstRead)
        {
            isFirstRead = false;
            return optional(text);
        }
        return (const optional<string>)nullopt;
    };

    return Parser(f).parse();
}

Parser::Parser(std::function<const std::optional<std::string>(void)> readLine) : _readLine(readLine),
                                                                                 _currentLine(std::optional(std::string())),
                                                                                 _lineNumber(0),
                                                                                 _currentPos(0)
{
    skipChar();
    consumeToken();
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
    if (peekToken())
    {
        reportError("Unexpected token " + peekToken().value());
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

    auto &tokOpt = peekToken();
    if (tokOpt)
    {
        auto gateId = tokOpt.value();
        if (map1bit.count(gateId) > 0)
        {
            auto builder = map1bit[gateId];
            consumeToken();
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

    auto tokOpt = peekToken();
    if (tokOpt)
    {
        auto gateId = tokOpt.value();
        if (map2bit.count(gateId) > 0)
        {
            auto builder = map2bit[gateId];
            consumeToken();
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

    auto tokOpt = peekToken();
    if (tokOpt)
    {
        auto gateId = tokOpt.value();
        if (map3bit.count(gateId) > 0)
        {
            auto builder = map3bit[gateId];
            consumeToken();
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
    auto &tokOpt = peekToken();
    if (!tokOpt || !isdigit(tokOpt.value().at(0)))
    {
        reportError("Expected digit");
    }
    const int result = stoi(tokOpt.value());
    consumeToken();
    return result;
}

void Parser::reportError(const string &msg) const
{
    cerr << _lineNumber << ":" << _currentPos << ": " << msg << endl
         << "|" << currentText() << endl;
    throw invalid_argument(msg);
}

void Parser::requiredToken(const string &token)
{
    auto &tokOpt = peekToken();
    if (!tokOpt || tokOpt.value() != token)
    {
        reportError("Expected " + token);
    }
    consumeToken();
}

void Parser::consumeToken(void)
{
    skipBlanks();
    int ch = currentChar();
    if (ch < 0)
    {
        _currentToken = nullopt;
    }
    else if (isalpha(ch))
    {
        string token;
        do
        {
            token += (char)ch;
            skipChar();
        } while (isalnum((ch = currentChar())));
        _currentToken = optional(token);
    }
    else if (isdigit(ch))
    {
        string token;
        do
        {
            token += (char)ch;
            skipChar();
        } while (isalnum((ch = currentChar())));
        _currentToken = optional(token);
    }
    else
    {
        skipChar();
        string result;
        result += (char)ch;
        _currentToken = optional(result);
    }
}

const string Parser::currentText() const
{
    return _currentLine ? _currentLine.value().substr(0, _currentPos + 1) : "<EOF>";
}

/**
 * Skips blanks from input
 */
void Parser::skipBlanks(void)
{
    while (isblank(currentChar()))
    {
        skipChar();
    }
}

/**
 * Skip a character
 */
void Parser::skipChar(void)
{
    if (_currentLine)
    {
        // not eof
        // Increment char pointer
        _currentPos++;
        auto line = _currentLine.value();
        if (_currentPos >= line.size())
        {
            // triggered eol: read next line
            _currentLine = _readLine();
            _lineNumber++;
            _currentPos = 0;
        }
    }
}