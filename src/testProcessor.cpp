#include <gtest/gtest.h>

#include <iostream>

#include "processor.h"
#include "values.h"
#include "matrix.h"
#include "tokenizer.h"
#include "compiler.h"
#include "qusyntax.h"
#include "syntaxRules.h"

#define HALF_SQRT2 (sqrt(2) / 2)

using namespace std;
using namespace qc;
using namespace mx;

static const SourceContext SOURCE("1", "1", 1, 0);

TEST(testProcessor, testIntValue)
{
    const IntValue x(SOURCE, 1);
    EXPECT_EQ(1, x.value());
    EXPECT_EQ("1", to_string(x));
}

TEST(testProcessor, testComplexValue)
{
    const ComplexValue x(SOURCE, 1.1);
    EXPECT_EQ(complex(1.1), x.value());
    EXPECT_EQ("1.1", to_string(x));
}

TEST(testProcessor, testMatrixValue)
{
    const MatrixValue x(SOURCE, PLUS_KET);
    EXPECT_EQ(PLUS_KET.cells(), x.value().cells());
    EXPECT_EQ("(0.7071067811865476) |0> + (0.7071067811865476) |1>", to_string(x));
}

TEST(testProcessor, testListValue)
{
    const ListValue x(SOURCE, {new IntValue(SOURCE, 2), new ComplexValue(SOURCE, 1.1)});
    EXPECT_EQ("(2,1.1)", to_string(x));
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

    EXPECT_NO_THROW(
        try {
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
        } catch (QuSourceException ex) {
            cerr << ex << endl;
            throw;
        });
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
        catch (QuSourceException ex)
        {
            EXPECT_STREQ(expected.c_str(), ex.what());
            throw;
        } }, QuSourceException);

    delete &compiler;
}

INSTANTIATE_TEST_SUITE_P(testProcessorError,
                         ProcessorErrorFixture,
                         testing::Values(
                             // Code, expected error
                             pair<string, string>{"|0> * |3>;", "Invalid matrix multiplication 4x1 by 4x1"},
                             pair<string, string>{"1 + |0>;", "Unexpected arguments integer, matrix"},
                             pair<string, string>{"i + |0>;", "Unexpected arguments complex, matrix"},
                             pair<string, string>{"|0> + 1;", "Unexpected arguments matrix, integer"},
                             pair<string, string>{"|0> + i;", "Unexpected arguments matrix, complex"},
                             // 5
                             pair<string, string>{"1 - |0>;", "Unexpected arguments integer, matrix"},
                             pair<string, string>{"i - |0>;", "Unexpected arguments complex, matrix"},
                             pair<string, string>{"|0> - 1;", "Unexpected arguments matrix, integer"},
                             pair<string, string>{"|0> - i;", "Unexpected arguments matrix, complex"},
                             pair<string, string>{"1 / |0>;", "Unexpected arguments integer, matrix"},
                             // 10
                             pair<string, string>{"1.0 / |0>;", "Unexpected arguments complex, matrix"},
                             pair<string, string>{"0 x 0;", "Unexpected arguments integer, integer"},
                             pair<string, string>{"X(i);", "Unexpected argument complex"},
                             pair<string, string>{"X(|0>);", "Unexpected argument matrix"},
                             pair<string, string>{"I(i);", "Unexpected argument complex"},
                             // 15
                             pair<string, string>{"I(|0>);", "Unexpected argument matrix"},
                             pair<string, string>{"sqrt(|0>);", "Unexpected argument matrix"},
                             pair<string, string>{"a;", "Undefined variable a"},
                             pair<string, string>{"||+>>;", "Unexpected argument matrix"},
                             pair<string, string>{"<0| * <3|;", "Invalid matrix multiplication 1x2 by 1x4"},
                             // 20
                             pair<string, string>{"CNOT(0,0);", "Expected all different indices [0, 0]"},
                             pair<string, string>{"SWAP(1,1);", "Expected all different indices [1, 1]"},
                             pair<string, string>{"CCNOT(0,0,1);", "Expected all different indices [0, 0, 1]"},
                             pair<string, string>{"CCNOT(0,1,0);", "Expected all different indices [0, 1, 0]"},
                             pair<string, string>{"CCNOT(0,1,1);", "Expected all different indices [0, 1, 1]"},
                             pair<string, string>{"|1.0>;", "Unexpected argument complex"}));

