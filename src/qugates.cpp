#include <iostream>

#include "qugates.h"

using namespace std;
using namespace qb;
using namespace qg;

/**
 * Computes the state permutation
 * <pre>
 *     out[p[i]]=in[i]
 * </pre>
 *
 * @param bitPermutation the bit permutation
 */
static const indices_t computeStatePermutation(const indices_t bitPermutation)
{
    const size_t numBits = bitPermutation.size();
    const size_t numStates = 1 << numBits;
    indices_t result;
    result.assign(numStates, 0);
    for (size_t s = 0; s < numStates; s++)
    {
        size_t s1 = 0;
        size_t mask = 1;
        for (size_t i = 0; i < numBits; i++)
        {
            size_t b = s & mask;
            if (b != 0)
            {
                const size_t b1 = bitPermutation[i];
                if (i > b1)
                {
                    b >>= i - b1;
                }
                else if (b1 > i)
                {
                    b <<= b1 - i;
                }
                s1 |= b;
            }
            mask <<= 1;
        }
        result[s] = s1;
    }
    return result;
}

/**
 * Returns the inverse permutation
 *
 * @param s the permutation
 */
static const indices_t inversePermutation(const indices_t &s)
{
    const size_t n = s.size();
    indices_t reverse;
    reverse.assign(n, 0);
    for (size_t i = 0; i < n; i++)
    {
        reverse[s[i]] = i;
    }
    return reverse;
}

static ostream &operator<<(ostream &os, vector<bool> map)
{
    os << "[";
    bool isFirst = true;
    for (const bool value : map)
    {
        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            os << ", ";
        }
        os << value;
    }
    os << "]";
    return os;
}

/**
 * Returns the bit permutation from input to internal gate input
 *
 * @param numBits number of qubits
 * @param portMap the bit mapping from internal gate input to input
 */
static const indices_t computeMap(const size_t numBits, const indices_t portMap)
{
    indices_t result;
    result.assign(numBits, 0);
    const size_t n = portMap.size();

    vector<bool> gateMapped;
    gateMapped.assign(numBits, false);

    vector<bool> inMapped;
    inMapped.assign(numBits, false);

    // Map gate input
    for (size_t i = 0; i < n; i++)
    {
        const size_t j = portMap[i];
        result[j] = i;
        gateMapped[j] = inMapped[i] = true;
    }

    // Mapped unchanged
    for (size_t i = n; i < numBits; i++)
    {
        if (!gateMapped[i])
        {
            gateMapped[i] = inMapped[i] = true;
            result[i] = i;
        }
    }

    // Map remaining
    size_t free = 0;
    for (size_t i = n; i < numBits; i++)
    {
        if (!inMapped[i])
        {
            while (gateMapped[free])
            {
                free++;
            }
            result[free] = i;
            gateMapped[free] = inMapped[i] = true;
        }
    }
    return result;
}

const QuGate QuGate::h(const size_t data)
{
    return QuGate("h", {data}, Matrix::h);
}

const QuGate QuGate::i(const size_t data)
{
    return QuGate("i", {data}, Matrix::i);
}

const QuGate QuGate::x(const size_t data)
{
    return QuGate("x", {data}, Matrix::x);
}

const QuGate QuGate::y(const size_t data)
{
    return QuGate("y", {data}, Matrix::y);
}

const QuGate QuGate::z(const size_t data)
{
    return QuGate("z", {data}, Matrix::z);
}

const QuGate QuGate::s(const size_t data)
{
    return QuGate("s", {data}, Matrix::s);
}

const QuGate QuGate::t(const size_t data)
{
    return QuGate("t", {data}, Matrix::t);
}

const QuGate QuGate::swap(const size_t a, const size_t b)
{
    return QuGate("swap", {a, b}, Matrix::swap);
}

const QuGate QuGate::cnot(const size_t control, const size_t data)
{
    return QuGate("cnot", {control, data}, Matrix::ccnot);
}

const QuGate QuGate::ccnot(const size_t c0, const size_t c1, const size_t data)
{
    return QuGate("ccnot", {c0, c1, data}, Matrix::ccnot);
}

const qb::Matrix QuGate::build(const size_t numBits) const
{
    const size_t m = maxIndices();
    if (m >= numBits)
    {
        throw invalid_argument(
            (ostringstream() << "number of bits must be freater then " << m << ", got (" << numBits << ")")
                .str());
    }
    // Swap the gate input to align to base transform
    indices_t inBits = computeMap(numBits, _indices);
    const indices_t statesPermutation = computeStatePermutation(inBits);
    const indices_t reverseState = inversePermutation(statesPermutation);
    Matrix inSwapMatrix = Matrix::permute(statesPermutation);
    Matrix outSwapMatrix = Matrix::permute(reverseState);

    // Create the full gate matrix
    const size_t numInputs = _indices.size();
    Matrix gateMatrix = _matrix;
    if (numBits > numInputs)
    {
        Matrix identities = Matrix::identity(1 << (numBits - numInputs));
        gateMatrix = cross(identities, gateMatrix);
    }
    Matrix result = outSwapMatrix * gateMatrix * inSwapMatrix;
    return result;
}

const size_t QuGate::maxIndices(void) const
{
    size_t result = 0;
    for (const size_t s : _indices)
    {
        if (s > result)
        {
            result = s;
        }
    }
    return result;
}

const size_t qg::numQuBits(const std::vector<QuGate> &gates)
{
    size_t n = 0;
    for (auto gate : gates)
    {
        size_t m = gate.maxIndices() + 1;
        if (m >= n)
        {
            n = m;
        }
    }
    return n;
}

/**
 * Returns the matrix of a quantum circuit
 * @param gates the gates
 */
const Matrix qg::buildCircuit(const gates_t &gates)
{
    const size_t n = numQuBits(gates);
    if (n == 0)
    {
        throw invalid_argument(
            (ostringstream() << "creating circuit with no qubits")
                .str());
    }
    Matrix result(Matrix::identity(1 << n));
    for (QuGate gate : gates)
    {
        Matrix a = gate.build(n);
        result = a * result;
    }

    return result;
}

ostream &operator<<(ostream &os, const QuGate &gate)
{
    os << gate.id() << "(";
    bool withComma = false;
    for (const size_t b : gate.indices())
    {
        if (withComma)
        {
            os << ", ";
        }
        else
        {
            withComma = true;
        }
        os << b;
    }
    os << ")";
    return os;
}

ostream &operator<<(ostream &os, const indices_t &indices)
{
    bool isFirst = true;
    os << "(";
    for (const size_t i : indices)
    {
        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            os << ", ";
        }
        os << i;
    }
    os << ")";
    return os;
}
