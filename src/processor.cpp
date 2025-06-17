#include <sstream>

#include "processor.h"
#include "values.h"
#include "commands.h"
#include "matrix.h"
#include "operators.h"

using namespace std;
using namespace qc;
using namespace mx;

Processor::~Processor()
{
    for (const auto &[k, v] : _variables)
    {
        delete v;
    }
}

// -------- eps

static const Value *epsMapper(const SourceContext &context, const int i, const int j)
{
    return new MatrixValue(context, mx::eps(i, j));
}

const ChainBinaryOperator &epsOper = *(new BinaryErrorOperator())
                                          ->mapIntInt(epsMapper);

const static Value *epsFuncMapper(const SourceContext &context, const ListValue &args)
{
    return epsOper.apply(context, *args.values().at(0), *args.values().at(1));
}

// -------- sim

static const Value *simMapper(const SourceContext &context, const int i, const int j)
{
    return new MatrixValue(context, sim(i, j));
}

const ChainBinaryOperator &simOper = *(new BinaryErrorOperator())
                                          ->mapIntInt(simMapper);

const static Value *simFuncMapper(const SourceContext &context, const ListValue &args)
{
    return simOper.apply(context, *args.values().at(0), *args.values().at(1));
}

// -------- ary

static const Value *aryMapper(const SourceContext &context, const int i, const int j)
{
    return new MatrixValue(context, ary(i, j));
}

const ChainBinaryOperator &aryOper = *(new BinaryErrorOperator())
                                          ->mapIntInt(aryMapper);

static const Value *aryFuncMapper(const SourceContext &context, const ListValue &args)
{
    return aryOper.apply(context, *args.values().at(0), *args.values().at(1));
};

// -------- sqrt

static const Value *intSqrt(const SourceContext &context, const int arg)
{
    return new ComplexValue(context, sqrt((complex<double>)arg));
}

static const Value *complexSqrt(const SourceContext &context, const complex<double> &arg)
{
    return new ComplexValue(context, sqrt(arg));
}

const ChainUnaryOperator &sqrtOper = *(new UnaryErrorOperator())
                                          ->mapInt(intSqrt)
                                          ->mapComplex(complexSqrt);

static const Value *sqrtMapper(const SourceContext &context, const ListValue &args)
{
    return sqrtOper.apply(context, *args.values().at(0));
};

// -------- normalise

static const Value *intNormalise(const SourceContext &context, const int arg)
{
    return new IntValue(context, 1);
}
#include <iostream>

static const Value *complexNormalise(const SourceContext &context, const complex<double> &arg)
{
    return new ComplexValue(context, arg / sqrt(norm(arg)));
}

static const Value *matrixNormalise(const SourceContext &context, const Matrix &arg)
{
    return new MatrixValue(context, arg);
}

const ChainUnaryOperator &normOper = *(new UnaryErrorOperator())
                                          ->mapInt(intNormalise)
                                          ->mapComplex(complexNormalise)
                                          ->mapMatrix(matrixNormalise);

static const Value *normMapper(const SourceContext &context, const ListValue &args)
{
    return normOper.apply(context, *args.values().at(0));
};

// -------- I

static const Value *intI(const SourceContext &context, const int arg)
{
    return new MatrixValue(context, I(arg));
}

const ChainUnaryOperator &iOper = *(new UnaryErrorOperator())
                                       ->mapInt(intI);

static const Value *iMapper(const SourceContext &context, const ListValue &args)
{
    return iOper.apply(context, *args.values().at(0));
};

// -------- H

static const Value *intH(const SourceContext &context, const int arg)
{
    return new MatrixValue(context, H(arg));
}

const ChainUnaryOperator &hOper = *(new UnaryErrorOperator())
                                       ->mapInt(intH);

static const Value *hMapper(const SourceContext &context, const ListValue &args)
{
    return hOper.apply(context, *args.values().at(0));
};

// -------- S

static const Value *intS(const SourceContext &context, const int arg)
{
    return new MatrixValue(context, S(arg));
}

