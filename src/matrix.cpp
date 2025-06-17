#include <iostream>
#include <sstream>
#include <cmath>
#include <array>
#include <format>

#include "matrix.h"
#include "vectutils.h"

#define HALF_SQRT2 (sqrt(2) / 2)

using namespace std;
using namespace mx;
using namespace vu;

const Matrix mx::PLUS_KET(2, 1, {complex<double>(HALF_SQRT2), complex<double>(HALF_SQRT2)});
const Matrix mx::MINUS_KET(2, 1, {HALF_SQRT2, -HALF_SQRT2});
const Matrix mx::I_KET(2, 1, {HALF_SQRT2, complex<double>(0, HALF_SQRT2)});
const Matrix mx::MINUS_I_KET(2, 1, {HALF_SQRT2, complex<double>(0, -HALF_SQRT2)});

Matrix::Matrix(const size_t numRows, const size_t numCols, const vu::ComplexVect &cells)
    : _numRows(numRows), _numCols(numCols), _cells(cells)
{
    const size_t n = cells.size();
    if (n != numCols * numRows)
    {
        throw invalid_argument(
            (ostringstream() << "Expected " << numRows << "x" << numCols << "=" << (numRows * numCols)
                             << " matrix cells, got (" << n << ")")
                .str());
    }
}

const complex<double> &Matrix::at(const size_t i, const size_t j) const
{
    validateIndices(i, j);
    return unsafeAt(i, j);
}

void Matrix::validateIndices(const size_t i, const size_t j) const
{
    if (i < 0 || i >= _numRows || j < 0 || j >= _numCols)
    {
        throw invalid_argument(
            (ostringstream() << "Cells access with index out of range 0..." << _numRows - 1 << ", 0..." << _numCols - 1 << ", got (" << i << ", " << j << ")")
                .str());
    }
}

const Matrix Matrix::transpose(void) const
{
    return Matrix(_numCols, _numRows, _cells);
}

const Matrix Matrix::conj(void) const
{
    return Matrix(_numRows, _numCols, vu::conj(_cells));
}

const Matrix Matrix::operator-(void) const
{
    return Matrix(_numRows, _numCols, -_cells);
}

const Matrix Matrix::operator*(const complex<double> &right) const
{
    return Matrix(_numRows, _numCols, right * _cells);
}

const Matrix Matrix::operator/(const complex<double> &right) const
{
    return Matrix(_numRows, _numCols, _cells / right);
}

const Matrix Matrix::operator+(const Matrix &right) const
{
    const size_t n = max(_numRows, right._numRows);
    const size_t m = max(_numCols, right._numCols);
    Matrix l = extends0(n, m);
    Matrix r = right.extends0(n, m);
    ComplexVect cells(l._cells + r._cells);
    return Matrix(n, m, cells);
}

const Matrix Matrix::operator-(const Matrix &right) const
{
    const size_t n = max(_numRows, right._numRows);
    const size_t m = max(_numCols, right._numCols);
    Matrix l = extends0(n, m);
    Matrix r = right.extends0(n, m);
    ComplexVect cells(l._cells - r._cells);
    return Matrix(n, m, cells);
}

const Matrix Matrix::extendsRows(int numRows) const
{
    if (_numRows >= numRows)
    {
        return *this;
    }
    ComplexVect cells = _cells;
    cells.insert(cells.end(), (numRows - _numRows) * _numCols, 0);
    return Matrix(numRows, _numCols, cells);
}

const Matrix Matrix::extendsCols(int numCols) const
{
    if (_numCols >= numCols)
    {
        return *this;
    }
    ComplexVect cells = _cells;
    auto pos = cells.begin();
    for (int i = 0; i < _numRows; i++)
    {
        auto pos = cells.begin() + (i * numCols) + _numCols;
        cells.insert(pos, numCols - _numCols, 0);
    }
    return Matrix(_numRows, numCols, cells);
}

