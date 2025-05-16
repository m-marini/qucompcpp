#include <iostream>
#include <string>

#include "parser.h"

using namespace std;
using namespace qp;

Tokenizer::Tokenizer(istream &stream) : _stream(stream),
                                        _currentLine(optional("")),
                                        _currentPos(0),
                                        _lineNum(0),
                                        _tokenLineNum(0),
                                        _tokenPos(0)
{
    popChar()
        .popToken();
}

const bool Tokenizer::eof(void) const
{
    return !_currentLine && !_currentToken;
}

void Tokenizer::popToken(void)
{
    _currentToken = nullopt;
    while (!_currentToken && !eof())
    {
        _tokenLineNum = _lineNum;
        _tokenPos = _currentPos;
        string &tok = _currentLine.value();
        _tokenLine = optional(tok.substr(0, tok.length() - 1));
        int ch = currentChar();
        if (isdigit(ch))
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
        else if (isspace(ch))
        {
            skipBlanks();
        }
        else
        {
            _currentToken = optional(string("") + (char)ch);
            popChar();
        }
    }
}

Tokenizer &Tokenizer::popChar(void)
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
    return *this;
}

void Tokenizer::readNextLine(void)
{
    char bfr[256];
    if (_stream.getline(bfr, sizeof(bfr)))
    {
        _currentLine = optional(string(bfr) + "\n");
        _currentPos = 0;
        _lineNum++;
    }
    else
    {
        _currentLine = nullopt;
    }
}

void Tokenizer::skipBlanks(void)
{
    while (!eof() && isspace(currentChar()))
    {
        popChar();
    }
}

void Tokenizer::parseNumber(void)
{
    string token;
    do
    {
        token += (char)currentChar();
        popChar();
    } while (!eof() && isdigit(currentChar()));
    _currentToken = optional(token);
}

void Tokenizer::parseIdentifier(void)
{
    string token;
    do
    {
        token += (char)currentChar();
        popChar();
    } while (!eof() && isalnum(currentChar()));
    _currentToken = optional(token);
}

void Tokenizer::parseSlash(void)
{
    popChar();
    if (eof())
    {
        _currentToken = optional("/");
    }
    else
    {
        char ch = currentChar();
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
            _currentToken = optional("/");
        }
    }
}

const int Tokenizer::currentChar(void) const
{
    return _currentLine ? _currentLine.value().at(_currentPos) : 0;
}

void Tokenizer::skipComment(void)
{
    popChar();
    while (!eof())
    {
        int ch = currentChar();
        popChar();
        if (ch == '*' && !eof() && currentChar() == '/')
        {
            popChar();
            break;
        }
    }
}
