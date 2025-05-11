#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include "qubit.h"

using namespace std;
using namespace qb;
using namespace vu;

#define HALF_SQRT2 (sqrt(2) / 2)

const float Bra::norm(void) const
{
    float tot = 0;
    for (auto s : _states)
    {
        tot += std::norm(s);
    }
    return tot;
}

const Ket Bra::conj(void) const
{
    return Ket(vu::conj(states()));
}

const Bra Bra::base(const size_t value, const size_t size)
{
    const size_t n = 1 << size;
    vector<complex<float>> result;
    result.assign(n, 0);
    result[value] = 1;
    return Bra(result);
}

const Bra Bra::zero({std::conj(complex<float>(1)), std::conj(complex<float>(0))});
const Bra Bra::one({std::conj(complex<float>(0)), std::conj(complex<float>(1))});
const Bra Bra::i({std::conj(complex<float>(HALF_SQRT2)), std::conj(complex<float>(0, HALF_SQRT2))});
const Bra Bra::minus_i({std::conj(complex<float>(HALF_SQRT2)), std::conj(complex<float>(0, -HALF_SQRT2))});
const Bra Bra::plus({std::conj(complex<float>(HALF_SQRT2)), std::conj(complex<float>(HALF_SQRT2))});
const Bra Bra::minus({complex<float>(HALF_SQRT2), -complex<float>(HALF_SQRT2)});

const Bra operator+(const Bra &a, const Bra &b)
{
    return Bra(a.states() + b.states());
}

const Bra operator-(const Bra &a, const Bra &b)
{
    return Bra(a.states() - b.states());
}

const Bra operator*(const Bra &a, const Bra &b)
{
    return Bra(a.states() * b.states());
}

const Bra operator-(const Bra &a)
{
    return Bra(-a.states());
}

const Bra operator*(const Bra &a, const complex<float> &lambda)
{
    return Bra(lambda * a.states());
}

const Bra operator*(const complex<float> &lambda, const Bra &a)
{
    return Bra(lambda * a.states());
}

const complex<float> operator*(const Bra &a, const Ket &b)
{
    complex<float> result(0);
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        result += a.state(i) * b.state(i);
    }
    return result;
}

ostream &operator<<(ostream &os, const Bra &a)
{
    bool isFirst = true;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            os << " + ";
        }
        os << a.state(i) << " <" << i << "|";
    }
    return os;
}
