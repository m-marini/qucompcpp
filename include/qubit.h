#ifndef _qubit_h_
#define _qubit_h_

#include <vector>
#include <complex>

#include "vectutils.h"

namespace qb
{

    /**
     * Defines a matrix in complex field
     */
    class Matrix
    {
    public:
        /**
         * Returns the index of element
         *
         * @param stride  the stride (number of columns)
         * @param i the row index
         * @param j the columns index
         */
        static const size_t indexOf(const size_t stride, const size_t i, const size_t j) { return stride * i + j; }

        /**
         * Identiy matrix for single qubit
         */
        static const Matrix i;
        /**
         * H matrix (Hadamard)
         */
        static const Matrix h;
        /**
         * X matrix
         */
        static const Matrix x;
        /**
         * Y matrix
         */
        static const Matrix y;
        /**
         * Z matrix
         */
        static const Matrix z;
        /**
         * S matrix
         */
        static const Matrix s;
        /**
         * T matrix
         */
        static const Matrix t;
        /**
         * swap operator
         *
         * ```
         *      SWAP |0> = |0>
         *      SWAP |1> = |2>
         *      SWAP |2> = |1>
         *      SWAP |3> = |3>
         * ```
         */
        static const Matrix swap;

        /**
         * cnot operator (control, data)
         *
         * ```
         *      CNOT |0> = |0>
         *      CNOT |1> = |1>
         *      CNOT |2> = |3>
         *      CNOT |3> = |2>
         * ```
         */
        static const Matrix cnot;

        /**
         * Toffoli operator (control, control, data)
         *
         * ```
         *      CCNOT |0> = |0>
         *      CCNOT |1> = |1>
         *      CCNOT |2> = |2>
         *      CCNOT |3> = |3>
         *      CCNOT |4> = |4>
         *      CCNOT |5> = |5>
         *      CCNOT |6> = |7>
         *      CCNOT |7> = |6>
         * ```
         */
        static const Matrix ccnot;

        /**
         * Returns the matrix that permutes the values of column vector.
         *
         * The value of the i-th element of the permutation array is the target position
         * of source element at i-th position
         *
         * ```
         * B = M x A
         * b(p[i]) := a[i]
         * ```
         * E.g.
         * ```
         *     p = [ 2 3 4 0 1 ]
         *
         *     b(2) = a(0)
         *     b(3) = a(1)
         *     b(4) = a(2)
         *     b(0) = a(3)
         *     b(1) = a(4)
         *
         *     | 0 0 0 1 0 |
         *     | 0 0 0 0 1 |
         * M = | 1 0 0 0 0 | m[p[j],j] = 1
         *     | 0 1 0 0 0 |
         *     | 0 0 1 0 0 |
         *
         *     a=(2 3 4 0 1) => b=(0 1 2 3 4)
         *     a=(0 1 2 3 4) => b=(3 4 0 1 2)
         * ```
         *
         * @param permutation the target mapping
         */
        static const Matrix permute(const std::vector<size_t> &permutation);

        /**
         * Returns the identity matrix of n states
         */
        static const Matrix identity(const size_t n);

        /**
         * Creates the matrix
         * @param numRows the number of rows
         * @param numCols the number of colums
         * @param cells the cells
         */
        Matrix(const size_t numRows, const size_t numCols, const vu::ComplexVect &cells);

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
        const std::complex<float> &at(const size_t i, const size_t j) const;

        /**
         * Assign value of onother matrix
         */
        Matrix &operator=(const Matrix &a);

    private:
        size_t _numRows;
        size_t _numCols;
        vu::ComplexVect _cells;

        void validateIndices(const size_t i, const size_t j) const;
        const std::complex<float> &unsafeAt(const size_t i, const size_t j) const { return (_cells)[unsafeIndexOf(i, j)]; }
        const size_t unsafeIndexOf(const size_t i, const size_t j) const { return indexOf(_numCols, i, j); }
    };

    class Bra;

    /**
     * Defines the Ket
     */
    class Ket
    {
    public:
        /**
         * ket |0>
         */
        static const Ket zero;
        /**
         * ket |1>
         */
        static const Ket one;
        /**
         * ket |+>
         */
        static const Ket plus;
        /**
         * ket |->
         */
        static const Ket minus;
        /**
         * ket |i>
         */
        static const Ket i;
        /**
         * ket |-i>
         */
        static const Ket minus_i;