const ChainUnaryOperator &sOper = *(new UnaryErrorOperator())
                                       ->mapInt(intS);

static const Value *sMapper(const SourceContext &context, const ListValue &args)
{
    return sOper.apply(context, *args.values().at(0));
};

// -------- T

static const Value *intT(const SourceContext &context, const int arg)
{
    return new MatrixValue(context, T(arg));
}

const ChainUnaryOperator &tOper = *(new UnaryErrorOperator())
                                       ->mapInt(intT);

static const Value *tMapper(const SourceContext &context, const ListValue &args)
{
    return tOper.apply(context, *args.values().at(0));
};

// -------- X

static const Value *intX(const SourceContext &context, const int arg)
{
    return new MatrixValue(context, X(arg));
}

const ChainUnaryOperator &xOper = *(new UnaryErrorOperator())
                                       ->mapInt(intX);

static const Value *xMapper(const SourceContext &context, const ListValue &args)
{
    return xOper.apply(context, *args.values().at(0));
};

// -------- Y

static const Value *intY(const SourceContext &context, const int arg)
{
    return new MatrixValue(context, Y(arg));
}

const ChainUnaryOperator &yOper = *(new UnaryErrorOperator())
                                       ->mapInt(intY);

static const Value *yMapper(const SourceContext &context, const ListValue &args)
{
    return yOper.apply(context, *args.values().at(0));
};

// -------- Z

static const Value *intZ(const SourceContext &context, const int arg)
{
    return new MatrixValue(context, Z(arg));
}

const ChainUnaryOperator &zOper = *(new UnaryErrorOperator())
                                       ->mapInt(intZ);

static const Value *zMapper(const SourceContext &context, const ListValue &args)
{
    return zOper.apply(context, *args.values().at(0));
};

// -------- CNOT

static const Value *intCNOT(const SourceContext &context, const int data, const int control)
{
    try
    {
        return new MatrixValue(context, CNOT(data, control));
    }
    catch (invalid_argument ex)
    {
        throw context.execException(ex.what());
    }
}

const ChainBinaryOperator &cnotOper = *(new BinaryErrorOperator())
                                           ->mapIntInt(intCNOT);

static const Value *cnotMapper(const SourceContext &context, const ListValue &args)
{
    return cnotOper.apply(context, *args.values().at(0), *args.values().at(1));
};

// -------- SWAP

static const Value *intSWAP(const SourceContext &context, const int data0, const int data1)
{
    try
    {
        return new MatrixValue(context, SWAP(data0, data1));
    }
    catch (invalid_argument ex)
    {
        throw context.execException(ex.what());
    }
}

const ChainBinaryOperator &swapOper = *(new BinaryErrorOperator())
                                           ->mapIntInt(intSWAP);

static const Value *swapMapper(const SourceContext &context, const ListValue &args)
{
    return swapOper.apply(context, *args.values().at(0), *args.values().at(1));
};

//-----------------------

static const Value *ccnotMapper(const SourceContext &context, const ListValue &args)
{
    const Value *data = args.values().at(0);
    const Value *ctrl0 = args.values().at(1);
    const Value *ctrl1 = args.values().at(2);

    if (data->type() != ValueType::intValueType || ctrl0->type() != ValueType::intValueType || ctrl1->type() != ValueType::intValueType)
    {
        stringstream str;
        str << "Unexpected arguments " << data->type() << ", " << ctrl0->type() << ", " << ctrl1->type();
        throw context.execException(str.str());
    }
    try
    {
        return new MatrixValue(context, CCNOT(((const IntValue *)data)->value(),
                                              ((const IntValue *)ctrl0)->value(),
                                              ((const IntValue *)ctrl1)->value()));
    }
    catch (invalid_argument ex)
    {
        throw context.execException(ex.what());
    }
}

// -------- qbit0

static const Value *intQubit0(const SourceContext &context, const int index, const int numBits)
{
    try
    {
        return new MatrixValue(context, qubit0(index, numBits));
    }
    catch (invalid_argument ex)
    {
        throw context.execException(ex.what());
    }
}