static const Matrix baseMultiply(const Matrix &left, const Matrix &right)
{
    if (left.numCols() != right.numRows())
    {
        throw invalid_argument(
            (ostringstream() << "Invalid matrix multiplication " << left.numRows() << "x" << left.numCols() << " by " << right.numRows() << "x" << right.numCols())
                .str());
    }
    const size_t n = left.numRows() * right.numCols();
    ComplexVect cells;
    cells.assign(n, 0);
    partMul(cells, 0, left.numRows(), right.numCols(), left.cells(), 0, left.numCols(), right.cells(), 0, right.numCols());
    return Matrix(left.numRows(), right.numCols(), cells);
}

const Matrix Matrix::extendsCross(const int size) const
{
    if (_numCols == 1)
    {
        // Extend ket
        return extendsRows(size);
    }
    else if (_numRows == 1)
    {
        // Extend bra
        return extendsCols(size);
    }
    else if (_numCols != _numRows)
    {
        throw invalid_argument("Expected square matrix (" + std::to_string(_numRows) + "x" + std::to_string(_numCols) + ")");
    }
    if (_numRows == size)
    {
        return *this;
    }
    if ((size % _numRows) > 0)
    {
        throw invalid_argument((stringstream() << "Expected size multiple of " << _numRows << "x" << _numCols << " (" << size << "x" << size << ")").str());
    }
    const size_t q = size / _numRows;
    return identity(q).cross(*this);
}

const Matrix Matrix::multiply(const Matrix &right) const
{
    if (_numCols < right._numRows)
    {
        return baseMultiply(extendsCols(right._numRows), right);
    }
    else if (_numCols > right._numRows)
    {
        return baseMultiply(*this, right.extendsRows(_numCols));
    }
    else
    {
        return baseMultiply(*this, right);
    }
}

const Matrix Matrix::operator*(const Matrix &right) const
{
    if (_numCols < right._numRows)
    {
        return baseMultiply(extendsCross(right._numRows), right);
    }
    else if (_numCols > right._numRows)
    {
        return baseMultiply(*this, right.extendsCross(_numCols));
    }
    else
    {
        return baseMultiply(*this, right);
    }
}

const Matrix Matrix::cross(const Matrix &right) const
{
    int rows = _numRows * right._numRows;
    int cols = _numCols * right._numCols;
    ComplexVect cells;
    for (size_t i = 0; i < _numRows; i++)
    {
        for (size_t j = 0; j < right._numRows; j++)
        {
            for (size_t k = 0; k < _numCols; k++)
            {
                for (size_t l = 0; l < right._numCols; l++)
                {
                    complex<double> lik = at(i, k);
                    complex<double> rjl = right.at(j, l);
                    complex<double> mikjl = lik * rjl;
                    cells.push_back(mikjl);
                }
            }
        }
    }
    return Matrix(rows, cols, cells);
}

/**
 * Checks for the valid bit map with different values each other
 *
 * @param bitMap the bit map
 */
static void validateBitMap(const indices_t &bitMap)
{
    for (size_t i = 0; i < bitMap.size(); i++)
    {
        for (size_t j = i + 1; j < bitMap.size(); j++)
        {
            if (bitMap[i] == bitMap[j])
            {
                throw invalid_argument("Expected all different indices " + to_string(bitMap));
            }
        }
    }
}

/**
 * Returns the bit permutation from input to internal gate input.
 * <p>
 * The input bits map is the values of internal gate bits for each input gate input (internal[i]<-bitMap[i])
 * E.g.
 * <pre>
 *     [2, 0] = internals[0] <- input[2],
 *              internals[1] <- input[0]
 * </pre>
 * </p>
 * <p>
 * The results is the map of the values of input gate bits for each internal gate input (internal[i]->bitMap[i])
 * E.g. the result of [2, 1] is
 * <pre>
 *     [2, 1, 0] = internals[0] -> input[2],
 *                 internals[1] -> input[1],
 *                 internals[2] -> input[0]
 * </pre>
 * </p>
 *
 * @param bitMap the bits map
 */
