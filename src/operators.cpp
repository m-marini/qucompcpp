#include <sstream>
#include <iostream>

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

ChainUnaryOperator *ChainUnaryOperator::mapInt(const IntMapperFunction &mapper) const
{
    return new UnaryIntOperator(mapper, this);
}

ChainUnaryOperator *ChainUnaryOperator::mapComplex(const ComplexMapperFunction &mapper) const
{
    return new UnaryComplexOperator(mapper, this);
}

ChainUnaryOperator *ChainUnaryOperator::mapMatrix(const MatrixMapperFunction &mapper) const
{
    return new UnaryMatrixOperator(mapper, this);
}

const Value *UnaryErrorOperator::apply(const SourceContext &context, const Value &value) const
{
    stringstream stream;
    stream << _format << " " << value.type();
    throw context.execException(stream.str());
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

ChainBinaryOperator *ChainBinaryOperator::mapIntInt(const IntIntMapperFunction &mapper) const
{
    return new IntIntOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapIntComplex(const IntComplexMapperFunction &mapper) const
{
    return new IntComplexOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapIntMatrix(const IntMatrixMapperFunction &mapper) const
{
    return new IntMatrixOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapComplexInt(const ComplexIntMapperFunction &mapper) const
{
    return new ComplexIntOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapComplexComplex(const ComplexComplexMapperFunction &mapper) const
{
    return new ComplexComplexOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapComplexMatrix(const ComplexMatrixMapperFunction &mapper) const
{
    return new ComplexMatrixOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapMatrixInt(const MatrixIntMapperFunction &mapper) const
{
    return new MatrixIntOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapMatrixComplex(const MatrixComplexMapperFunction &mapper) const
{
    return new MatrixComplexOperator(mapper, this);
}

ChainBinaryOperator *ChainBinaryOperator::mapMatrixMatrix(const MatrixMatrixMapperFunction &mapper) const
{
    return new MatrixMatrixOperator(mapper, this);
}

const Value *BinaryErrorOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    stringstream stream;
    stream << _format << " " << left.type() << ", " << right.type();
    throw context.execException(stream.str());
}

const Value *IntIntOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::intValueType && right.type() == ValueType::intValueType
               ? _mapper(context, ((const IntValue *)&left)->value(), ((const IntValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *IntComplexOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::intValueType && right.type() == ValueType::complexValueType
               ? _mapper(context, ((const IntValue *)&left)->value(), ((const ComplexValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *IntMatrixOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::intValueType && right.type() == ValueType::matrixValueType
               ? _mapper(context, ((const IntValue *)&left)->value(), ((const MatrixValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *ComplexIntOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::complexValueType && right.type() == ValueType::intValueType
               ? _mapper(context, ((const ComplexValue *)&left)->value(), ((const IntValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *ComplexComplexOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::complexValueType && right.type() == ValueType::complexValueType
               ? _mapper(context, ((const ComplexValue *)&left)->value(), ((const ComplexValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *ComplexMatrixOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::complexValueType && right.type() == ValueType::matrixValueType
               ? _mapper(context, ((const ComplexValue *)&left)->value(), ((const MatrixValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *MatrixIntOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::matrixValueType && right.type() == ValueType::intValueType
               ? _mapper(context, ((const MatrixValue *)&left)->value(), ((const IntValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *MatrixComplexOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::matrixValueType && right.type() == ValueType::complexValueType
               ? _mapper(context, ((const MatrixValue *)&left)->value(), ((const ComplexValue *)&right)->value())
               : _other->apply(context, left, right);
}

const Value *MatrixMatrixOperator::apply(const SourceContext &context, const Value &left, const Value &right) const
{
    return left.type() == ValueType::matrixValueType && right.type() == ValueType::matrixValueType
               ? _mapper(context, ((const MatrixValue *)&left)->value(), ((const MatrixValue *)&right)->value())
               : _other->apply(context, left, right);
}
