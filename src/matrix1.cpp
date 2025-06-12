#include <iostream>
#include <cmath>

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
    return Matrix(_numRows,_numCols, vu::conj(_cells));
}

const Matrix Matrix::operator-(void) const
{
    return Matrix(_numRows,_numCols, -_cells);
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
