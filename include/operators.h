#ifndef _operators_h_
#define _operators_h_

#include <string>
#include <functional>
#include <complex>

#include "values.h"

namespace qc
{

    class SourceContext;

    typedef std::function<const Value *(const SourceContext &, const int)> IntMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const std::complex<double> &)> ComplexMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const mx::Matrix &)> MatrixMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const std::vector<Value *> &)> ListMapperFunction;

    typedef std::function<const Value *(const SourceContext &, const int, const int)> IntIntMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const int, const std::complex<double> &)> IntComplexMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const int, const mx::Matrix &)> IntMatrixMapperFunction;

    typedef std::function<const Value *(const SourceContext &, const std::complex<double> &, const int)> ComplexIntMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const std::complex<double> &, const std::complex<double> &)> ComplexComplexMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const std::complex<double> &, const mx::Matrix &)> ComplexMatrixMapperFunction;

    typedef std::function<const Value *(const SourceContext &, const mx::Matrix &, const int)> MatrixIntMapperFunction;
    typedef std::function<const Value *(const SourceContext &, const mx::Matrix &, const std::complex<double> &)> MatrixComplexMapperFunction;
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

        ChainUnaryOperator *mapInt(const IntMapperFunction &mapper) const;
        ChainUnaryOperator *mapComplex(const ComplexMapperFunction &mapper) const;
        ChainUnaryOperator *mapMatrix(const MatrixMapperFunction &mapper) const;
    };

    class UnaryErrorOperator : public ChainUnaryOperator
    {
        std::string _format;

    public:
        UnaryErrorOperator(const std::string &format) : _format(format) {}

        UnaryErrorOperator(void) : UnaryErrorOperator("Unexpected argument") {}

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

        ChainBinaryOperator *mapIntInt(const IntIntMapperFunction &mapper) const;
        ChainBinaryOperator *mapIntComplex(const IntComplexMapperFunction &mapper) const;
        ChainBinaryOperator *mapIntMatrix(const IntMatrixMapperFunction &mapper) const;
        ChainBinaryOperator *mapComplexInt(const ComplexIntMapperFunction &mapper) const;
        ChainBinaryOperator *mapComplexComplex(const ComplexComplexMapperFunction &mapper) const;
        ChainBinaryOperator *mapComplexMatrix(const ComplexMatrixMapperFunction &mapper) const;
        ChainBinaryOperator *mapMatrixInt(const MatrixIntMapperFunction &mapper) const;
        ChainBinaryOperator *mapMatrixComplex(const MatrixComplexMapperFunction &mapper) const;
        ChainBinaryOperator *mapMatrixMatrix(const MatrixMatrixMapperFunction &mapper) const;
    };

    class BinaryErrorOperator : public ChainBinaryOperator
    {
        std::string _format;

    public:

        BinaryErrorOperator(const std::string &format) : _format(format) {}

        BinaryErrorOperator(void) : BinaryErrorOperator("Unexpected arguments") {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator int, int
     */
    class IntIntOperator : public ChainBinaryOperator
    {
        IntIntMapperFunction _mapper;

    public:
        IntIntOperator(const IntIntMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator int, complex
     */
    class IntComplexOperator : public ChainBinaryOperator
    {
        IntComplexMapperFunction _mapper;

    public:
        IntComplexOperator(const IntComplexMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator int, matrix
     */

    class IntMatrixOperator : public ChainBinaryOperator
    {
        IntMatrixMapperFunction _mapper;

    public:
        IntMatrixOperator(const IntMatrixMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator complex, int
     */
    class ComplexIntOperator : public ChainBinaryOperator
    {
        ComplexIntMapperFunction _mapper;

    public:
        ComplexIntOperator(const ComplexIntMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator complex, complex
     */
    class ComplexComplexOperator : public ChainBinaryOperator
    {
        ComplexComplexMapperFunction _mapper;

    public:
        ComplexComplexOperator(const ComplexComplexMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator complex, matrix
     */
    class ComplexMatrixOperator : public ChainBinaryOperator
    {
        ComplexMatrixMapperFunction _mapper;

    public:
        ComplexMatrixOperator(const ComplexMatrixMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator matrix int
     */
    class MatrixIntOperator : public ChainBinaryOperator
    {
        MatrixIntMapperFunction _mapper;

    public:
        MatrixIntOperator(const MatrixIntMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator matrix, complex
     */
    class MatrixComplexOperator : public ChainBinaryOperator
    {
        MatrixComplexMapperFunction _mapper;

    public:
        MatrixComplexOperator(const MatrixComplexMapperFunction &mapper, const BinaryOperator *other)
            : ChainBinaryOperator(other), _mapper(mapper) {}

        virtual const Value *apply(const SourceContext &context, const Value &left, const Value &right) const override;
    };

    /**
     * Binary operator matrix, matrix
     */
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
