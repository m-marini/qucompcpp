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

static const Value *int2State(const SourceContext &context, const int state)
{
    return new MatrixValue(Matrix::ketBase(state));
}

const ChainUnaryOperator &int2StateOper = *(new UnaryErrorOperator("Expected integer value: "))
                                               ->mapIntValue(int2State);

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
