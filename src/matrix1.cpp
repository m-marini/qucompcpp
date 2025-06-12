#include <iostream>
#include <cmath>

#include "matrix.h"
#include "vectutils.h"

#define HALF_SQRT2 (sqrt(2) / 2)

using namespace std;
using namespace mx;
using namespace vu;

typedef std::vector<size_t> indices_t;

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

const Matrix Matrix::ketBase(const int value)
{
    const size_t n = 1 << numBitsByState(value);
    vector<complex<double>> cells;
    cells.assign(n, 0);
    cells[value] = 1;
    return Matrix(n, 1, cells);
}

const Matrix Matrix::I(const size_t bit)
{
    const size_t n = 2 << bit;
    return identity(n);
}

const Matrix Matrix::identity(const size_t size)
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
                string msg = "Expected all different indices (";
                bool first = true;
                for (const size_t idx : bitMap)
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        msg += ", ";
                    }
                    msg += to_string(idx);
                }
                throw new invalid_argument(msg + ")");
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
static const indices_t computeBitsPermutation(const indices_t &bitMap)
{
    validateBitMap(bitMap);
    size_t numBits = 0;
    for (const size_t idx : bitMap)
    {
        if (idx > numBits)
        {
            numBits = idx;
        }
    }
    const int m = bitMap.size();
    numBits = max(bitMap.size(), numBits + 1);

    indices_t result;
    result.resize(numBits, 0);
    bool gateMapped[numBits];
    bool inMapped[numBits];
    // Map gate input
    for (int i = 0; i < m; i++)
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
static const indices_t computeStatePermutation(const indices_t &bitPermutation)
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
static const indices_t inversePermutation(indices_t s)
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
                        cells.push_back(at(i, k)*right.at(j, l));
                }
            }
        }
    }
    return Matrix(rows, cols, cells);
}

const string to_string(const Matrix &a)
{
    stringstream stream;
    stream << a;
    return stream.str();
}