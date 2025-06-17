#ifndef _matrix_h_
#define _matrix_h_

#include <string>
#include <vector>

#include "vectutils.h"

namespace mx
{
    typedef std::vector<size_t> indices_t;

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

        /**
         * Return the transpose congiugate matrix
         */
        const Matrix dagger(void) const { return transpose().conj(); }

        /**
         * Return the congiugate matrix
         */
        const Matrix conj(void) const;

        /**
         * Return the negate matrix
         */
        const Matrix operator-(void) const;

        /**
         * Return the product matrix by complex
         */
        const Matrix operator*(const std::complex<double> &) const;

        /**
         * Return the sum matrix plus matrix
         */
        const Matrix operator+(const Matrix &) const;

        /**
         * Return the difference matrix from matrix
         */
        const Matrix operator-(const Matrix &) const;

        /**
         * Return the product matrix by matrix with zero fill extension
         */
        const Matrix operator*(const Matrix &) const;

        /**
         * Return the product matrix by matrix with cross extension
         */
        const Matrix multiply(const Matrix &) const;

        /**
         * Return the divided matrix by complex
         */
        const Matrix operator/(const std::complex<double> &) const;

        /**
         * Returns the tensor produce
         */
        const Matrix cross(const Matrix &a) const;

        /**
         * Return the transpose congiugate matrix
         */
        const Matrix transpose(void) const;

        const Matrix extendsCross(const int size) const;
        const Matrix extendsRows(const int numRows) const;
        const Matrix extendsCols(const int numCols) const;
        const Matrix extends0(const int numRows, const int numCols) const
        {
            return extendsCols(numCols)
                .extendsRows(numRows);
        }
    };

    extern const Matrix PLUS_KET;
    extern const Matrix MINUS_KET;
    extern const Matrix I_KET;
    extern const Matrix MINUS_I_KET;

    /**
     * Returns the ket base
     * @state the state
     */
    extern const Matrix ketBase(const int state);

    /**
     * Returns the matrix base
     * @state the state
     */
    extern const Matrix ary(const int i, const int j);

    /**
     * Returns the symetric matrix  base
     * @state the state
     */
    extern const Matrix sim(const int i, const int j);

    /**
     * Returns the antisymetric matrix base
     * @state the state
     */
    extern const Matrix eps(const int i, const int j);

    /**
     * Returns the I gate
     * @qubit the bit to apply the I gates
     */
    extern const Matrix I(const size_t qubit);

    /**
     * Returns the H gate
     * @qubit the bit to apply the I gates
     */
    extern const Matrix H(const size_t qubit);

    /**
     * Returns the S gate
     * @qubit the bit to apply the I gates
     */
    extern const Matrix S(const size_t qubit);

    /**
     * Returns the X gate
     * @qubit the bit to apply the I gates
     */
    extern const Matrix T(const size_t qubit);

    /**
     * Returns the X gate
     * @qubit the bit to apply the I gates
     */
    extern const Matrix X(const size_t qubit);

    /**
     * Returns the Y gate
     * @qubit the bit to apply the I gates
     */
    extern const Matrix Y(const size_t qubit);

    /**
     * Returns the Z gate
     * @qubit the bit to apply the I gates
     */
    extern const Matrix Z(const size_t qubit);

    /**
     * Returns the SWAP gate
     * @index0 the first bit index
     * @index0 the second bit index
     */
    extern const Matrix SWAP(const size_t index0, const size_t index1);

    /**
     * Returns the CNOT gate
     * @index0 the data bit index
     * @index0 the control bit index
     */
    extern const Matrix CNOT(const size_t data, const size_t control);

    /**
     * Returns the CCNOT gate
     * @index0 the data bit index
     * @index0 the control0 the first control bit index
     * @index0 the control1 the second control bit index
     */
    extern const Matrix CCNOT(const size_t data, const size_t control0, const size_t control1);

    /**
     * Returns the qubit 0 value projection matrix
     *
     * @param index     the qubit index
     * @param numQubits the number of qubits
     */
    extern const Matrix qubit0(const size_t index, const size_t numQubits);

    /**
     * Returns the qubit 1 value projection matrix
     *
     * @param index     the qubit index
     * @param numQubits the number of qubits
     */
    extern const Matrix qubit1(const size_t index, const size_t numQubits);

    /**
     * Returns the identity matrix
     * @size the size of matrix
     */
    const Matrix identity(const size_t size);

    extern const indices_t computeBitsPermutation(const indices_t &bitMap);
    extern const indices_t computeStatePermutation(const indices_t &bitPermutation);
    extern const indices_t inversePermutation(const indices_t s);
    extern const Matrix permute(const indices_t &s);

    /**
     * Returns the matrix of cnot gate applied to the given bits
     *
     * @param baseGate the base gate matrix
     * @param bitMap   the bit map
     */
    extern const Matrix createGate(Matrix baseGate, const indices_t bitMap);

    extern const std::string fmt(const std::complex<double> &value);
}

extern std::ostream &operator<<(std::ostream &stream, const mx::Matrix &a);
extern std::ostream &operator<<(std::ostream &out, const mx::indices_t &indices);

extern const std::string to_string(const mx::Matrix &a);
extern const std::string to_string(const mx::indices_t &indices);

#endif