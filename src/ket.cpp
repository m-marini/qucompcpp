#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include "qubit.h"

using namespace std;
using namespace ket;
using namespace bra;

Ket::Ket(const vector<complex<float>> &states)
{
    _states = new vector<complex<float>>(states);
}

Ket::Ket(const Ket &ket)
{
    _states = new vector<complex<float>>(ket.states());
}

Ket::~Ket()
{
    delete _states;
}

const float Ket::norm(void) const
{
    auto tot = 0.0;
    for (auto s : *_states)
    {
        tot += std::norm(s);
    }
    return tot;
}

const Bra Ket::conj(void) const
{
    vector<complex<float>> result;
    for (complex<float> s : states())
    {
        auto sum = std::conj(s);
        result.insert(result.end(), sum);
    }
    return Bra(result);
}

const Ket operator+(const Ket &a, const Ket &b)
{
    vector<complex<float>> result;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = a.state(i) + b.state(i);
        result.insert(result.end(), sum);
    }
    return Ket(result);
}

const Ket operator-(const Ket &a, const Ket &b)
{
    vector<complex<float>> result;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = a.state(i) - b.state(i);
        result.insert(result.end(), sum);
    }
    return Ket(result);
}

const Ket operator-(const Ket &a)
{
    vector<complex<float>> result;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = -a.state(i);
        result.insert(result.end(), sum);
    }
    return Ket(result);
}

const Ket operator*(const Ket &a, const complex<float> &lambda)
{
    vector<complex<float>> result;
    const size_t n = a.size();
    for (size_t i = 0; i < n; i++)
    {
        auto sum = a.state(i) * lambda;
        result.insert(result.end(), sum);
    }
    return Ket(result);
}

const Ket ket::base(const size_t value, const size_t size)
{
    const size_t n = 1 << size;
    vector<complex<float>> result;
    for (size_t i = 0; i < n; i++)
    {
        result.insert(result.end(), i == value ? 1.0f : 0.0f);
    }
    return Ket(result);
}

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

const Ket operator*(const complex<float> &lambda, const Ket &a) { return a * lambda; }

const Ket ket::zero({complex<float>(1), complex<float>(0)});
const Ket ket::one({complex<float>(0), complex<float>(1)});
const Ket ket::i({complex<float>(sqrt(2) / 2), complex<float>(0, sqrt(2) / 2)});
const Ket ket::minus_i({complex<float>(sqrt(2) / 2), -complex<float>(0, sqrt(2) / 2)});
const Ket ket::plus({complex<float>(sqrt(2) / 2), complex<float>(sqrt(2) / 2)});
const Ket ket::minus({complex<float>(sqrt(2) / 2), -complex<float>(sqrt(2) / 2)});
