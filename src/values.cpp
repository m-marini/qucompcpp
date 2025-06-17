#include "values.h"
#include "matrix.h"

using namespace std;
using namespace qc;
using namespace mx;

ListValue::~ListValue()
{
    for (const Value *val : _values)
    {
        delete val;
    }
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

const string to_string(const ValueType &type)
{
    string t("unknown value type");
    switch (type)
    {
    case ValueType::intValueType:
        t = "integer";
        break;
    case ValueType::complexValueType:
        t = "complex";
        break;
    case ValueType::matrixValueType:
        t = "matrix";
        break;
    case ValueType::listValueType:
        t = "list";
        break;
    }
    return t;
}

std::ostream &operator<<(std::ostream &stream, const qc::ValueType &type)
{
    return stream << to_string(type);
}