const ChainBinaryOperator &qubit0Oper = *(new BinaryErrorOperator())
                                             ->mapIntInt(intQubit0);

static const Value *qubit0Mapper(const SourceContext &context, const ListValue &args)
{
    return qubit0Oper.apply(context, *args.values().at(0), *args.values().at(1));
};

// -------- qbit1

static const Value *intQubit1(const SourceContext &context, const int index, const int numBits)
{
    try
    {
        return new MatrixValue(context, qubit1(index, numBits));
    }
    catch (invalid_argument ex)
    {
        throw context.execException(ex.what());
    }
}

const ChainBinaryOperator &qubit1Oper = *(new BinaryErrorOperator())
                                             ->mapIntInt(intQubit1);

static const Value *qubit1Mapper(const SourceContext &context, const ListValue &args)
{
    return qubit1Oper.apply(context, *args.values().at(0), *args.values().at(1));
};

const map<string, FunctionDef> qc::QU_PROCESSOR_FUNCTIONS{
    {"sqrt", FunctionDef("sqrt", 1, sqrtMapper)},
    {"ary", FunctionDef("ary", 2, aryFuncMapper)},
    {"sim", FunctionDef("sim", 2, simFuncMapper)},
    {"eps", FunctionDef("eps", 2, epsFuncMapper)},
    {"H", FunctionDef("H", 1, hMapper)},
    {"I", FunctionDef("I", 1, iMapper)},
    {"S", FunctionDef("S", 1, sMapper)},
    {"T", FunctionDef("T", 1, tMapper)},
    {"X", FunctionDef("X", 1, xMapper)},
    {"Y", FunctionDef("Y", 1, yMapper)},
    {"Z", FunctionDef("Z", 1, zMapper)},
    {"SWAP", FunctionDef("SWAP", 2, swapMapper)},
    {"CNOT", FunctionDef("CNOT", 2, cnotMapper)},
    {"CCNOT", FunctionDef("CCNOT", 3, ccnotMapper)},
    {"qubit0", FunctionDef("qubit0", 2, qubit0Mapper)},
    {"qubit1", FunctionDef("qubit1", 2, qubit1Mapper)},
    {"normalise", FunctionDef("normalise", 1, normMapper)}};

static const Value *intNegate(const SourceContext &context, const int state)
{
    return new IntValue(context, -state);
}

static const Value *complexNegate(const SourceContext &context, const complex<double> &state)
{
    return new ComplexValue(context, -state);
}

static const Value *matrixNegate(const SourceContext &context, const Matrix &state)
{
    return new MatrixValue(context, -state);
}

static const Value *int2State(const SourceContext &context, const int state)
{
    return new MatrixValue(context, ketBase(state));
}

static const Value *intDagger(const SourceContext &context, const int value)
{
    return new IntValue(context, value);
}

static const Value *complexDagger(const SourceContext &context, const complex<double> &value)
{
    return new ComplexValue(context, conj(value));
}

static const Value *matrixDagger(const SourceContext &context, const Matrix &value)
{
    return new MatrixValue(context, value.dagger());
}

const ChainUnaryOperator &int2StateOper = *(new UnaryErrorOperator())
                                               ->mapInt(int2State);

const ChainUnaryOperator &daggerOper = *(new UnaryErrorOperator())
                                            ->mapInt(intDagger)
                                            ->mapComplex(complexDagger)
                                            ->mapMatrix(matrixDagger);

const ChainUnaryOperator &negOper = *(new UnaryErrorOperator())
                                         ->mapInt(intNegate)
                                         ->mapComplex(complexNegate)
                                         ->mapMatrix(matrixNegate);

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
    if (_variables.count(id) > 0)
    {
        delete _variables[id];
    }
    _variables[id] = arg->source(source);
    delete arg;
    return _variables[id]->clone();
}