static const Matrix KET0(2, 1, {1, 0});
static const Matrix KET3(4, 1, {0, 0, 0, 1});
static const Matrix I0(2, 2, {1, 0, 0, 1});
static const Matrix X0(2, 2,
                       {0, 1,
                        1, 0});
static const Matrix X00(4, 1, {1, 0, 0, 0});
static const Matrix X1(4, 4,
                       {0, 0, 1, 0,
                        0, 0, 0, 1,
                        1, 0, 0, 0,
                        0, 1, 0, 0});

static const Matrix ARY02(2, 4,
                          {0, 0, 1, 0,
                           0, 0, 0, 0});
static const Matrix MX21BY2(2, 1,
                            {2, 0});
static const Matrix MX21DIV2(2, 1,
                             {0.5, 0});
static const Matrix MX0PLUS3(4, 1,
                             {1, 0, 0, 1});
static const Matrix MX42_K0PB3(4, 2,
                               {1, 0,
                                0, 0,
                                0, 0,
                                1, 0});
static const Matrix MX24_B0PK3(4, 2,
                               {1, 0,
                                0, 0,
                                0, 0,
                                1, 0});
static const Matrix MX21_0M0(2, 1,
                             {0,
                              0});
static const Matrix MX41_0M3(4, 1,
                             {1,
                              0,
                              0,
                              -1});
static const Matrix MX41_3M0(4, 1,
                             {-1,
                              0,
                              0,
                              1});
static const Matrix ZERO22(2, 2,
                           {0, 0,
                            0, 0});
static const Matrix ZERO44(4, 4,
                           {0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 0});

static const Matrix EPS01(2, 2,
                          {0, -1,
                           1, 0});

static const Matrix M22_SIM1(2, 2,
                             {1, 0,
                              0, -1});

static const Matrix Y0(2, 2,
                       {0, {0, -1}, 1i, 0});

static const Matrix Y1(4, 4,
                       {0, 0, {0, -1}, 0, 0, 0, 0, {0, -1}, 1i, 0, 0, 0, 0, 1i, 0, 0});

static const Matrix Z0(2, 2,
                       {1, 0,
                        0, -1});

static const Matrix Z1(4, 4,
                       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, -1, 0,
                        0, 0, 0, -1});

static const Matrix H0(2, 2,
                       {HALF_SQRT2, HALF_SQRT2,
                        HALF_SQRT2, -HALF_SQRT2});

static const Matrix H1(4, 4,
                       {HALF_SQRT2, 0, HALF_SQRT2, 0,
                        0, HALF_SQRT2, 0, HALF_SQRT2,
                        HALF_SQRT2, 0, -HALF_SQRT2, 0,
                        0, HALF_SQRT2, 0, -HALF_SQRT2});

static const Matrix S0(2, 2,
                       {1, 0,
                        0, 1i});

static const Matrix S1(4, 4,
                       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1i, 0,
                        0, 0, 0, 1i});

static const Matrix T0(2, 2,
                       {1, 0, 0, {HALF_SQRT2, HALF_SQRT2}});

static const Matrix T1(4, 4,
                       {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, {HALF_SQRT2, HALF_SQRT2}, 0, 0, 0, 0, {HALF_SQRT2, HALF_SQRT2}});

