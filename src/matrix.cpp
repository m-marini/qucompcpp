#include <iostream>
#include <cmath>
#include "qubit.h"
#include "vectutils.h"

#define HALF_SQRT2 (sqrt(2) / 2)

using namespace std;
using namespace qb;
using namespace vu;

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

void Matrix::validateIndices(const size_t i, const size_t j) const
{
    if (i < 0 || i >= _numRows || j < 0 || j >= _numCols)
    {
        throw invalid_argument(
            (ostringstream() << "Cells access with index out of range 0..." << _numRows - 1 << ", 0..." << _numCols - 1 << ", got (" << i << ", " << j << ")")
                .str());
    }
}

const complex<float> &Matrix::at(const size_t i, const size_t j) const
{
    validateIndices(i, j);
    return unsafeAt(i, j);
}

Matrix &Matrix::operator=(const Matrix &a)
{
    _numRows = a._numRows;
    _numCols = a._numCols;
    _cells = a._cells;
    return *this;
}

const Matrix Matrix::identity(const size_t n)
{
    ComplexVect cells;
    cells.assign(n * n, 0);
    for (size_t i = 0; i < n; i++)
    {
        cells[Matrix::indexOf(n, i, i)] = 1;
    }
    return Matrix(n, n, cells);
}

const Matrix Matrix::permute(const vector<size_t> &permutation)
{
    const size_t n = permutation.size();
    ComplexVect cells;
    cells.reserve(n * n);
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            cells.insert(cells.end(), i == permutation.at(j)
                                          ? complex<float>(1)
                                          : complex<float>(0));
        }
    }
    return Matrix(n, n, cells);
}

const Ket operator*(const Matrix &a, const Ket &b)
{
    const size_t n = b.size();
    if (a.numCols() != n)
    {
        throw invalid_argument(
            (ostringstream() << "Expected ket transform with matrix shape ?x" << n << ", got (" << a.numRows() << "x" << a.numCols()
                             << ")")
                .str());
    }
    const size_t m = a.numRows();
    ComplexVect cells;
    cells.assign(m, 0);
    partMul(cells, 0, m, 1, a.cells(), 0, n, b.states(), 0, 1);
    return Ket(cells);
}

const Bra operator*(const Bra &a, const Matrix &b)
{
    const size_t n = a.size();
    if (b.numRows() != n)
    {
        throw invalid_argument(
            (ostringstream() << "Expected bra transform with matrix shape " << n << "x?, got (" << b.numRows() << "x" << b.numCols()
                             << ")")
                .str());
    }
    const size_t m = b.numCols();
    ComplexVect cells;
    cells.assign(m, 0);
    partMul(cells, 0, 1, m, a.states(), 0, n, b.cells(), 0, m);
    return Bra(cells);
}

const Matrix operator*(const Matrix &a, const Matrix &b)
{
    if (a.numCols() != b.numRows())
    {
        throw invalid_argument(
            (ostringstream() << "Invalid matrix multiplication " << a.numRows() << "x" << a.numCols() << " by " << b.numRows() << "x" << b.numCols())
                .str());
    }
    const size_t n = a.numRows() * b.numCols();
    ComplexVect cells;
    cells.assign(n, 0);
    partMul(cells, 0, a.numRows(), b.numCols(), a.cells(), 0, a.numCols(), b.cells(), 0, b.numCols());
    return Matrix(a.numRows(), b.numCols(), cells);
}

const Matrix qb::cross(const Matrix &a, const Matrix &b)
{
    const size_t n = a.numRows();
    const size_t m = b.numRows();
    if (n != a.numCols() || m != b.numCols())
    {
        throw invalid_argument(
            (ostringstream() << "Expected cross product of square matrices, got (" << n << "x" << a.numCols() << ") x (" << m << "x" << b.numCols() << ")")
                .str());
    }
    const size_t nm = n * m;
    ComplexVect cells;
    cells.reserve(nm * nm);
    cells.assign(nm * nm, 0);
    for (size_t i = 0; i < n; i++)
    {
        for (size_t k = 0; k < n; k++)
        {
            for (size_t j = 0; j < m; j++)
            {
                for (size_t l = 0; l < m; l++)
                {
                    const size_t r = i * m + j;
                    const size_t q = k * m + l;
                    const size_t idx = Matrix::indexOf(nm, r, q);
                    cells[idx] = a.at(i, k) * b.at(j, l);
                }
            }
        }
    }
    return Matrix(nm, nm, cells);
}

const Matrix Matrix::i(2, 2, ComplexVect({1, 0, 0, 1}));
const Matrix Matrix::x(permute({1, 0}));
const Matrix Matrix::y(2, 2, {0, -1if, 1if, 0});
const Matrix Matrix::z(2, 2, {1, 0, 0, -1});
const Matrix Matrix::swap(permute({0, 2, 1, 3}));
const Matrix Matrix::cnot(permute({0, 1, 3, 2}));
const Matrix Matrix::ccnot(permute({0, 1, 2, 3, 4, 5, 7, 6}));
const Matrix Matrix::h(2, 2, {HALF_SQRT2, HALF_SQRT2, HALF_SQRT2, -HALF_SQRT2});
const Matrix Matrix::s(2, 2, {1, 0, 0, -1if});
const Matrix Matrix::t(2, 2, {1, 0, 0, complex<float>(HALF_SQRT2, HALF_SQRT2)});

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
