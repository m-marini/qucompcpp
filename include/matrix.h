#ifndef _matrix_h_
#define _matrix_h_

#include <string>
#include <vector>

#include "vectutils.h"

namespace mx
{
    class Matrix
    {
        size_t _numRows;
        size_t _numCols;
        vu::ComplexVect _cells;

        void validateIndices(const size_t i, const size_t j) const;
        const std::complex<double> &unsafeAt(const size_t i, const size_t j) const { return (_cells)[unsafeIndexOf(i, j)]; }
        const size_t unsafeIndexOf(const size_t i, const size_t j) const { return indexOf(_numCols, i, j); }

    public:
        /**
         * Creates the matrix
         * @param numRows the number of rows
         * @param numCols the number of colums
         * @param cells the cells
         */
        Matrix(const size_t numRows, const size_t numCols, const vu::ComplexVect &cells);

        /**
         * Returns the index of element
         *
         * @param stride  the stride (number of columns)
         * @param i the row index
         * @param j the columns index
         */
        static const size_t indexOf(const size_t stride, const size_t i, const size_t j) { return stride * i + j; }

        /**
         * Returns the number of rows
         */
        const size_t numRows() const { return _numRows; }

        /**
         * Returns the number of columns
         */
        const size_t numCols() const { return _numCols; }

        /**
         * Returns the cells
         */
        const vu::ComplexVect &cells() const { return _cells; }

        /**
         * Returns the cell value
         */
        const std::complex<double> &at(const size_t i, const size_t j) const;

        /**
         * Assign value of onother matrix
         */
        Matrix &operator=(const Matrix &a);

        static const Matrix ketBase(const int state);
    };

    extern const Matrix PLUS_KET;
    extern const Matrix MINUS_KET;
    extern const Matrix I_KET;
    extern const Matrix MINUS_I_KET;
}

extern std::ostream &operator<<(std::ostream &stream, const mx::Matrix &a);

#endif