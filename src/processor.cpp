#include "processor.h"
#include "values.h"
#include "commands.h"
#include "matrix.h"

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

class Int2StateMapper : public ValueMapper
{
    const SourceContext &_ctx;

public:
    Int2StateMapper(const SourceContext &ctx) : _ctx(ctx) {}

    virtual const Value *map(const IntValue &value) const
    {
        return new MatrixValue(Matrix::ketBase(value.value()));
    };
    
    virtual const Value *map(const ComplexValue &value) const
    {
        throw _ctx.execException("Expected integer value: " + to_string(value));
    };
    
    virtual const Value *map(const MatrixValue &value) const
    {
        throw _ctx.execException("Expected integer value: (" + to_string(value) + ")");
    };
    
    virtual const Value *map(const ListValue &value) const
    {
        throw _ctx.execException("Expected integer value: " + to_string(value));
    };
};

const Value *Processor::int2Ket(const SourceContext &source, const Value *arg)
{
    Int2StateMapper mapper(source);
    try
    {
        const Value *result = arg->map(mapper);
        delete arg;
        return result;
    }
    catch (QuExecException ex)
    {
        delete arg;
        throw;
    }
}
