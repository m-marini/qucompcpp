#ifndef _values_h_
#define _values_h_

#include <complex>
#include <vector>
#include <ostream>

#include "matrix.h"

namespace qc
{
    class ValueMapper;

    class Value
    {
    public:
        virtual const Value *map(const ValueMapper &mapper) const = 0;
        virtual std::ostream &write(std::ostream &stream) const { return stream << "value"; }
    };

    class IntValue : public Value
    {
        int _value;

    public:
        IntValue(const int value) : _value(value) {}

        const int value(void) const { return _value; }

        virtual const Value *map(const ValueMapper &mapper) const override;

        virtual std::ostream &write(std::ostream &stream) const override { return stream << _value; }
    };

    class ComplexValue : public Value
    {
        std::complex<double> _value;

    public:
        ComplexValue(const std::complex<double> &value) : _value(value) {}

        const std::complex<double> &value(void) const { return _value; }

        virtual const Value *map(const ValueMapper &mapper) const override;

        virtual std::ostream &write(std::ostream &stream) const override { return stream << _value; }
    };

    class MatrixValue : public Value
    {
        mx::Matrix _value;

    public:
        MatrixValue(const mx::Matrix &value) : _value(value) {}

        const mx::Matrix &value(void) const { return _value; }

        virtual const Value *map(const ValueMapper &mapper) const override;

        virtual std::ostream &write(std::ostream &stream) const override { return stream << _value; }
    };

    class ListValue : public Value
    {
        std::vector<const Value *> _values;

    public:
        ListValue(const std::vector<const Value *> values) : _values(values) {}
        ~ListValue();

        const std::vector<const Value *> &values(void) const { return _values; }

        virtual const Value *map(const ValueMapper &mapper) const override;

        virtual std::ostream &write(std::ostream &stream) const override;
    };

    class ValueMapper
    {
    public:
        virtual const Value *map(const IntValue &value) const = 0;
        virtual const Value *map(const ComplexValue &value) const = 0;
        virtual const Value *map(const MatrixValue &value) const = 0;
        virtual const Value *map(const ListValue &value) const = 0;
    };

}

extern std::ostream &operator<<(std::ostream &stream, const qc::Value &value);
extern const std::string to_string(const qc::Value &value);

#endif