const Value *Processor::retrieveVar(const SourceContext &source, const string &id)
{
    if (_variables.count(id) == 0)
    {
        throw source.execException("Undefined variable " + id);
    }
    return _variables.at(id)->source(source);
}

const Value *Processor::clear(const SourceContext &source)
{
    for (const auto &[k, v] : _variables)
    {
        delete v;
    }
    _variables.clear();
    return new IntValue(source, 0);
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
    return new MatrixValue(source, left.cross(right));
};

static ChainBinaryOperator &crossOp = *(new BinaryErrorOperator())
                                           ->mapMatrixMatrix(crossMapper);

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

static const Value *mulIntIntMapper(const SourceContext &source, const int left, const int right)
{
    return new IntValue(source, left * right);
};

static const Value *mulIntComplexMapper(const SourceContext &source, const int left, const complex<double> &right)
{
    return new ComplexValue(source, (double)left * right);
};

static const Value *mulComplexIntMapper(const SourceContext &source, const complex<double> &left, const int right)
{
    return new ComplexValue(source, left * (double)right);
};

static const Value *mulComplexComplexMapper(const SourceContext &source, const complex<double> &left, const complex<double> &right)
{
    return new ComplexValue(source, left * right);
};

static const Value *mulMatrixIntMapper(const SourceContext &source, const Matrix &left, const int right)
{
    return new MatrixValue(source, left * (const complex<double>)right);
};

static const Value *mulMatrixComplexMapper(const SourceContext &source, const Matrix &left, const complex<double> &right)
{
    return new MatrixValue(source, left * right);
};

static const Value *mulMatrixMatrixMapper(const SourceContext &source, const Matrix &left, const Matrix &right)
{
    return new MatrixValue(source, left.multiply(right));
};

static ChainBinaryOperator &mulOp = *(new BinaryErrorOperator())
                                         ->mapMatrixMatrix(mulMatrixMatrixMapper)
                                         ->mapMatrixComplex(mulMatrixComplexMapper)
                                         ->mapMatrixInt(mulMatrixIntMapper)
                                         ->mapComplexComplex(mulComplexComplexMapper)
                                         ->mapComplexInt(mulComplexIntMapper)
                                         ->mapIntComplex(mulIntComplexMapper)
                                         ->mapIntInt(mulIntIntMapper);

