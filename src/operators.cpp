#include <format>

#include "sourceContext.h"
#include "values.h"
#include "operators.h"
#include "matrix.h"

using namespace std;
using namespace qc;
using namespace mx;

ChainUnaryOperator::~ChainUnaryOperator()
{
    if (_other)
    {
        delete _other;
    }
}

ChainUnaryOperator *ChainUnaryOperator::mapIntValue(const IntMapperFunction &mapper) const
{
    return new UnaryIntOperator(mapper, this);
}

ChainUnaryOperator *ChainUnaryOperator::mapComplexValue(const ComplexMapperFunction &mapper) const
{
    return new UnaryComplexOperator(mapper, this);
}

ChainUnaryOperator *ChainUnaryOperator::mapMatrixValue(const MatrixMapperFunction &mapper) const
{
    return new UnaryMatrixOperator(mapper, this);
}

const Value *UnaryErrorOperator::apply(const SourceContext &context, const Value &value) const
{
    throw context.execException(_format + to_string(value));
}

const Value *UnaryIntOperator::apply(const SourceContext &context, const Value &value) const
{
    return value.type() == ValueType::intValueType
               ? _mapper(context, ((const IntValue *)&value)->value())
               : _other->apply(context, value);
}

const Value *UnaryComplexOperator::apply(const SourceContext &context, const Value &value) const
{
    return value.type() == ValueType::complexValueType
               ? _mapper(context, ((const ComplexValue *)&value)->value())
               : _other->apply(context, value);
}

const Value *UnaryMatrixOperator::apply(const SourceContext &context, const Value &value) const
{
    return value.type() == ValueType::matrixValueType
               ? _mapper(context, ((const MatrixValue *)&value)->value())
               : _other->apply(context, value);
}

ChainBinaryOperator::~ChainBinaryOperator()
{
    if (_other)
    {
        delete _other;
    }
}

ChainBinaryOperator *ChainBinaryOperator::mapMatrixMatrixValue(const MatrixMatrixMapperFunction &mapper) const
{
    return new MatrixMatrixOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapIntIntValue(const IntIntMapperFunction &mapper) const
{
    return new IntIntOperator(mapper, this);
}

const Value *BinaryErrorOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    throw context.execException(_format + to_string(left) + ", " + to_string(right));
}

const Value *MatrixMatrixOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::matrixValueType && right.type() == ValueType::matrixValueType
               ? _mapper(context, ((const MatrixValue *)&left)->value(), ((const MatrixValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *IntIntOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::intValueType && right.type() == ValueType::intValueType
               ? _mapper(context, ((const IntValue *)&left)->value(), ((const IntValue *)&right)->value())
               : _other->apply(context, left, right);
}
