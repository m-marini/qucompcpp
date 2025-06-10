#include <iostream>
#include <string>
#include <optional>
#include "tokenizer.h"

using namespace std;
using namespace qc;

#define LINE_BUFFER_SIZE (256)

Tokenizer::Tokenizer(istream &stream) : _stream(stream),
                                        _currentToken(NULL),
                                        _currentLine(optional("")),
                                        _tokenLine(nullopt),
                                        _tokenLineNumber(0),
                                        _tokenPos(0),
                                        _lineNumber(0)
{
}

Tokenizer &Tokenizer ::open(void)
{
    popChar();
    popToken();
    return *this;
}

const bool Tokenizer::eof(void) const
{
    return !_currentLine && !_currentToken;
}

void Tokenizer::popChar(void)
{
    if (!eof())
    {
        if (_currentLine)
        {
            _currentPos++;
            if (_currentPos >= _currentLine.value().length())
            {
                readNextLine();
            }
        }
    }
}

void Tokenizer::readNextLine(void)
{
    char buffer[LINE_BUFFER_SIZE];
    if (_stream.getline(buffer, sizeof(buffer) - 1))
    {
        string line(buffer);
        line += "\n";
        _currentLine = optional(line);
        _currentPos = 0;
        _lineNumber++;
    }
    else
    {
        _currentLine = nullopt;
    }
}

/**
 * Pops the current token and fetches next one
 */
void Tokenizer::popToken(void)
{
    if (!_currentToken || _currentToken->type() != TokenType::eofTokenType)
    {
        delete _currentToken;
        _currentToken = NULL;
        while (!_currentToken)
        {
            if (eof())
            {
                _currentToken = new EOFToken(createTokenContext(""));
                break;
            }
            _tokenLineNumber = _lineNumber;
            _tokenPos = _currentPos;
            _tokenLine = _currentLine;
            int ch = currentChar();
            if (isspace(ch))
            {
                skipBlanks();
            }
            else if (isdigit(ch))
            {
                parseNumber();
            }
            else if (isalpha(ch))
            {
                parseIdentifier();
            }
            else if (ch == '/')
            {
                parseSlash();
            }
            else
            {
                char buffer[] = {(char)ch, 0};
                _currentToken = new OperatorToken(createTokenContext(buffer));
                popChar();
            }
        }
    }
}

const int Tokenizer::currentChar(void) const
{
    return _currentLine ? _currentLine.value().at(_currentPos) : 0;
}

void Tokenizer::parseNumber(void)
{
    string bfr;
    do
    {
        bfr += (char)currentChar();
        popChar();
    } while (!eof() && isdigit(currentChar()));
    if (eof())
    {
        _currentToken = new IntegerToken(createTokenContext(bfr));
    }
    else
    {
        int ch = currentChar();
        if (ch == '.')
        {
            parseFract(bfr);
        }
        else if (ch == 'E' || ch == 'e')
        {
            parseExp(bfr);
        }
        else
        {
            _currentToken = new IntegerToken(createTokenContext(bfr));
        }
    }
}

void Tokenizer::parseFract(const string &prefix)
{
    string bfr(prefix);
    do
    {
        bfr += (char)currentChar();
        popChar();
    } while (!eof() && isdigit(currentChar()));
    if (eof())
    {
        _currentToken = new RealToken(createTokenContext(bfr));
    }
    else
    {
        char ch = currentChar();
        if (ch == 'E' || ch == 'e')
        {
            parseExp(bfr);
        }
        else
        {
            _currentToken = new RealToken(createTokenContext(bfr));
        }
    }
}

const SourceContext Tokenizer::createCharContext(void) const
{
    int ch = currentChar();

    string chStr;

    switch (ch)
    {
    case 0:
        chStr = "<eof>";
        break;
    case '\n':
        chStr = "\\n";
        break;
    case '\r':
        chStr = "\\r";
        break;
    case '\t':
        chStr = "\\t";
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
    case 12:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
        chStr = "\\0";
        chStr += to_string(ch);
        break;
    default:
        chStr = (char)ch;
    };
    return SourceContext(chStr, _currentLine.value_or("<eof>"), _lineNumber, _currentPos);
}

void Tokenizer::parseExp(const string &prefix)
{
    string bfr(prefix);
    bfr += (char)currentChar();
    popChar();
    if (eof())
    {
        throw createCharContext().parseException("Missing exponent");
    }
    int ch = currentChar();
    if (ch == '+' || ch == '-')
    {
        bfr += (char)ch;
        popChar();
    }
    if (eof())
    {
        throw createCharContext().parseException("Missing exponent");
    }
    ch = currentChar();
    if (!isdigit(ch))
    {
        throw createCharContext().parseException("Missing exponent");
    }
    do
    {
        bfr += (char)currentChar();
        popChar();
    } while (!eof() && isdigit(currentChar()));
    _currentToken = new RealToken(createTokenContext(bfr));
}

void Tokenizer::parseIdentifier(void)
{
    string bfr;
    do
    {
        bfr += (char)currentChar();
        popChar();
    } while (!eof() && isalnum(currentChar()));
    _currentToken = new IdentifierToken(createTokenContext(bfr));
}

void Tokenizer::parseSlash(void)
{
    popChar();
    if (eof())
    {
        _currentToken = new OperatorToken(createTokenContext("/"));
    }
    else
    {
        int ch = currentChar();
        if (ch == '/')
        {
            readNextLine();
        }
        else if (ch == '*')
        {
            skipComment();
        }
        else
        {
            _currentToken = new OperatorToken(createTokenContext("/"));
        }
    }
}

void Tokenizer::skipComment(void)
{
    popChar();
    while (!eof())
    {
        int ch = currentChar();
        popChar();
        if (!eof() && ch == '*' && currentChar() == '/')
        {
            popChar();
            break;
        }
    }
}

void Tokenizer::skipBlanks(void)
{
    while (!eof() && isspace(currentChar()))
    {
        popChar();
    }
}

const SourceContext Tokenizer::createTokenContext(const string &token) const
{
    return SourceContext(token,
                         _tokenLine ? _tokenLine.value().substr(0, _tokenLine.value().length() - 1) : "",
                         _tokenLineNumber, _tokenPos);
}
