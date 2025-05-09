#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include "qubit.h"

using namespace std;
using namespace bra;
using namespace ket;

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
    vector<complex<float>> result;
    for (complex<float> s : states())
    {
        auto sum = std::conj(s);
        result.insert(result.end(), sum);
    }
    return Ket(result);
}

const Bra operator+(const Bra &a, const Bra &b)
{
    vector<complex<float>> result;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = a.state(i) + b.state(i);
        result.insert(result.end(), sum);
    }
    return Bra(result);
}

const Bra operator-(const Bra &a, const Bra &b)
{
    vector<complex<float>> result;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = a.state(i) - b.state(i);
        result.insert(result.end(), sum);
    }
    return Bra(result);
}

const Bra operator-(const Bra &a)
{
    vector<complex<float>> result;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = -a.state(i);
        result.insert(result.end(), sum);
    }
    return Bra(result);
}

const Bra operator*(const Bra &a, const complex<float> &lambda)
{
    vector<complex<float>> result;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = a.state(i) * lambda;
        result.insert(result.end(), sum);
    }
    return Bra(result);
}

const complex<float> operator*(const Bra &a, const Ket &b)
{
    complex<float> result(0);
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = a.state(i) * b.state(i);
        result += sum;
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

const Bra operator*(const complex<float> &lambda, const Bra &a) { return a * lambda; }

const Bra bra::zero({conj(complex<float>(1)), conj(complex<float>(0))});
const Bra bra::one({conj(complex<float>(0)), conj(complex<float>(1))});
const Bra bra::i({conj(complex<float>(HALF_SQRT2)), conj(complex<float>(0, HALF_SQRT2))});
const Bra bra::minus_i({conj(complex<float>(HALF_SQRT2)), conj(complex<float>(0, -HALF_SQRT2))});
const Bra bra::plus({conj(complex<float>(HALF_SQRT2)), conj(complex<float>(HALF_SQRT2))});
const Bra bra::minus({complex<float>(HALF_SQRT2), -complex<float>(HALF_SQRT2)});