const Value *Processor::mul(const SourceContext &source, const Value *left, const Value *right)
{
    try
    {
        const Value *result = mulOp.apply(source, *left, *right);
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

static const Value *mulStarMatrixMatrixMapper(const SourceContext &source, const Matrix &left, const Matrix &right)
{
    return new MatrixValue(source, left * right);
};

static ChainBinaryOperator &mulStarOp = *(new BinaryErrorOperator())
                                             ->mapMatrixMatrix(mulStarMatrixMatrixMapper)
                                             ->mapMatrixComplex(mulMatrixComplexMapper)
                                             ->mapMatrixInt(mulMatrixIntMapper)
                                             ->mapComplexComplex(mulComplexComplexMapper)
                                             ->mapComplexInt(mulComplexIntMapper)
                                             ->mapIntComplex(mulIntComplexMapper)
                                             ->mapIntInt(mulIntIntMapper);

const Value *Processor::mulStar(const SourceContext &source, const Value *left, const Value *right)
{
    try
    {
        const Value *result = mulStarOp.apply(source, *left, *right);
        delete left;
        delete right;
        return result;
    }
    catch (invalid_argument ex)
    {
        delete left;
        delete right;
        throw source.execException(ex.what());
    }
    catch (QuExecException ex)
    {
        delete left;
        delete right;
        throw;
    }
}

static const Value *divIntIntMapper(const SourceContext &source, const int left, const int right)
{
    return (left % right) == 0
               ? (Value *)new IntValue(source, left / right)
               : (Value *)new ComplexValue(source, (double)left / right);
};

static const Value *divIntComplexMapper(const SourceContext &source, const int left, const complex<double> &right)
{
    return new ComplexValue(source, (double)left / right);
};

static const Value *divComplexIntMapper(const SourceContext &source, const complex<double> &left, const int right)
{
    return new ComplexValue(source, left / (double)right);
};

static const Value *divComplexComplexMapper(const SourceContext &source, const complex<double> &left, const complex<double> &right)
{
    return new ComplexValue(source, left / right);
};

static const Value *divMatrixIntMapper(const SourceContext &source, const Matrix &left, const int right)
{
    return new MatrixValue(source, left / right);
};

static const Value *divMatrixComplexMapper(const SourceContext &source, const Matrix &left, const complex<double> &right)
{
    return new MatrixValue(source, left / right);
};

static ChainBinaryOperator &divOp = *(new BinaryErrorOperator())
                                         ->mapMatrixComplex(divMatrixComplexMapper)
                                         ->mapMatrixInt(divMatrixIntMapper)
                                         ->mapComplexComplex(divComplexComplexMapper)
                                         ->mapComplexInt(divComplexIntMapper)
                                         ->mapIntComplex(divIntComplexMapper)
                                         ->mapIntInt(divIntIntMapper);

const Value *Processor::div(const SourceContext &source, const Value *left, const Value *right)
{
    try
    {
        const Value *result = divOp.apply(source, *left, *right);
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

static const Value *addIntIntMapper(const SourceContext &source, const int left, const int right)
{
    return new IntValue(source, left + right);
};

static const Value *addIntComplexMapper(const SourceContext &source, const int left, const complex<double> &right)
{
    return new ComplexValue(source, complex<double>(left) + right);
};

static const Value *addComplexIntMapper(const SourceContext &source, const complex<double> &left, const int right)
{
    return new ComplexValue(source, left + complex<double>(right));
};

static const Value *addComplexComplexMapper(const SourceContext &source, const complex<double> &left, const complex<double> &right)
{
    return new ComplexValue(source, left + right);
};

static const Value *addMatrixMatrixMapper(const SourceContext &source, const Matrix &left, const Matrix &right)
{
    return new MatrixValue(source, left + right);
};

static ChainBinaryOperator &addOp = *(new BinaryErrorOperator())
                                         ->mapMatrixMatrix(addMatrixMatrixMapper)
                                         ->mapComplexComplex(addComplexComplexMapper)
                                         ->mapComplexInt(addComplexIntMapper)
                                         ->mapIntComplex(addIntComplexMapper)
                                         ->mapIntInt(addIntIntMapper);

const Value *Processor::add(const SourceContext &source, const Value *left, const Value *right)
{
    try
    {
        const Value *result = addOp.apply(source, *left, *right);
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

static const Value *subIntIntMapper(const SourceContext &source, const int left, const int right)
{
    return new IntValue(source, left - right);
};

static const Value *subIntComplexMapper(const SourceContext &source, const int left, const complex<double> &right)
{
    return new ComplexValue(source, complex<double>(left) - right);
};

static const Value *subComplexIntMapper(const SourceContext &source, const complex<double> &left, const int right)
{
    return new ComplexValue(source, left - complex<double>(right));
};

static const Value *subComplexComplexMapper(const SourceContext &source, const complex<double> &left, const complex<double> &right)
{
    return new ComplexValue(source, left - right);
};

static const Value *subMatrixMatrixMapper(const SourceContext &source, const Matrix &left, const Matrix &right)
{
    return new MatrixValue(source, left - right);
};

static ChainBinaryOperator &subOp = *(new BinaryErrorOperator())
                                         ->mapMatrixMatrix(subMatrixMatrixMapper)
                                         ->mapComplexComplex(subComplexComplexMapper)
                                         ->mapComplexInt(subComplexIntMapper)
                                         ->mapIntComplex(subIntComplexMapper)
                                         ->mapIntInt(subIntIntMapper);

const Value *Processor::sub(const SourceContext &source, const Value *left, const Value *right)
{
    try
    {
        const Value *result = subOp.apply(source, *left, *right);
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