const indices_t mx::computeBitsPermutation(const indices_t &bitMap)
{
    validateBitMap(bitMap);
    size_t _numBits = 0;
    for (const size_t i : bitMap)
    {
        if (i > _numBits)
        {
            _numBits = i;
        }
    }
    const size_t m = bitMap.size();
    const size_t numBits = max(m, _numBits + 1);

    indices_t result(numBits, 0);

    vector<bool> gateMapped(numBits, false);
    vector<bool> inMapped(numBits, false);

    // Map gate input
    for (size_t i = 0; i < m; i++)
    {
        result[bitMap[i]] = i;
        gateMapped[bitMap[i]] = inMapped[i] = true;
    }
    // Mapped unchanged
    for (int i = m; i < numBits; i++)
    {
        if (!gateMapped[i])
        {
            gateMapped[i] = inMapped[i] = true;
            result[i] = i;
        }
    }
    // Map remaining
    int free = 0;
    for (int i = m; i < numBits; i++)
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

/**
 * Returns the state permutation given the input bit permutation
 * <pre>
 *     out[p[i]]=in[i]
 * </pre>
 *
 * @param bitPermutation the bit permutations in[i] = the bit index of the resulting bit for the i-th input bit
 */
const indices_t mx::computeStatePermutation(const indices_t &bitPermutation)
{
    const size_t n = 1 << bitPermutation.size();
    indices_t result;
    for (size_t s = 0; s < n; s++)
    {
        int s1 = 0;
        int mask = 1;
        for (int i = 0; i < n; i++)
        {
            int b = s & mask;
            if (b != 0)
            {
                int sh = bitPermutation[i] - i;
                if (sh < 0)
                {
                    b >>= -sh;
                }
                else if (sh > 0)
                {
                    b <<= sh;
                }
                s1 |= b;
            }
            mask <<= 1;
        }
        result.push_back(s1);
    }
    return result;
}

/**
 * Returns the inverse permutation
 *
 * @param s the permutation
 */
const indices_t mx::inversePermutation(const indices_t s)
{
    indices_t reverse;
    const size_t n = s.size();
    reverse.resize(n, 0);
    for (int i = 0; i < n; i++)
    {
        reverse[s[i]] = i;
    }
    return reverse;
}

const Matrix mx::permute(const indices_t &permutation)
{
    const size_t n = permutation.size();
    ComplexVect cells;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            cells.push_back(i == permutation[j]
                                ? 1
                                : 0);
        }
    }
    return Matrix(n, n, cells);
}

/**
 * Returns the matrix of cnot gate applied to the given bits
 *
 * @param baseGate the base gate matrix
 * @param bitMap   the bit map
 */
const Matrix mx::createGate(Matrix baseGate, const indices_t bitMap)
{
    const indices_t statePermuteIn = computeStatePermutation(computeBitsPermutation(bitMap));
    const indices_t statePermuteOut = inversePermutation(statePermuteIn);
    return permute(statePermuteOut) * baseGate * permute(statePermuteIn);
}

const Matrix mx::ary(const int ii, const int jj)
{
    const int n = 1 << numBitsByState(ii);
    const int m = 1 << numBitsByState(jj);
    ComplexVect cells;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            cells.push_back(i == ii && j == jj ? 1 : 0);
        }
    }
    return Matrix(n, m, cells);
}

const Matrix mx::sim(const int ii, const int jj)
{
    const size_t n = 1 << numBitsByState(max(ii, jj));
    ComplexVect cells;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            cells.push_back(
                i == ii && j == jj || i == jj && j == ii
                    ? 1
                    : 0);
        }
    }
    return Matrix(n, n, cells);
}

const Matrix mx::eps(const int ii, const int jj)
{
    const size_t n = 1 << numBitsByState(max(ii, jj));
    const int iii = ii > jj ? jj : ii;
    const int jjj = ii > jj ? ii : jj;
    ComplexVect cells;
    complex<double> ij = iii == jjj
                             ? 0
                         : (iii + jjj) % 2 == 0
                             ? 1
                             : -1;
    complex<double> ji = iii == jjj
                             ? 0
                         : (iii + jjj) % 2 == 0
                             ? -1
                             : 1;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            cells.push_back(
                i == iii && j == jjj
                    ? ij
                : i == jjj && j == iii
                    ? ji
                    : 0);
        }
    }
    return Matrix(n, n, cells);
}

