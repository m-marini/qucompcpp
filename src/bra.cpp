#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include "qubit.h"

using namespace std;
using namespace qb;
using namespace vu;

#define HALF_SQRT2 (sqrt(2) / 2)

Bra::Bra(const vector<complex<float>> &states)
{
    _states = new vector<complex<float>>(states);
}

Bra::~Bra()
{
    delete _states;
}

const float Bra::norm(void) const
{
    auto tot = 0.0;
    for (auto s : *_states)
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
    for (size_t i = 0; i < n; i++)
    {
        result.insert(result.end(), i == value ? 1.0f : 0.0f);
    }
    return Bra(result);
}

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
    complex<float> result;
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

const Bra Bra::zero({std::conj(complex<float>(1)), std::conj(complex<float>(0))});
const Bra Bra::one({std::conj(complex<float>(0)), std::conj(complex<float>(1))});
const Bra Bra::i({std::conj(complex<float>(HALF_SQRT2)), std::conj(complex<float>(0, HALF_SQRT2))});
const Bra Bra::minus_i({std::conj(complex<float>(HALF_SQRT2)), std::conj(complex<float>(0, -HALF_SQRT2))});
const Bra Bra::plus({std::conj(complex<float>(HALF_SQRT2)), std::conj(complex<float>(HALF_SQRT2))});
const Bra Bra::minus({complex<float>(HALF_SQRT2), -complex<float>(HALF_SQRT2)});
