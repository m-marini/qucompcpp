#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include "qubit.h"

using namespace std;
using namespace qb;
using namespace vu;

const float Ket::norm(void) const
{
    auto tot = 0.0;
    for (auto s : _states)
    {
        tot += std::norm(s);
    }
    return tot;
}

const Bra Ket::conj(void) const
{
    return Bra(vu::conj(states()));
}

const size_t Ket::numBits(void) const
{
    size_t ns = _states.size();
    size_t n = 0;
    while (ns > 0)
    {
        n++;
        ns >>= 1;
    }
    return n;
}

const float Ket::bitProb(const size_t i) const
{
    const size_t mask = 1 << i;
    const size_t n = _states.size();
    float result = 0;
    for (size_t j = 0; j < n; j++)
    {
        if ((j & mask) != 0)
        {
            const complex<float> s = _states[j];
            result += std::norm(s);
        }
    }
    return result;
}

const Ket operator+(const Ket &a, const Ket &b)
{
    return Ket(a.states() + b.states());
}

const Ket operator-(const Ket &a, const Ket &b)
{
    return Ket(a.states() - b.states());
}

const Ket operator*(const Ket &a, const Ket &b)
{
    return Ket(a.states() * b.states());
}

const Ket operator-(const Ket &a)
{
    return Ket(-a.states());
}

const Ket operator*(const Ket &a, const complex<float> &lambda)
{
    return Ket(lambda * a.states());
}

const Ket operator*(const complex<float> &lambda, const Ket &a)
{
    return Ket(lambda * a.states());
}

const Ket Ket::base(const size_t value, const size_t size)
{
    const size_t n = 1 << size;
    vector<complex<float>> result;
    result.assign(n, 0);
    result[value] = 1;
    return Ket(result);
}

const Ket Ket::zero({complex<float>(1), complex<float>(0)});
const Ket Ket::one({complex<float>(0), complex<float>(1)});
const Ket Ket::i({complex<float>(sqrt(2) / 2), complex<float>(0, sqrt(2) / 2)});
const Ket Ket::minus_i({complex<float>(sqrt(2) / 2), -complex<float>(0, sqrt(2) / 2)});
const Ket Ket::plus({complex<float>(sqrt(2) / 2), complex<float>(sqrt(2) / 2)});
const Ket Ket::minus({complex<float>(sqrt(2) / 2), -complex<float>(sqrt(2) / 2)});

ostream &operator<<(ostream &os, const Ket &ket)
{
    bool isFirst = true;
    const size_t n = ket.size();
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
        os << ket.state(i) << " |" << i << ">";
    }
    return os;
}