const Matrix mx::ketBase(const int value)
{
    const size_t n = 1 << numBitsByState(value);
    vector<complex<double>> cells;
    cells.assign(n, 0);
    cells[value] = 1;
    return Matrix(n, 1, cells);
}

const Matrix mx::identity(const size_t size)
{
    ComplexVect cells;
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            cells.push_back(i == j ? 1 : 0);
        }
    }
    return Matrix(size, size, cells);
}

const Matrix mx::I(const size_t bit)
{
    const size_t n = 2 << bit;
    return mx::identity(n);
}

static const Matrix X_GATE(2, 2, {0, 1, 1, 0});

const Matrix mx::X(const size_t bit)
{
    return createGate(X_GATE, {bit});
}

static const Matrix Y_GATE(2, 2,
                           {0, complex<double>{0, -1},
                            1i, 0});

const Matrix mx::Y(const size_t bit)
{
    return createGate(Y_GATE, {bit});
}

static const Matrix Z_GATE(2, 2,
                           {1, 0,
                            0, -1});

const Matrix mx::Z(const size_t bit)
{
    return createGate(Z_GATE, {bit});
}

static const Matrix H_GATE(2, 2,
                           {HALF_SQRT2, HALF_SQRT2,
                            HALF_SQRT2, -HALF_SQRT2});

const Matrix mx::H(const size_t bit)
{
    return createGate(H_GATE, {bit});
}

static const Matrix S_GATE(2, 2,
                           {1, 0,
                            0, 1i});

const Matrix mx::S(const size_t bit)
{
    return createGate(S_GATE, {bit});
}

static const Matrix T_GATE(2, 2,
                           {1, 0, 0, {HALF_SQRT2, HALF_SQRT2}});

const Matrix mx::T(const size_t bit)
{
    return createGate(T_GATE, {bit});
}

static const Matrix CNOT_GATE(4, 4,
                              {1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 0, 1,
                               0, 0, 1, 0});

const Matrix mx::CNOT(const size_t data, const size_t control)
{
    return createGate(CNOT_GATE, {data, control});
}

static const Matrix SWAP_GATE(4, 4,
                              {1, 0, 0, 0,
                               0, 0, 1, 0,
                               0, 1, 0, 0,
                               0, 0, 0, 1});

const Matrix mx::SWAP(const size_t data0, const size_t data1)
{
    return createGate(SWAP_GATE, {data0, data1});
}

const Matrix mx::qubit0(const size_t index, const size_t numQubits)
{
    const size_t nBits = max(index + 1, numQubits);
    const size_t nStates = 1 << nBits;
    const size_t mask = 1 << index;
    ComplexVect cells(nStates * nStates, 0);
    for (size_t i = 0; i < nStates; i++)
    {
        if ((i & mask) == 0)
        {
            cells[i * (nStates + 1)] = 1;
        }
    }
    return Matrix(nStates, nStates, cells);
}

const Matrix mx::qubit1(const size_t index, const size_t numQubits)
{
    const size_t nBits = max(index + 1, numQubits);
    const size_t nStates = 1 << nBits;
    const size_t mask = 1 << index;
    ComplexVect cells(nStates * nStates, 0);
    for (size_t i = 0; i < nStates; i++)
    {
        if ((i & mask) != 0)
        {
            cells[i * (nStates + 1)] = 1;
        }
    }
    return Matrix(nStates, nStates, cells);
}

static const Matrix CCNOT_GATE(permute({0, 1, 2, 3, 4, 5, 7, 6}));

const Matrix mx::CCNOT(const size_t data, const size_t control0, const size_t control1)
{
    return createGate(CCNOT_GATE, {data, control0, control1});
}

static ostream &writeBra(ostream &out, const Matrix &a)
{
    bool isZero = true;
    ComplexVect cells = a.cells();
    for (size_t i = 0; i < cells.size(); i++)
    {
        const complex<double> &cell = cells[i];
        if (norm(cell) != 0)
        {
            if (!isZero)
            {
                out << " + ";
            }
            isZero = false;
            if (cell.imag() == 0 && cell.real() == 1)
            {
                out << "<" << i << "|";
            }
            else
            {
                out << "(" << fmt(cell) << ") <" << i << "|";
            }
        }
    }
    return isZero
               ? out << "(0.0) <" << cells.size() - 1 << "|"
               : out;
}

