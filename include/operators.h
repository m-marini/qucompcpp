#ifndef _operators_h_
#define _operators_h_

#include <string>
#include <functional>

#include "values.h"

namespace qc
{

    class SourceContext;

    typedef std::function<const Value *(const SourceContext &, const int)> IntMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const std::complex<double> &)> ComplexMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const mx::Matrix &)> MatrixMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const std::vector<Value *> &)> ListMapperFunction;

    typedef std::function<const Value *(const SourceContext &, const mx::Matrix &, const mx::Matrix &)> MatrixMatrixMapperFunction;

    class UnaryOperator
    {
    public:
        virtual const Value *apply(const SourceContext &context, const Value &value) const = 0;
    };

    class ChainUnaryOperator : public UnaryOperator
    {
    protected:
        const UnaryOperator *_other;

    public:
        ChainUnaryOperator(void) : _other(NULL) {}
        ChainUnaryOperator(const UnaryOperator *other) : _other(other) {}

        ~ChainUnaryOperator();

        ChainUnaryOperator *mapIntValue(const IntMapperFunction &mapper) const;
        ChainUnaryOperator *mapComplexValue(const ComplexMapperFunction &mapper) const;
        ChainUnaryOperator *mapMatrixValue(const MatrixMapperFunction &mapper) const;
    };

    class UnaryErrorOperator : public ChainUnaryOperator
    {
        std::string _format;

    public:
        UnaryErrorOperator(const std::string &format) : _format(format) {}

        virtual const Value *apply(const SourceContext &context, const Value &value) const override;
    };

    class UnaryIntOperator : public ChainUnaryOperator
    {
        IntMapperFunction _mapper;

    public:
        UnaryIntOperator(const IntMapperFunction &mapper,
                         const UnaryOperator *other) : ChainUnaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &value) const override;
    };

    class UnaryComplexOperator : public ChainUnaryOperator
    {
        ComplexMapperFunction _mapper;

    public:
        UnaryComplexOperator(const ComplexMapperFunction &mapper,
                             const UnaryOperator *other) : ChainUnaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &value) const override;
    };

    class UnaryMatrixOperator : public ChainUnaryOperator
    {
        MatrixMapperFunction _mapper;

    public:
        UnaryMatrixOperator(const MatrixMapperFunction &mapper,
                            const UnaryOperator *other) : ChainUnaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &value) const override;
    };

    class BinaryOperator
    {
    public:
        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const = 0;
    };

    class ChainBinaryOperator : public BinaryOperator
    {
    protected:
        const BinaryOperator *_other;

    public:
        ChainBinaryOperator(void) : _other(NULL) {}
        ChainBinaryOperator(const BinaryOperator *other) : _other(other) {}

        ~ChainBinaryOperator();

        ChainBinaryOperator *mapMatrixMatrixValue(const MatrixMatrixMapperFunction &mapper) const;
    };

    class BinaryErrorOperator : public ChainBinaryOperator
    {
        std::string _format;

    public:
        BinaryErrorOperator(const std::string &format) : _format(format) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    class MatrixMatrixOperator : public ChainBinaryOperator
    {
        MatrixMatrixMapperFunction _mapper;

    public:
        MatrixMatrixOperator(const MatrixMatrixMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };
}
#endif
