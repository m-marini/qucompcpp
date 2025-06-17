#include "token.h"

using namespace std;
using namespace qc;

EOFToken *EOFToken::clone(void) const
{
    return new EOFToken(*this);
}

IntegerToken *IntegerToken::clone(void) const
{
    return new IntegerToken(*this);
}

RealToken *RealToken::clone(void) const
{
    return new RealToken(*this);
}

IdentifierToken *IdentifierToken::clone(void) const
{
    return new IdentifierToken(*this);
}

OperatorToken *OperatorToken::clone(void) const
{
    return new OperatorToken(*this);
}

const double RealToken::value(void) const
{
    return stod(token());
}

const int IntegerToken::value(void) const
{
    return stoi(token());
}

ostream &operator<<(ostream &os, const Token &token)
{
    return os << token.context() << endl;
}
