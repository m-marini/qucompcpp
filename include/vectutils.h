#ifndef _VectUtils_h
#define _VectUtils_h

#include <vector>
#include <complex>

namespace vu
{
    typedef std::vector<std::complex<double>> ComplexVect;

    extern const ComplexVect operator-(const ComplexVect &a);
    extern const ComplexVect operator+(const ComplexVect &a, const ComplexVect &b);
    extern const ComplexVect operator-(const ComplexVect &a, const ComplexVect &b);
    extern const ComplexVect operator*(const ComplexVect &a, const ComplexVect &b);
    extern const ComplexVect operator*(const std::complex<double> &a, const ComplexVect &b);
    extern const ComplexVect operator/(const ComplexVect &a, const std::complex<double> &b);
    extern const ComplexVect conj(const ComplexVect &a);
    extern const ComplexVect operator-(const ComplexVect &a);
    extern const size_t numBitsByState(const size_t state);

    /**
     * Partial matrix multiplication
     *
     * @param d       the destination matrix
     * @param dOffset the destination matrix offset
     * @param numRow  the number of computation rows
     * @param numCols the number of computation columns
     * @param a       the left source matrix
     * @param aOffset the left source matrix offset
     * @param aStride the left source matrix stride (number of columns)
     * @param b       the right source matrix
     * @param bOffset the right source matrix offset
     * @param bStride the right source matrix stride (number of colums)
     */
    extern ComplexVect &partMul(ComplexVect &d, const size_t dOffset, const size_t numRow, const size_t numCols,
                                const ComplexVect &a, const size_t aOffset, const size_t aStride,
                                const ComplexVect &b, const size_t bOffset, const size_t bStride);
}
#endif
