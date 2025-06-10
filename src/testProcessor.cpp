#include <gtest/gtest.h>

#include "processor.h"
#include "values.h"
#include "matrix.h"
#include "tokenizer.h"
#include "compiler.h"
#include "qusyntax.h"

using namespace std;
using namespace qc;
using namespace mx;

class MockValueMapper : public ValueMapper
{
public:
    virtual const Value *map(const IntValue &value) const override
    {
        return new IntValue(value.value() * 2);
    }
    virtual const Value *map(const ComplexValue &value) const override
    {
        return new ComplexValue(value.value() * 2.0);
    }
    virtual const Value *map(const MatrixValue &value) const override
    {
        return new IntValue(value.value().numRows());
    }
    virtual const Value *map(const ListValue &value) const override
    {
        return new IntValue(value.values().size());
    }
};

TEST(testProcessor, testIntValue)
{
    const IntValue x(1);
    EXPECT_EQ(1, x.value());
    EXPECT_EQ("1", to_string(x));

    MockValueMapper mapper;
    const Value *mappedValue = x.map(MockValueMapper());

    EXPECT_EQ(2, ((IntValue *)mappedValue)->value());

    delete mappedValue;
}

TEST(testProcessor, testComplexValue)
{
    const ComplexValue x(1.1);
    EXPECT_EQ(complex(1.1), x.value());
    EXPECT_EQ("(1.1,0)", to_string(x));

    MockValueMapper mapper;
    const Value *mappedValue = x.map(MockValueMapper());

    EXPECT_EQ(complex(2.2), ((ComplexValue *)mappedValue)->value());

    delete mappedValue;
}

TEST(testProcessor, testMatrixValue)
{
    const MatrixValue x(PLUS_KET);
    EXPECT_EQ(PLUS_KET.cells(), x.value().cells());
    EXPECT_EQ("(0.707107,0)\n(0.707107,0)", to_string(x));

    MockValueMapper mapper;
    const Value *mappedValue = x.map(MockValueMapper());

    EXPECT_EQ(2, ((IntValue *)mappedValue)->value());

    delete mappedValue;
}

TEST(testProcessor, testListValue)
{
    const ListValue x({new IntValue(2), new ComplexValue(1.1)});
    EXPECT_EQ("(2,(1.1,0))", to_string(x));
    EXPECT_EQ(2, x.values().size());

    MockValueMapper mapper;
    const Value *mappedValue = x.map(MockValueMapper());

    EXPECT_EQ(2, ((IntValue *)mappedValue)->value());

    delete mappedValue;
}

class ProcessorFixture : public testing::TestWithParam<std::pair<std::string, Value *>>
{
};

class ProcessorErrorFixture : public testing::TestWithParam<std::pair<std::string, std::string>>
{
};

TEST_P(ProcessorFixture, testProcessor)
{
    auto &params = GetParam();
    const string &code = params.first;
    const Value &expected = *params.second;
    stringstream stream(code);
    Tokenizer tokenizer(stream);
    tokenizer.open();
    Compiler &compiler = *Compiler::createQu(tokenizer);
    RuleMap rules = Syntax::build();
    const SyntaxRule *rule = rules.map().at("<code-unit>");

    ASSERT_EQ("<code-unit>", rule->id());
    rule->parse(tokenizer, compiler);
    ASSERT_EQ(1, compiler.stack().size());
    const NodeCommand *cmd = compiler.popCommand();
    ASSERT_TRUE(cmd != NULL);

    Processor processor;

    const Value *result = cmd->eval(processor);
    ASSERT_TRUE(result != NULL);
    EXPECT_EQ(to_string(expected), to_string(*result));

    delete result;
    delete cmd;
    delete &expected;
    delete &compiler;
}

TEST_P(ProcessorErrorFixture, testProcessorError)
{
    auto &params = GetParam();
    const string &code = params.first;
    const string &expected = params.second;
    stringstream stream(code);
    Tokenizer tokenizer(stream);
    tokenizer.open();
    Compiler &compiler = *Compiler::createQu(tokenizer);
    RuleMap rules = Syntax::build();
    const SyntaxRule *rule = rules.map().at("<code-unit>");

    ASSERT_EQ("<code-unit>", rule->id());
    rule->parse(tokenizer, compiler);
    ASSERT_EQ(1, compiler.stack().size());
    const NodeCommand *cmd = compiler.popCommand();
    ASSERT_TRUE(cmd != NULL);

    Processor processor;

    EXPECT_THROW({
        try {
            cmd->eval(processor); 
        }
        catch (QuExecException ex)
        {
            EXPECT_STREQ(expected.c_str(), ex.what());
            throw;
        } }, QuExecException);

    delete &compiler;
}

INSTANTIATE_TEST_SUITE_P(testProcessorError,
                         ProcessorErrorFixture,
                         testing::Values(
                             // Code, expected error
                             pair<string, string>{"||+>>;", "Expected integer value: ((0.707107,0)\n(0.707107,0))"},
                             pair<string, string>{"|1.0>;", "Expected integer value: (1,0)"}));

INSTANTIATE_TEST_SUITE_P(testProcessor,
                         ProcessorFixture,
                         testing::Values(
                             // Code, expected result
                             pair<string, Value *>{"|3>;", new ListValue({new MatrixValue(Matrix(4, 1, {0, 0, 0, 1}))})},
                             pair<string, Value *>{"|0>;", new ListValue({new MatrixValue(Matrix(2, 1, {1, 0}))})},
                             pair<string, Value *>{"|+>;", new ListValue({new MatrixValue(PLUS_KET)})},
                             pair<string, Value *>{"1.2;", new ListValue({new ComplexValue(1.2)})},
                             pair<string, Value *>{"1;", new ListValue({new IntValue(1)})}));
