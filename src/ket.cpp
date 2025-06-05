#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include "qubit.h"

#define STATE_PER_LINE 8

using namespace std;
using namespace qb;
using namespace vu;

const double Ket::norm(void) const
{
    double tot = 0.0;
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

const double Ket::bitProb(const size_t i) const
{
    const size_t mask = 1 << i;
    const size_t n = _states.size();
    double result = 0;
    for (size_t j = 0; j < n; j++)
    {
        if ((j & mask) != 0)
        {
            const complex<double> s = _states[j];
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

const Ket operator*(const Ket &a, const complex<double> &lambda)
{
    return Ket(lambda * a.states());
}

const Ket operator*(const complex<double> &lambda, const Ket &a)
{
    return Ket(lambda * a.states());
}

const Ket Ket::base(const size_t value, const size_t size)
{
    const size_t n = 1 << size;
    vector<complex<double>> result;
    result.assign(n, 0);
    result[value] = 1;
    return Ket(result);
}

const Ket Ket::zero({complex<double>(1), complex<double>(0)});
const Ket Ket::one({complex<double>(0), complex<double>(1)});
const Ket Ket::i({complex<double>(sqrt(2) / 2), complex<double>(0, sqrt(2) / 2)});
const Ket Ket::minus_i({complex<double>(sqrt(2) / 2), -complex<double>(0, sqrt(2) / 2)});
const Ket Ket::plus({complex<double>(sqrt(2) / 2), complex<double>(sqrt(2) / 2)});
const Ket Ket::minus({complex<double>(sqrt(2) / 2), -complex<double>(sqrt(2) / 2)});

ostream &operator<<(ostream &os, const Ket &ket)
{
    bool isFirst = true;
    const size_t n = ket.size();
    for (size_t i = 0; i < n; i++)
    {
        const bool newLine = i % STATE_PER_LINE == 0 && i > 0;
        if (newLine)
        {
            os << endl;
        }
        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            os << (newLine ? "+ " : " + ");
        }
        os << ket.state(i) << " |" << i << ">";
    }
    return os;
}
