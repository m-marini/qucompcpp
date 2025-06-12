#include "processor.h"
#include "values.h"
#include "commands.h"
#include "matrix.h"
#include "operators.h"

using namespace std;
using namespace qc;
using namespace mx;

// -------- ary

static const Value *ary(const SourceContext &context, const int i, const int j)
{
    return new MatrixValue(Matrix::ary(i, j));
}

const ChainBinaryOperator &aryOper = *(new BinaryErrorOperator("Expected int, int, actual: "))
                                          ->mapIntIntValue(ary);

const static FunctionMapper aryMapper = [](const SourceContext &context, const ListValue &args)
{
    return aryOper.apply(context, *args.values().at(0), *args.values().at(1));
};

// -------- sqrt

static const Value *intSqrt(const SourceContext &context, const int arg)
{
    return new ComplexValue(sqrt((complex<double>)arg));
}

static const Value *complexSqrt(const SourceContext &context, const complex<double> &arg)
{
    return new ComplexValue(sqrt(arg));
}

const ChainUnaryOperator &sqrtOper = *(new UnaryErrorOperator("Unexpected value "))
                                          ->mapIntValue(intSqrt)
                                          ->mapComplexValue(complexSqrt);

const static FunctionMapper sqrtMapper = [](const SourceContext &context, const ListValue &args)
{
    return sqrtOper.apply(context, *args.values().at(0));
};

// -------- normalise

static const Value *intNormalise(const SourceContext &context, const int arg)
{
    return new IntValue(1);
}
#include <iostream>

static const Value *complexNormalise(const SourceContext &context, const complex<double> &arg)
{
    return new ComplexValue(arg / sqrt(norm(arg)));
}

static const Value *matrixNormalise(const SourceContext &context, const Matrix &arg)
{
    return new MatrixValue(arg);
}

const ChainUnaryOperator &normOper = *(new UnaryErrorOperator("Unexpected value "))
                                          ->mapIntValue(intNormalise)
                                          ->mapComplexValue(complexNormalise)
                                          ->mapMatrixValue(matrixNormalise);

const static FunctionMapper normMapper = [](const SourceContext &context, const ListValue &args)
{
    return normOper.apply(context, *args.values().at(0));
};

// -------- I

static const Value *intI(const SourceContext &context, const int arg)
{
    return new MatrixValue(Matrix::I(arg));
}

const ChainUnaryOperator &iOper = *(new UnaryErrorOperator("Expected integer value, actual "))
                                       ->mapIntValue(intI);

const static FunctionMapper iMapper = [](const SourceContext &context, const ListValue &args)
{
    return iOper.apply(context, *args.values().at(0));
};

// -------- X

static const Value *intX(const SourceContext &context, const int arg)
{
    return new MatrixValue(Matrix::I(arg));
}

const ChainUnaryOperator &xOper = *(new UnaryErrorOperator("Expected integer value, actual "))
                                       ->mapIntValue(intX);

const static FunctionMapper xMapper = [](const SourceContext &context, const ListValue &args)
{
    return xOper.apply(context, *args.values().at(0));
};

const static FunctionMapper nullMapper;

const map<string, FunctionDef> qc::QU_PROCESSOR_FUNCTIONS{
    {"sqrt", FunctionDef("sqrt", 1, sqrtMapper)},
    {"ary", FunctionDef("ary", 2, aryMapper)},
    {"sim", FunctionDef("sim", 2, nullMapper)},
    {"eps", FunctionDef("eps", 2, nullMapper)},
    {"I", FunctionDef("I", 1, iMapper)},
    {"H", FunctionDef("H", 1, nullMapper)},
    {"X", FunctionDef("X", 1, xMapper)},
    {"Y", FunctionDef("Y", 1, nullMapper)},
    {"Z", FunctionDef("Z", 1, nullMapper)},
    {"S", FunctionDef("S", 1, nullMapper)},
    {"T", FunctionDef("T", 1, nullMapper)},
    {"SWAP", FunctionDef("SWAP", 2, nullMapper)},
    {"CNOT", FunctionDef("CNOT", 2, nullMapper)},
    {"CCNOT", FunctionDef("CCNOT", 3, nullMapper)},
    {"qubit0", FunctionDef("qubit0", 2, nullMapper)},
    {"qubit1", FunctionDef("qubit1", 2, nullMapper)},
    {"normalise", FunctionDef("normalise", 1, normMapper)}};

