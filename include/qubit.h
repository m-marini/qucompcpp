#ifndef _qubit_h_
#define _qubit_h_

#include <vector>
#include <complex>

namespace bra
{
    class Bra;
}

namespace ket
{
    /**
     * Defines the Ket
     */
    class Ket
    {
    public:
        /**
         * Constructor
         */
        Ket(const std::vector<std::complex<float>> &states);
        Ket(const Ket &ket);

        ~Ket();

        /**
         * Returns the conjugate
         */
        const bra::Bra conj(void) const;

        /**
         * Returns the number of states
         */
        const size_t size(void) const { return _states->size(); }

        /**
         * Returns the states
         */
        const std::vector<std::complex<float>> &states(void) const { return *_states; }

        /**
         * Returns the value of a state
         */
        const std::complex<float> state(const size_t i) const { return (*_states)[i]; }

        /**
         * Returns the square of module
         */
        const float norm(void) const;

    private:
        const std::vector<std::complex<float>> *_states;
    };

    extern const Ket zero;
    extern const Ket one;
    extern const Ket i;
    extern const Ket minus_i;
    extern const Ket plus;
    extern const Ket minus;

    extern const Ket base(const size_t i, const size_t size);
}

namespace bra
{
    /**
     * Defines the Ket
     */
    class Bra
    {
    public:
        /**
         * Constructor
         */
        Bra(const std::vector<std::complex<float>> &states);
        Bra(const Bra &bra);

        ~Bra();

        /**
         * Returns the conjugate
         */
        const ket::Ket conj(void) const;

        /**
         * Returns the number of states
         */
        const size_t size(void) const { return _states->size(); }

        /**
         * Returns the states
         */
        const std::vector<std::complex<float>> &states(void) const { return *_states; }

        /**
         * Returns the value of a state
         */
        const std::complex<float> state(const size_t i) const { return (*_states)[i]; }

        /**
         * Returns the square of module
         */
        const float norm(void) const;

    private:
        const std::vector<std::complex<float>> *_states;
    };

    extern const Bra zero;
    extern const Bra one;
    extern const Bra i;
    extern const Bra minus_i;
    extern const Bra plus;
    extern const Bra minus;

    extern const Bra base(const size_t i, const size_t size);
}

extern const ket::Ket operator+(const ket::Ket &a, const ket::Ket &b);
extern const ket::Ket operator-(const ket::Ket &a, const ket::Ket &b);
extern const ket::Ket operator-(const ket::Ket &a);
extern const ket::Ket operator*(const ket::Ket &a, const std::complex<float> &lambda);
extern std::ostream &operator<<(std::ostream &stream, const ket::Ket &a);
extern const ket::Ket operator*(const std::complex<float> &lambda, const ket::Ket &a);

extern const bra::Bra operator+(const bra::Bra &a, const bra::Bra &b);
extern const bra::Bra operator-(const bra::Bra &a, const bra::Bra &b);
extern const bra::Bra operator-(const bra::Bra &a);
extern const bra::Bra operator*(const bra::Bra &a, const std::complex<float> &lambda);
extern std::ostream &operator<<(std::ostream &stream, const bra::Bra &a);
extern const bra::Bra operator*(const std::complex<float> &lambda, const bra::Bra &a);

extern const std::complex<float> operator*(const bra::Bra &a, const ket::Ket &b);

#endif