#include "sourceContext.h"

using namespace std;
using namespace qc;

const QuExecException SourceContext::execException(const string &id) const
{
    return QuExecException(id, *this);
}

const QuParseException SourceContext::parseException(const string &id) const
{
    return QuParseException(id, *this);
}

ostream &operator<<(ostream &os, const SourceContext &context)
{
    return os << "token: " << context.token() << endl
              << " line: " << context.line() << endl
              << "  num: " << context.lineNumber() << endl
              << "  pos: " << context.position() << endl;
}
