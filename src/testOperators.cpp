#include <gtest/gtest.h>

#include <functional>

#include "operators.h"
#include "sourceContext.h"

using namespace std;
using namespace qc;
using namespace mx;

TEST(testOperators, testUnaryError)
{
    UnaryErrorOperator mapper;
    const SourceContext context("1", "1", 1, 0);
    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, IntValue(context, 1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument integer", ex.what());
            throw;
         } }, QuExecException);

    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, ComplexValue(context,1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument complex", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, MatrixValue(context,PLUS_KET));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument matrix", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, ListValue(context,{new IntValue(context,1),new IntValue(context,1)}));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument list", ex.what());
            throw;
         } }, QuExecException);
}

TEST(testOperators, testIntChain)
{
    IntMapperFunction f = [](const SourceContext &context, const int value)
    {
        return new IntValue(context, 3);
    };
    ChainUnaryOperator *mapper = (new UnaryErrorOperator())->mapInt(f);

    const SourceContext context("1", "1", 1, 0);
    const Value *val = mapper->apply(context, IntValue(context, 1));

    EXPECT_EQ("3", to_string(*val));

    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ComplexValue(context,1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument complex", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, MatrixValue(context,PLUS_KET));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument matrix", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ListValue(context,{new IntValue(context,1),new IntValue(context,1)}));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument list", ex.what());
            throw;
         } }, QuExecException);
    delete mapper;
}

TEST(testOperators, testComplexChain)
{
    ComplexMapperFunction f = [](const SourceContext &context, const complex<double> &value)
    {
        return new ComplexValue(context, 3);
    };
    const ChainUnaryOperator *mapper = (new UnaryErrorOperator())->mapComplex(f);

    const SourceContext context("1", "1", 1, 0);
    const Value *val = mapper->apply(context, ComplexValue(context, 1));

    EXPECT_EQ("3", to_string(*val));

    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, IntValue(context,1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument integer", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, MatrixValue(context,PLUS_KET));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument matrix", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ListValue(context,{new IntValue(context,1),new IntValue(context,1)}));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument list", ex.what());
            throw;
         } }, QuExecException);
    delete mapper;
}

TEST(testOperators, testMatrixChain)
{
    MatrixMapperFunction f = [](const SourceContext &context, const Matrix &value)
    {
        return new IntValue(context, 3);
    };
    ChainUnaryOperator *mapper = (new UnaryErrorOperator())->mapMatrix(f);

    const SourceContext context("1", "1", 1, 0);
    const Value *val = mapper->apply(context, MatrixValue(context, PLUS_KET));

    EXPECT_EQ("3", to_string(*val));

    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, IntValue(context,1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument integer", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ComplexValue(context,1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument complex", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ListValue(context,{new IntValue(context,1),new IntValue(context,1)}));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected argument list", ex.what());
            throw;
         } }, QuExecException);
    delete mapper;
}

TEST(testOperators, testBinaryError)
{
    BinaryErrorOperator mapper;
    const SourceContext context("1", "1", 1, 0);
    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, IntValue(context,1), IntValue(context,2));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected arguments integer, integer", ex.what());
            throw;
         } }, QuExecException);
}

TEST(testOperators, testBinaryMatrixMatrix)
{
    MatrixMatrixMapperFunction f = [](const SourceContext &context, const Matrix &left, const Matrix &right)
    {
        return new MatrixValue(context, PLUS_KET);
    };
    ChainBinaryOperator *op = (new BinaryErrorOperator())
                                  ->mapMatrixMatrix(f);
    const SourceContext context("1", "1", 1, 0);
    const Value *value = op->apply(context, MatrixValue(context, I_KET), MatrixValue(context, I_KET));
    EXPECT_EQ(ValueType::matrixValueType, value->type());
    EXPECT_EQ(to_string(PLUS_KET), to_string(((const MatrixValue *)value)->value()));

    EXPECT_THROW({ 
        try
         {
        op->apply(context, IntValue(context,1), IntValue(context,2));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected arguments integer, integer", ex.what());
            throw;
         } }, QuExecException);
    delete op;
}

TEST(testOperators, testBinaryIntInt)
{
    IntIntMapperFunction f = [](const SourceContext &context, const int left, const int right)
    {
        return new MatrixValue(context, PLUS_KET);
    };
    ChainBinaryOperator *op = (new BinaryErrorOperator())
                                  ->mapIntInt(f);
    const SourceContext context("1", "1", 1, 0);
    const Value *value = op->apply(context, IntValue(context, 1), IntValue(context, 2));
    EXPECT_EQ(ValueType::matrixValueType, value->type());
    EXPECT_EQ(to_string(PLUS_KET), to_string(((const MatrixValue *)value)->value()));

    EXPECT_THROW({ 
        try
         {
        op->apply(context, ComplexValue(context, 1), ComplexValue(context, 2));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected arguments complex, complex", ex.what());
            throw;
         } }, QuExecException);
    delete op;
}