INSTANTIATE_TEST_SUITE_P(testProcessor,
                         ProcessorFixture,
                         testing::Values(
                             // Code, expected result
                             pair<string, Value *>{"1 - 2;", new ListValue(SOURCE, {new IntValue(SOURCE, -1)})},
                             pair<string, Value *>{"1 - i;", new ListValue(SOURCE, {new ComplexValue(SOURCE, {1, -1})})},
                             pair<string, Value *>{"i - 1;", new ListValue(SOURCE, {new ComplexValue(SOURCE, {-1, 1})})},
                             pair<string, Value *>{"i - i;", new ListValue(SOURCE, {new ComplexValue(SOURCE, {0, 0})})},
                             pair<string, Value *>{"1 + 2;", new ListValue(SOURCE, {new IntValue(SOURCE, 3)})},
                             // 5
                             pair<string, Value *>{"1 + i;", new ListValue(SOURCE, {new ComplexValue(SOURCE, {1, 1})})},
                             pair<string, Value *>{"i + 1;", new ListValue(SOURCE, {new ComplexValue(SOURCE, {1, 1})})},
                             pair<string, Value *>{"i + i;", new ListValue(SOURCE, {new ComplexValue(SOURCE, {0, 2})})},
                             pair<string, Value *>{"|0> / 2;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21DIV2)})},
                             pair<string, Value *>{"|0> / 2;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21DIV2)})},
                             // 10
                             pair<string, Value *>{"|0> / 2.0;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21DIV2)})},
                             pair<string, Value *>{"|0> . 2;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21BY2)})},
                             pair<string, Value *>{"|0> . 2.0;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21BY2)})},
                             pair<string, Value *>{"|0> * 2;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21BY2)})},
                             pair<string, Value *>{"|0> * 2;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21BY2)})},
                             // 15
                             pair<string, Value *>{"2.0 * 3.0;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 6)})},
                             pair<string, Value *>{"2.0 . 3.0;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 6)})},
                             pair<string, Value *>{"6.0 / 2.0;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 3)})},
                             pair<string, Value *>{"2.0 * 3;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 6)})},
                             pair<string, Value *>{"2.0 . 3;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 6)})},
                             // 20
                             pair<string, Value *>{"6.0 / 2;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 3)})},
                             pair<string, Value *>{"2 * 3.0;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 6)})},
                             pair<string, Value *>{"2 . 3.0;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 6)})},
                             pair<string, Value *>{"6 / 2.0;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 3)})},
                             pair<string, Value *>{"2 * 3;", new ListValue(SOURCE, {new IntValue(SOURCE, 6)})},
                             // 25
                             pair<string, Value *>{"2 . 3;", new ListValue(SOURCE, {new IntValue(SOURCE, 6)})},
                             pair<string, Value *>{"6 / 2;", new ListValue(SOURCE, {new IntValue(SOURCE, 3)})},
                             pair<string, Value *>{"3 / 2;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 1.5)})},
                             pair<string, Value *>{"ary(0,2);", new ListValue(SOURCE, {new MatrixValue(SOURCE, ARY02)})},
                             pair<string, Value *>{"|0> x |0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, X00)})},
                             // 30
                             pair<string, Value *>{"I(0);", new ListValue(SOURCE, {new MatrixValue(SOURCE, I0)})},
                             pair<string, Value *>{"normalise(4);", new ListValue(SOURCE, {new IntValue(SOURCE, 1)})},
                             pair<string, Value *>{"normalise(2.0);", new ListValue(SOURCE, {new ComplexValue(SOURCE, 1)})},
                             pair<string, Value *>{"normalise(|0>);", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0)})},
                             pair<string, Value *>{"sqrt(4);", new ListValue(SOURCE, {new ComplexValue(SOURCE, 2)})},
                             // 35
                             pair<string, Value *>{"sqrt(4.0);", new ListValue(SOURCE, {new ComplexValue(SOURCE, 2)})},
                             pair<string, Value *>{"let a = 1;a;", new ListValue(SOURCE, {new IntValue(SOURCE, 1), new IntValue(SOURCE, 1)})},
                             pair<string, Value *>{"let a = i;a;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 1i), new ComplexValue(SOURCE, 1i)})},
                             pair<string, Value *>{"let a = |0>;a;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0), new MatrixValue(SOURCE, KET0)})},
                             pair<string, Value *>{"let a = 1;", new ListValue(SOURCE, {new IntValue(SOURCE, 1)})},
                             // 40
                             pair<string, Value *>{"let a = i;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 1i)})},
                             pair<string, Value *>{"let a = |0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0)})},
                             pair<string, Value *>{"-1;", new ListValue(SOURCE, {new IntValue(SOURCE, -1)})},
                             pair<string, Value *>{"-i;", new ListValue(SOURCE, {new ComplexValue(SOURCE, -1i)})},
                             pair<string, Value *>{"-|0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, -KET0)})},
                             // 45
                             pair<string, Value *>{"1^;", new ListValue(SOURCE, {new IntValue(SOURCE, 1)})},
                             pair<string, Value *>{"i^;", new ListValue(SOURCE, {new ComplexValue(SOURCE, {0, -1})})},
                             pair<string, Value *>{"<0|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0.dagger())})},
                             pair<string, Value *>{"|3>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET3)})},
                             pair<string, Value *>{"|0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0)})},
                             // 50
                             pair<string, Value *>{"|0> - |0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21_0M0)})},
                             pair<string, Value *>{"|0> - |3>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX41_0M3)})},
                             pair<string, Value *>{"|3> - |0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX41_3M0)})},
                             pair<string, Value *>{"<0| - <0|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0.dagger() - KET0.dagger())})},
                             pair<string, Value *>{"<0| - <3|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0.dagger() - KET3.dagger())})},
                             // 55
                             pair<string, Value *>{"<3| - <0|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET3.dagger() - KET0.dagger())})},
                             pair<string, Value *>{"<0| - |3>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0.dagger() - KET3)})},
                             pair<string, Value *>{"|0> - <3|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0 - KET3.dagger())})},
                             pair<string, Value *>{"|0> + |0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX21BY2)})},
                             pair<string, Value *>{"|0> + |3>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX0PLUS3)})},
                             // 60
                             pair<string, Value *>{"sim(0,1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, X0)})},
                             pair<string, Value *>{"eps(0,0);", new ListValue(SOURCE, {new MatrixValue(SOURCE, ZERO22)})},
                             pair<string, Value *>{"eps(0,1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, EPS01)})},
                             pair<string, Value *>{"|3> + |0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, MX0PLUS3)})},
                             pair<string, Value *>{"<0| + <0|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0.dagger() + KET0.dagger())})},
                             // 65
                             pair<string, Value *>{"<0| + <3|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0.dagger() + KET3.dagger())})},
                             pair<string, Value *>{"<3| + <0|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET3.dagger() + KET0.dagger())})},
                             pair<string, Value *>{"<0| + |3>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0.dagger() + KET3)})},
                             pair<string, Value *>{"|0> + <3|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0 + KET3.dagger())})},
                             pair<string, Value *>{"|+>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, PLUS_KET)})},
                             // 70
                             pair<string, Value *>{"sim(0,1)*eps(0,1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, M22_SIM1)})},
                             pair<string, Value *>{"H(0);", new ListValue(SOURCE, {new MatrixValue(SOURCE, H0)})},
                             pair<string, Value *>{"H(1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, H1)})},
                             pair<string, Value *>{"S(0);", new ListValue(SOURCE, {new MatrixValue(SOURCE, S0)})},
                             pair<string, Value *>{"S(1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, S1)})},
                             // 75
                             pair<string, Value *>{"T(0);", new ListValue(SOURCE, {new MatrixValue(SOURCE, T0)})},
                             pair<string, Value *>{"T(1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, T1)})},
                             pair<string, Value *>{"X(0);", new ListValue(SOURCE, {new MatrixValue(SOURCE, X0)})},
                             pair<string, Value *>{"X(1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, X1)})},
                             pair<string, Value *>{"Y(0);", new ListValue(SOURCE, {new MatrixValue(SOURCE, Y0)})},
                             // 80
                             pair<string, Value *>{"Y(1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, Y1)})},
                             pair<string, Value *>{"Z(0);", new ListValue(SOURCE, {new MatrixValue(SOURCE, Z0)})},
                             pair<string, Value *>{"Z(1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, Z1)})},
                             pair<string, Value *>{"CNOT(1,2);", new ListValue(SOURCE, {new MatrixValue(SOURCE, mx::CNOT(1, 2))})},
                             pair<string, Value *>{"SWAP(1,2);", new ListValue(SOURCE, {new MatrixValue(SOURCE, mx::SWAP(1, 2))})},
                             // 85
                             pair<string, Value *>{"CCNOT(1,2,3);", new ListValue(SOURCE, {new MatrixValue(SOURCE, mx::CCNOT(1, 2, 3))})},
                             pair<string, Value *>{"qubit0(0,1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, mx::qubit0(0, 1))})},
                             pair<string, Value *>{"qubit1(0,1);", new ListValue(SOURCE, {new MatrixValue(SOURCE, mx::qubit1(0, 1))})},
                             pair<string, Value *>{"|0> . |0>;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0)})},
                             pair<string, Value *>{"<0| . <0|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, KET0.transpose())})},
                             // 90
                             pair<string, Value *>{"<0| * |0>;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 1)})},
                             pair<string, Value *>{"<0| . |0>;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 1)})},
                             pair<string, Value *>{"|1> . <0|;", new ListValue(SOURCE, {new MatrixValue(SOURCE, Matrix(2, 2, {0, 0, 1, 0}))})},
                             pair<string, Value *>{"1.2;", new ListValue(SOURCE, {new ComplexValue(SOURCE, 1.2)})},
                             pair<string, Value *>{"1;", new ListValue(SOURCE, {new IntValue(SOURCE, 1)})}));
