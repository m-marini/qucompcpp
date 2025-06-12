#include "processor.h"
#include "values.h"
#include "commands.h"
#include "matrix.h"
#include "operators.h"

using namespace std;
using namespace qc;
using namespace mx;

const map<string, FunctionDef> qc::QU_PROCESSOR_FUNCTIONS{
    {"sqrt", FunctionDef("sqrt", 1)},
    {"ary", FunctionDef("ary", 2)},
    {"sim", FunctionDef("sim", 2)},
    {"eps", FunctionDef("eps", 2)},
    {"I", FunctionDef("I", 1)},
    {"H", FunctionDef("H", 1)},
    {"X", FunctionDef("X", 1)},
    {"Y", FunctionDef("Y", 1)},
    {"Z", FunctionDef("Z", 1)},
    {"S", FunctionDef("S", 1)},
    {"T", FunctionDef("T", 1)},
    {"SWAP", FunctionDef("SWAP", 2)},
    {"CNOT", FunctionDef("CNOT", 2)},
    {"CCNOT", FunctionDef("CCNOT", 3)},
    {"qubit0", FunctionDef("qubit0", 2)},
    {"qubit1", FunctionDef("qubit1", 2)},
    {"normalise", FunctionDef("normalise", 1)}};

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