static ostream &writeKet(ostream &out, const Matrix &a)
{
    bool isZero = true;
    ComplexVect cells = a.cells();
    for (size_t i = 0; i < cells.size(); i++)
    {
        const complex<double> &cell = cells[i];
        if (norm(cell) != 0)
        {
            if (!isZero)
            {
                out << " + ";
            }
            isZero = false;
            if (cell.imag() == 0 && cell.real() == 1)
            {
                out << "|" << i << ">";
            }
            else
            {
                out << "(" << fmt(cell) << ") |" << i << ">";
            }
        }
    }
    return isZero
               ? out << "(0.0) |" << cells.size() - 1 << ">"
               : out;
}

static ostream &writeMat(ostream &out, const Matrix &a)
{
    vector<string> cols;
    for (const auto &cell : a.cells())
    {
        cols.push_back(fmt(cell));
    }

    const size_t n = a.numRows();
    const size_t m = a.numCols();
    size_t colSize[m];
    for (size_t j = 0; j < m; j++)
    {
        size_t max = 0;
        for (size_t i = 0; i < n; i++)
        {
            const size_t idx = m * i + j;
            const size_t len = cols[idx].size();
            if (len > max)
            {
                max = len;
            }
        }
        colSize[j] = max;
    }
    out << "[";
    for (size_t i = 0; i < n; i++)
    {
        if (i != 0)
        {
            out << endl
                << " ";
        }
        for (size_t j = 0; j < m; j++)
        {
            out << (j == 0 ? " " : ", ");
            string cell = cols[m * i + j];
            if (cell.size() < colSize[j])
            {
                cell.insert(0, colSize[j] - cell.size(), ' ');
            }
            out << cell;
        }
    }
    return out << " ]";
}

ostream &operator<<(ostream &out, const Matrix &a)
{
    if (a.numCols() == 1)
    {
        if (a.numRows() == 1)
        {
            // Scalar value
            return out << fmt(a.cells()[0]);
        }
        else
        {
            return writeKet(out, a);
        }
    }
    else if (a.numRows() == 1)
    {
        return writeBra(out, a);
    }
    else
    {
        return writeMat(out, a);
    }
}
/*
ostream &operator<<(ostream &os, const Matrix &a)
{
    bool isFirstRow = true;
    for (size_t i = 0; i < a.numRows(); i++)
    {
        if (isFirstRow)
        {
            isFirstRow = false;
        }
        else
        {
            os << endl;
        }
        bool isFirstCol = true;
        for (size_t j = 0; j < a.numCols(); j++)
        {
            if (isFirstCol)
            {
                isFirstCol = false;
            }
            else
            {
                os << ", ";
            }
            os << a.at(i, j);
        }
    }
    return os;
}
*/
ostream &operator<<(ostream &out, const vector<bool> &values)
{
    out << "[";
    bool first = true;
    for (const bool i : values)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            out << ", ";
        }
        out << i;
    }
    return out << "]";
}

ostream &operator<<(ostream &out, const indices_t &indices)
{
    out << "[";
    bool first = true;
    for (const size_t i : indices)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            out << ", ";
        }
        out << i;
    }
    return out << "]";
}

const string to_string(const Matrix &a)
{
    stringstream stream;
    stream << a;
    return stream.str();
}

const string to_string(const indices_t &a)
{
    stringstream stream;
    stream << a;
    return stream.str();
}

const string mx::fmt(const complex<double> &value)
{
    const string real = std::format("{}", value.real());
    const string im = std::format("{}", value.imag());
    if (value.imag() == 0)
    {
        return real;
    }
    if (value.real() == 0)
    {
        if (value.imag() == 1)
        {
            return "i";
        }
        if (value.imag() == -1)
        {
            return "-i";
        }
        return im + " i";
    }
    if (value.imag() == 1)
    {
        return real + " +i";
    }
    if (value.imag() == -1)
    {
        return real + " -i";
    }
    return value.imag() > 0
               ? real + " +" + im + " i"
               : real + " " + im + " i";
}
