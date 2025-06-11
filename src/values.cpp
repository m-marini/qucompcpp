#include "values.h"
#include "matrix.h"

using namespace std;
using namespace qc;
using namespace mx;

const Value *IntValue::map(const ValueMapper &mapper) const
{
    return mapper.map(*this);
}

const Value *ComplexValue::map(const ValueMapper &mapper) const
{
    return mapper.map(*this);
}

const Value *MatrixValue::map(const ValueMapper &mapper) const
{
    return mapper.map(*this);
}

ListValue::~ListValue()
{
    for (const Value *val : _values)
    {
        delete val;
    }
}

const Value *ListValue::map(const ValueMapper &mapper) const
{
    return mapper.map(*this);
}

std::ostream &ListValue::write(std::ostream &stream) const
{
    stream << "(";
    bool first = true;
    for (const Value *el : _values)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            stream << ",";
        }
        el->write(stream);
    }
    return stream << ")";
}

const string to_string(const Value &value)
{
    stringstream stream;
    value.write(stream);
    return stream.str();
}

ostream &operator<<(ostream &stream, const Value &value)
{
    return value.write(stream);
}
