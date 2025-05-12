#ifndef _qugates_h
#define _qugates_h

#include <string>
#include <vector>
#include <optional>

#include "qubit.h"

namespace qg
{
    typedef std::vector<size_t> indices_t;

    /**
     * Defines a quantum gate
     */
    class QuGate
    {
    public:
        /**
         * Returns the I gate (identity)
         *
         * @param qubit the data bit
         */
        static const QuGate i(const size_t qubit);

        /**
         * Returns the h gate (Hadamard)
         *
         * @param data the data bit index
         */
        static const QuGate h(const size_t data);
        /**
         * Returns the x gate
         *
         * @param data the data bit index
         */
        static const QuGate x(const size_t data);

        /**
         * Returns the y gate
         *
         * @param data the data bit index
         */
        static const QuGate y(const size_t data);

        /**
         * Returns the z gate
         *
         * @param data the data bit index
         */
        static const QuGate z(const size_t data);

        /**
         * Returns the s gate
         *
         * @param data the data bit index
         */
        static const QuGate s(const size_t data);

        /**
         * Returns the t gate
         *
         * @param data the data bit index
         */
        static const QuGate t(const size_t data);

        /**
         * Returns the swap gate
         *
         * @param data0 the data0 bit index
         * @param data1 the data1 bit index
         */
        static const QuGate swap(const size_t data0, const size_t data1);

        /**
         * Returns the cnot gate
         *
         * @param control the control bit index
         * @param data the data bit index
         */
        static const QuGate cnot(const size_t control, const size_t data);

        /**
         * Returns the cnot gate
         *
         * @param c0 the first control bit index
         * @param c1 the second control bit index
         * @param data the data bit index
         */
        static const QuGate ccnot(const size_t c0, const size_t c1, const size_t data);

        /**
         * Creates the gates
         *
         * @param id the gate id
         * @param indices the qubit indices
         * @param matrix the base matrix
         */
        QuGate(const std::string &id, const indices_t &indices, const qb::Matrix &matrix) : _id(id), _indices(indices), _matrix(matrix) {}
        QuGate(const QuGate &gate) : _id(gate._id), _indices(gate._indices), _matrix(gate._matrix) {}

        QuGate &operator=(const QuGate &gate)
        {
            _id = gate._id;
            _indices = gate._indices;
            _matrix = gate._matrix;
            return *this;
        }

        /**
         * Returns the gate id
         */
        const std::string &id(void) const { return _id; }

        /**
         * Returns the qubit indices
         */
        const indices_t &indices(void) const { return _indices; }

        /**
         * Returns the qubit indices
         */
        const qb::Matrix &matrix(void) const { return _matrix; }

        /**
         * Returns the circuit transformation matrix of the gate
         *
         * @param numBits the number of circuit qubits
         */
        const qb::Matrix build(const size_t numBits) const;

        /**
         * Returns the highest bit index of the gate
         */
        const size_t maxIndices(void) const;

    private:
        std::string _id;
        indices_t _indices;
        qb::Matrix _matrix;
    };

    typedef std::vector<QuGate> gates_t;

    /**
     * Returns the matrix of circuit composed by the gates
     * @param gates the gates
     */
    extern const qb::Matrix buildCircuit(const gates_t &gates);

    /**
     * Returns the qubits number of circuit composed by the gates
     * @param gates the gates
     */
    extern const size_t numQuBits(const gates_t &gates);
}

std::ostream &operator<<(std::ostream &os, const qg::QuGate &gate);
std::ostream &operator<<(std::ostream &os, const qg::indices_t &indices);

#endif