        /**
         * Returns the ket |n>
         * @param n the state index
         * @param size the number of states
         */
        static const Ket base(const size_t n, const size_t size);

        /**
         * Construct the ket
         * @param states the states
         */
        Ket(const vu::ComplexVect &states) : _states(states) {}

        Ket(const Ket &ket) : _states(ket._states) {}

        /**
         * Returns the conjugate
         */
        const Bra conj(void) const;

        /**
         * Returns the number of states
         */
        const size_t size(void) const { return _states.size(); }

        /**
         * Returns the states
         */
        const vu::ComplexVect &states(void) const { return _states; }

        /**
         * Returns the value of a state
         * @param i the state index
         */
        const std::complex<float> &state(const size_t i) const { return _states.at(i); }

        /**
         * Returns the number of bits
         */
        const size_t numBits(void) const;

        /**
         * Returns the square of module
         */
        const float norm(void) const;

        /**
         * Returns the states amplitude of a bits
         */
        const float bitProb(const size_t i) const;

    private:
        const vu::ComplexVect _states;
    };

    /**
     * Defines the Bra
     */
    class Bra
    {
    public:
        /**
         * bra <0|
         */
        static const Bra zero;
        /**
         * bra <1|
         */
        static const Bra one;
        /**
         * bra <+|
         */
        static const Bra plus;
        /**
         * bra <-|
         */
        static const Bra minus;
        /**
         * bra <i|
         */
        static const Bra i;
        /**
         * bra <-i|
         */
        static const Bra minus_i;

        /**
         * Returns the bra <n|
         * @param n the state index
         * @param size the number of states
         */
        static const Bra base(const size_t i, const size_t size);

        /**
         * Constructor
         */
        Bra(const vu::ComplexVect &states) : _states(states) {}
        Bra(const Bra &bra) : _states(bra._states) {}

        /**
         * Returns the square of module
         */
        const float norm(void) const;

        /**
         * Returns the conjugate
         */
        const Ket conj(void) const;

        /**
         * Returns the number of states
         */
        const size_t size(void) const { return _states.size(); }

        /**
         * Returns the states
         */
        const vu::ComplexVect &states(void) const { return _states; }

        /**
         * Returns the value of a state
         */
        const std::complex<float> &state(const size_t i) const { return _states.at(i); }

    private:
        const vu::ComplexVect _states;
    };

    /**
     * Returns the vector product of two matrices
     *
     * @param a the left matrices
     * @param b the right matrices
     */
    extern const Matrix cross(const Matrix &a, const Matrix &b);
}

extern const qb::Matrix operator*(const qb::Matrix &a, const qb::Matrix &b);
extern const qb::Ket operator*(const qb::Matrix &a, const qb::Ket &b);
extern const qb::Bra operator*(const qb::Bra &a, const qb::Matrix &b);
extern std::ostream &operator<<(std::ostream &stream, const qb::Matrix &a);

extern const qb::Ket operator-(const qb::Ket &a);
extern const qb::Ket operator+(const qb::Ket &a, const qb::Ket &b);
extern const qb::Ket operator-(const qb::Ket &a, const qb::Ket &b);
extern const qb::Ket operator*(const qb::Ket &a, const qb::Ket &b);
extern const qb::Ket operator*(const qb::Ket &a, const std::complex<float> &lambda);
extern const qb::Ket operator*(const std::complex<float> &lambda, const qb::Ket &a);
extern std::ostream &operator<<(std::ostream &stream, const qb::Ket &a);

extern const qb::Bra operator-(const qb::Bra &a);
extern const qb::Bra operator+(const qb::Bra &a, const qb::Bra &b);
extern const qb::Bra operator-(const qb::Bra &a, const qb::Bra &b);
extern const qb::Bra operator*(const qb::Bra &a, const qb::Bra &b);
extern const qb::Bra operator*(const qb::Bra &a, const std::complex<float> &lambda);
extern const qb::Bra operator*(const std::complex<float> &lambda, const qb::Bra &a);
extern const std::complex<float> operator*(const qb::Bra &a, const qb::Ket &b);
extern std::ostream &operator<<(std::ostream &stream, const qb::Bra &a);

#endif