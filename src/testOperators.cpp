#include <gtest/gtest.h>

#include <functional>

#include "operators.h"
#include "sourceContext.h"

using namespace std;
using namespace qc;
using namespace mx;

TEST(testOperators, testError)
{
    UnaryErrorOperator mapper("Unexpected value: ");
    const SourceContext context("1", "1", 1, 0);
    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, IntValue(1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: 1", ex.what());
            throw;
         } }, QuExecException);

    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, ComplexValue(1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (1,0)", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, MatrixValue(PLUS_KET));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (0.707107,0)\n(0.707107,0)", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper.apply(context, ListValue({new IntValue(1),new IntValue(1)}));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (1,1)", ex.what());
            throw;
         } }, QuExecException);
}

TEST(testOperators, testIntChain)
{
    IntMapperFunction f = [](const SourceContext &context, const IntValue &value)
    {
        return new IntValue(3);
    };
    ChainUnaryOperator *mapper = (new UnaryErrorOperator("Unexpected value: "))->mapIntValue(f);

    const SourceContext context("1", "1", 1, 0);
    const Value *val = mapper->apply(context, IntValue(1));

    EXPECT_EQ("3", to_string(*val));

    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ComplexValue(1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (1,0)", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, MatrixValue(PLUS_KET));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (0.707107,0)\n(0.707107,0)", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ListValue({new IntValue(1),new IntValue(1)}));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (1,1)", ex.what());
            throw;
         } }, QuExecException);
    delete mapper;
}

TEST(testOperators, testComplexChain)
{
    ComplexMapperFunction f = [](const SourceContext &context, const complex<double> &value)
    {
        return new ComplexValue(3);
    };
    const ChainUnaryOperator *mapper = (new UnaryErrorOperator("Unexpected value: "))->mapComplexValue(f);

    const SourceContext context("1", "1", 1, 0);
    const Value *val = mapper->apply(context, ComplexValue(1));

    EXPECT_EQ("(3,0)", to_string(*val));

    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, IntValue(1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: 1", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, MatrixValue(PLUS_KET));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (0.707107,0)\n(0.707107,0)", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ListValue({new IntValue(1),new IntValue(1)}));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (1,1)", ex.what());
            throw;
         } }, QuExecException);
    delete mapper;
}

TEST(testOperators, testMatrixChain)
{
    MatrixMapperFunction f = [](const SourceContext &context, const Matrix &value)
    {
        return new IntValue(3);
    };
    ChainUnaryOperator *mapper = (new UnaryErrorOperator("Unexpected value: "))->mapMatrixValue(f);

    const SourceContext context("1", "1", 1, 0);
    const Value *val = mapper->apply(context, MatrixValue(PLUS_KET));

    EXPECT_EQ("3", to_string(*val));

    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, IntValue(1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: 1", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ComplexValue(1));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (1,0)", ex.what());
            throw;
         } }, QuExecException);
    EXPECT_THROW({ 
        try
         {
        mapper->apply(context, ListValue({new IntValue(1),new IntValue(1)}));
         } catch (QuExecException ex){
            EXPECT_STREQ("Unexpected value: (1,1)", ex.what());
            throw;
         } }, QuExecException);
    delete mapper;
}
