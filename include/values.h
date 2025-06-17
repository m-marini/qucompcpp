#ifndef _values_h_
#define _values_h_

#include <complex>
#include <vector>
#include <map>
#include <ostream>

#include "sourceContext.h"
#include "matrix.h"

namespace qc
{
    enum class ValueType
    {
        intValueType,
        complexValueType,
        matrixValueType,
        listValueType
    };

    class Value
    {
        SourceContext _source;

    protected:
        Value(const SourceContext &source) : _source(source) {}

    public:
        const SourceContext &source(void) const { return _source; }

        virtual const ValueType type(void) const = 0;

        virtual const Value *clone(void) const = 0;

        virtual const Value *source(const SourceContext &source) const = 0;

        virtual std::ostream &write(std::ostream &stream) const { return stream << "value"; }
    };

    class IntValue : public Value
    {
        int _value;

    public:
        IntValue(const SourceContext &source, int value) : Value(source), _value(value) {}

        virtual const ValueType type(void) const override { return ValueType::intValueType; };

        const int value(void) const { return _value; }

        virtual std::ostream &write(std::ostream &stream) const override { return stream << _value; }

        virtual const Value *clone(void) const override { return new IntValue(*this); }

        virtual const Value *source(const SourceContext &source) const override { return new IntValue(source, _value); };
    };

    class ComplexValue : public Value
    {
        std::complex<double> _value;

    public:
        ComplexValue(const SourceContext &source, const std::complex<double> &value) : Value(source), _value(value) {}

        virtual const ValueType type(void) const override { return ValueType::complexValueType; };

        const std::complex<double> &value(void) const { return _value; }

        virtual const Value *clone(void) const override { return new ComplexValue(*this); }

        virtual const Value *source(const SourceContext &source) const override { return new ComplexValue(source, _value); };

        virtual std::ostream &write(std::ostream &stream) const override { return stream << mx::fmt(_value); }
    };

    class MatrixValue : public Value
    {
        mx::Matrix _value;

    public:
        MatrixValue(const SourceContext &source, const mx::Matrix &value) : Value(source), _value(value) {}

        virtual const ValueType type(void) const override { return ValueType::matrixValueType; };

        const mx::Matrix &value(void) const { return _value; }

        virtual const Value *clone(void) const override { return new MatrixValue(*this); }

        virtual const Value *source(const SourceContext &source) const override { return new MatrixValue(source, _value); };

        virtual std::ostream &write(std::ostream &stream) const override { return stream << _value; }
    };

    class ListValue : public Value
    {
        std::vector<const Value *> _values;

    public:
        ListValue(const SourceContext &source, const std::vector<const Value *> values) : Value(source), _values(values) {}
        ~ListValue();

        virtual const ValueType type(void) const override { return ValueType::listValueType; };

        const std::vector<const Value *> &values(void) const { return _values; }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *clone(void) const override { throw std::logic_error("Clone of list values is not supported"); }

        virtual const Value *source(const SourceContext &source) const override { return new ListValue(source, _values); };
    };

}

extern std::ostream &operator<<(std::ostream &stream, const qc::Value &value);
extern const std::string to_string(const qc::Value &value);

extern std::ostream &operator<<(std::ostream &stream, const qc::ValueType &type);
extern const std::string to_string(const qc::ValueType &value);

#endif
