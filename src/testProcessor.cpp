#include <gtest/gtest.h>

#include "processor.h"
#include "values.h"
#include "matrix.h"
#include "tokenizer.h"
#include "compiler.h"
#include "qusyntax.h"
#include "syntaxRules.h"

using namespace std;
using namespace qc;
using namespace mx;

TEST(testProcessor, testIntValue)
{
    const IntValue x(1);
    EXPECT_EQ(1, x.value());
    EXPECT_EQ("1", to_string(x));
}

TEST(testProcessor, testComplexValue)
{
    const ComplexValue x(1.1);
    EXPECT_EQ(complex(1.1), x.value());
    EXPECT_EQ("(1.1,0)", to_string(x));
}

TEST(testProcessor, testMatrixValue)
{
    const MatrixValue x(PLUS_KET);
    EXPECT_EQ(PLUS_KET.cells(), x.value().cells());
    EXPECT_EQ("(0.707107,0)\n(0.707107,0)", to_string(x));
}

TEST(testProcessor, testListValue)
{
    const ListValue x({new IntValue(2), new ComplexValue(1.1)});
    EXPECT_EQ("(2,(1.1,0))", to_string(x));
    EXPECT_EQ(2, x.values().size());
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
                             pair<string, string>{"sqrt(|0>);", "Unexpected value (1,0)\n(0,0)"},
                             pair<string, string>{"a;", "Undefined variable a"},
                             pair<string, string>{"||+>>;", "Expected integer value, actual: (0.707107,0)\n(0.707107,0)"},
                             pair<string, string>{"|1.0>;", "Expected integer value, actual: (1,0)"}));

static const Matrix KET0(2, 1, {1, 0});
static const Matrix KET3(4, 1, {0, 0, 0, 1});

INSTANTIATE_TEST_SUITE_P(testProcessor,
                         ProcessorFixture,
                         testing::Values(
                             // Code, expected result
                             pair<string, Value *>{"sqrt(4);", new ListValue({new ComplexValue(2)})},
                             pair<string, Value *>{"sqrt(4.0);", new ListValue({new ComplexValue(2)})},
                             pair<string, Value *>{"let a = 1;a;", new ListValue({new IntValue(1), new IntValue(1)})},
                             pair<string, Value *>{"let a = i;a;", new ListValue({new ComplexValue(1i), new ComplexValue(1i)})},
                             pair<string, Value *>{"let a = |0>;a;", new ListValue({new MatrixValue(KET0), new MatrixValue(KET0)})},
                             pair<string, Value *>{"let a = 1;", new ListValue({new IntValue(1)})},
                             pair<string, Value *>{"let a = i;", new ListValue({new ComplexValue(1i)})},
                             pair<string, Value *>{"let a = |0>;", new ListValue({new MatrixValue(KET0)})},
                             pair<string, Value *>{"-1;", new ListValue({new IntValue(-1)})},
                             pair<string, Value *>{"-i;", new ListValue({new ComplexValue(-1i)})},
                             pair<string, Value *>{"-|0>;", new ListValue({new MatrixValue(-KET0)})},
                             pair<string, Value *>{"1^;", new ListValue({new IntValue(1)})},
                             pair<string, Value *>{"i^;", new ListValue({new ComplexValue({0, -1})})},
                             pair<string, Value *>{"<0|;", new ListValue({new MatrixValue(Matrix(KET0.dagger()))})},
                             pair<string, Value *>{"|3>;", new ListValue({new MatrixValue(KET3)})},
                             pair<string, Value *>{"|0>;", new ListValue({new MatrixValue(KET0)})},
                             pair<string, Value *>{"|+>;", new ListValue({new MatrixValue(PLUS_KET)})},
                             pair<string, Value *>{"1.2;", new ListValue({new ComplexValue(1.2)})},
                             pair<string, Value *>{"1;", new ListValue({new IntValue(1)})}));