static const Value *intNegate(const SourceContext &context, const int state)
{
    return new IntValue(-state);
}

static const Value *complexNegate(const SourceContext &context, const complex<double> &state)
{
    return new ComplexValue(-state);
}

static const Value *matrixNegate(const SourceContext &context, const Matrix &state)
{
    return new MatrixValue(-state);
}

static const Value *int2State(const SourceContext &context, const int state)
{
    return new MatrixValue(Matrix::ketBase(state));
}

static const Value *intDagger(const SourceContext &context, const int value)
{
    return new IntValue(value);
}

static const Value *complexDagger(const SourceContext &context, const complex<double> &value)
{
    return new ComplexValue(conj(value));
}

static const Value *matrixDagger(const SourceContext &context, const Matrix &value)
{
    return new MatrixValue(value.dagger());
}

const ChainUnaryOperator &int2StateOper = *(new UnaryErrorOperator("Expected integer value, actual: "))
                                               ->mapIntValue(int2State);

const ChainUnaryOperator &daggerOper = *(new UnaryErrorOperator("Unexpected value: "))
                                            ->mapIntValue(intDagger)
                                            ->mapComplexValue(complexDagger)
                                            ->mapMatrixValue(matrixDagger);

const ChainUnaryOperator &negOper = *(new UnaryErrorOperator("Unexpected value: "))
                                         ->mapIntValue(intNegate)
                                         ->mapComplexValue(complexNegate)
                                         ->mapMatrixValue(matrixNegate);

const Value *Processor::dagger(const SourceContext &source, const Value *arg)
{
    try
    {
        const Value *result = daggerOper.apply(source, *arg);
        delete arg;
        return result;
    }
    catch (QuExecException ex)
    {
        delete arg;
        throw;
    }
}

const Value *Processor::neg(const SourceContext &source, const Value *arg)
{
    try
    {
        const Value *result = negOper.apply(source, *arg);
        delete arg;
        return result;
    }
    catch (QuExecException ex)
    {
        delete arg;
        throw;
    }
}

const Value *Processor::int2Ket(const SourceContext &source, const Value *arg)
{
    try
    {
        const Value *result = int2StateOper.apply(source, *arg);
        delete arg;
        return result;
    }
    catch (QuExecException ex)
    {
        delete arg;
        throw;
    }
}

const Value *Processor::assign(const SourceContext &source, const string &id, const Value *arg)
{
    return _variables[id] = arg->clone();
    delete arg;
}

const Value *Processor::retrieveVar(const SourceContext &source, const string &id)
{
    if (_variables.count(id) == 0)
    {
        throw source.execException("Undefined variable " + id);
    }
    return _variables.at(id)->clone();
}

const Value *Processor::callFunction(const SourceContext &source, const string &id, const Value *args)
{
    try
    {
        const FunctionMapper &mapper = QU_PROCESSOR_FUNCTIONS.at(id).mapper();
        const Value *result = mapper(source, *(const ListValue *)args);
        delete args;
        return result;
    }
    catch (QuExecException ex)
    {
        delete args;
        throw;
    }
}

static const Value *crossMapper(const SourceContext &source, const Matrix &left, const Matrix &right)
{
    return new MatrixValue(left.cross(right));
};

static ChainBinaryOperator &crossOp = *(new BinaryErrorOperator("Expected matrix x matrix, actual: "))
                                           ->mapMatrixMatrixValue(crossMapper);

const Value *Processor::cross(const SourceContext &source, const Value *left, const Value *right)
{
    try
    {
        const Value *result = crossOp.apply(source, *left, *right);
        delete left;
        delete right;
        return result;
    }
    catch (QuExecException ex)
    {
        delete left;
        delete right;
        throw;
    }
}
