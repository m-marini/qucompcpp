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
    result.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        result.push_back(a.at(i) + b.at(i));
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
    result.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        result.push_back(a.at(i) - b.at(i));
    }
    return result;
}

const ComplexVect vu::operator-(const ComplexVect &a)
{
    const size_t n = a.size();
    ComplexVect result;
    result.reserve(n);
    for (complex<double> s : a)
    {
        result.push_back(-s);
    }
    return result;
}

const ComplexVect vu::operator*(const complex<double> &lambda, const ComplexVect &a)
{
    const size_t n = a.size();
    ComplexVect result;
    result.reserve(n);
    for (complex<double> s : a)
    {
        result.push_back(lambda * s);
    }
    return result;
}

const ComplexVect vu::operator/(const ComplexVect &left, const complex<double> &right)
{
    const size_t n = left.size();
    ComplexVect result;
    result.reserve(n);
    for (complex<double> s : left)
    {
        result.push_back(s / right);
    }
    return result;
}

const ComplexVect vu::conj(const ComplexVect &a)
{
    const size_t n = a.size();
    ComplexVect result;
    result.reserve(n);
    for (complex<double> s : a)
    {
        result.push_back(conj(s));
    }
    return result;
}

const ComplexVect vu::operator*(const ComplexVect &a, const ComplexVect &b)
{
    ComplexVect result;
    result.reserve(a.size() * b.size());
    for (complex<double> va : a)
    {
        for (complex<double> vb : b)
        {
            result.push_back(va * vb);
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
            complex<double> cell;
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

const size_t vu::numBitsByState(const size_t state)
{
    int n = 0;
    size_t s = state;
    do
    {
        n++;
    } while ((s >>= 1) != 0);
    return max(n, 1);
}
