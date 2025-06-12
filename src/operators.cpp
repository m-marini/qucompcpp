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
    delete _other;
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
