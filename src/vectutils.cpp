#include "vectutils.h"

using namespace vu;
using namespace std;

const ComplexVect vu::operator+(const ComplexVect &a, const ComplexVect &b)
{
    const size_t n = a.size();
    if (n != b.size())
    {
        throw invalid_argument(
            (ostringstream() << "adding vectors must have same size (" << n << " != " << b.size() << ")")
                .str());
    }
    ComplexVect result;
    for (size_t i = 0; i < n; i++)
    {
        result.insert(result.end(), a.at(i) + b.at(i));
    }
    return result;
}

const ComplexVect vu::operator-(const ComplexVect &a, const ComplexVect &b)
{
    const size_t n = a.size();
    if (n != b.size())
    {
        throw invalid_argument(
            (ostringstream() << "subtracting vectors must have same size (" << n << " != " << b.size() << ")")
                .str());
    }
    ComplexVect result;
    for (size_t i = 0; i < n; i++)
    {
        result.insert(result.end(), a.at(i) - b.at(i));
    }
    return result;
}

const ComplexVect vu::operator-(const ComplexVect &a)
{
    const size_t n = a.size();
    ComplexVect result;
    for (complex<float> s : a)
    {
        result.insert(result.end(), -s);
    }
    return result;
}

const ComplexVect vu::operator*(const complex<float> &lambda, const ComplexVect &a)
{
    const size_t n = a.size();
    ComplexVect result;
    for (complex<float> s : a)
    {
        result.insert(result.end(), lambda * s);
    }
    return result;
}

const ComplexVect vu::conj(const ComplexVect &a)
{
    const size_t n = a.size();
    ComplexVect result;
    for (complex<float> s : a)
    {
        result.insert(result.end(), conj(s));
    }
    return result;
}

const ComplexVect vu::operator*(const ComplexVect &a, const ComplexVect &b)
{
    ComplexVect result;
    for (complex<float> va : a)
    {
        for (complex<float> vb : b)
        {
            result.insert(result.end(), va * vb);
        }
    }
    return result;
}

ComplexVect &vu::partMul(ComplexVect &d, const size_t dOffset, const size_t numRow, const size_t numCols,
                         const ComplexVect &a, const size_t aOffset, const size_t aStride,
                         const ComplexVect &b, const size_t bOffset, const size_t bStride)
{
    size_t di = dOffset;
    size_t ai = aOffset;
    for (size_t i = 0; i < numRow; i++)
    {
        size_t dij = di;
        size_t bj = bOffset;
        for (size_t j = 0; j < numCols; j++)
        {
            complex<float> cell;
            size_t aik = ai;
            size_t bkj = bj;
            for (int k = 0; k < aStride; k++)
            {
                cell += a[aik] * b[bkj];
                aik++;
                bkj += bStride;
            }
            d[dij] = cell;
            dij++;
            bj++;
        }
        di += bStride;
        ai += aStride;
    }
    return d;
}