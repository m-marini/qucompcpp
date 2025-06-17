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

ostream &SourceContext::write(ostream &out, const string &message) const
{
    string lineNum = to_string(_lineNumber);
    string lineNumFiller(lineNum.size(), ' ');
    string posFiller(_position, '-');
    return out << lineNum << ":" << _line << endl
               << lineNumFiller << ":" << posFiller << "^ " << message << endl;
    /*
    return out << lineNum << ":" << _line << endl
               << lineNumFiller << ":" << posFiller << "^ " << message << " token(\"" << _token << "\")" << endl;
               */
}

ostream &operator<<(ostream &os, const SourceContext &context)
{
    return context.write(os, "");
}

ostream &operator<<(ostream &os, const QuSourceException &ex)
{
    return ex.context().write(os, ex.what());
    /*
    const SourceContext ctx = ex.context();
    string lineNum = to_string(ctx.lineNumber());
    string lineNumFiller(lineNum.size(), ' ');
    string posFiller(ctx.position(), '-');
    return os << lineNum << ":" << ctx.line() << endl
              << lineNumFiller << ":" << posFiller << "^ " << ex.what() << " token(\"" << ctx.token() << "\")" << endl;
              */
}
