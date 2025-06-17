#include <gtest/gtest.h>

#include "compiler.h"
#include "tokenizer.h"
#include "syntaxBuilder.h"
#include "qusyntax.h"
#include "qumocks.h"

using namespace std;
using namespace qc;

TEST(testCompiler, testIntCommand)
{
    SourceContext ctx("token", "token line", 1, 0);
    IntCommand cmd(ctx, 1);
    EXPECT_EQ("1", to_string(cmd));
}

TEST(testCompiler, testComplexCommand)
{
    SourceContext ctx("token", "token line", 1, 0);
    ComplexCommand cmd(ctx, 1);
    EXPECT_EQ("(1,0)", to_string(cmd));
}

static const string deepString(const NodeCommand &cmd)
{
    return to_string(cmd);
}

class CompilerFixture : public testing::TestWithParam<std::vector<std::string>>
{
};

class CompilerErrorFixture : public testing::TestWithParam<std::vector<std::string>>
{
};

TEST_P(CompilerFixture, testCompiler)
{
    auto &params = GetParam();
    const string &code = params.at(0);
    const string expected = params.at(1);
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
    const string cmdStr = deepString(*cmd);
    ASSERT_EQ(expected, cmdStr);
    delete cmd;
    delete &compiler;
}

TEST_P(CompilerErrorFixture, testError)
{
    auto &params = GetParam();
    const string &code = params.at(0);
    const string expected = params.at(1);
    stringstream stream(code);
    Tokenizer tokenizer(stream);
    tokenizer.open();
    Compiler &compiler = *Compiler::createQu(tokenizer);
    RuleMap rules = Syntax::build();
    const SyntaxRule *rule = rules.map().at("<code-unit>");

    ASSERT_EQ("<code-unit>", rule->id());

    EXPECT_THROW({
        try {
            rule->parse(tokenizer, compiler);
        } catch(QuParseException ex){
            EXPECT_STREQ(expected.c_str(), ex.what());
            throw;
        } }, QuParseException);
    delete &compiler;
}

INSTANTIATE_TEST_SUITE_P(testCompiler,
                         CompilerFixture,
                         testing::Values(
                             // Code, expected command
                             vector<string>{"a.(b*c);", "list(mul(a,mulStar(b,c)))"},
                             vector<string>{"a.b*c;", "list(mulStar(mul(a,b),c))"},
                             vector<string>{"(a+b)*c;", "list(mulStar(add(a,b),c))"},
                             vector<string>{"a+b*c;", "list(add(a,mulStar(b,c)))"},
                             vector<string>{"a+(b-c);", "list(add(a,sub(b,c)))"},
                             vector<string>{"a+b-c;", "list(sub(add(a,b),c))"},
                             vector<string>{"qubit0(0,1);", "list(qubit0(0,1))"},
                             vector<string>{"qubit1(0,1);", "list(qubit1(0,1))"},
                             vector<string>{"normalise(0);", "list(normalise(0))"},
                             vector<string>{"SWAP(0,1);", "list(SWAP(0,1))"},
                             // 10
                             vector<string>{"CNOT(0,1);", "list(CNOT(0,1))"},
                             vector<string>{"CCNOT(0,1,2);", "list(CCNOT(0,1,2))"},
                             vector<string>{"I(0);", "list(I(0))"},
                             vector<string>{"H(0);", "list(H(0))"},
                             vector<string>{"X(0);", "list(X(0))"},
                             vector<string>{"Y(0);", "list(Y(0))"},
                             vector<string>{"Z(0);", "list(Z(0))"},
                             vector<string>{"S(0);", "list(S(0))"},
                             vector<string>{"T(0);", "list(T(0))"},
                             vector<string>{"eps(0,0);", "list(eps(0,0))"},
                             // 20
                             vector<string>{"sim(0,0);", "list(sim(0,0))"},
                             vector<string>{"ary(0,0);", "list(ary(0,0))"},
                             vector<string>{"sqrt(1);", "list(sqrt(1))"},
                             vector<string>{"clear();", "list(clear)"},
                             vector<string>{"let a=1;", "list(let(a,(1)))"},
                             vector<string>{"a;", "list(a)"},
                             vector<string>{"1 x 2;", "list(x(1,2))"},
                             vector<string>{"1 . 2;", "list(mul(1,2))"},
                             vector<string>{"1*2;", "list(mulStar(1,2))"},
                             vector<string>{"1/2;", "list(div(1,2))"},
                             // 30
                             vector<string>{"1+2;", "list(add(1,2))"},
                             vector<string>{"1-2;", "list(sub(1,2))"},
                             vector<string>{"-1;", "list(neg(1))"},
                             vector<string>{"i^;", "list(^((0,1)))"},
                             vector<string>{"<+|;", "list(^((0.7071067811865476) |0> + (0.7071067811865476) |1>))"},
                             vector<string>{"|i>;", "list((0.7071067811865476) |0> + (0.7071067811865476 i) |1>)"},
                             vector<string>{"|-i>;", "list((0.7071067811865476) |0> + (-0.7071067811865476 i) |1>)"},
                             vector<string>{"|+>;", "list((0.7071067811865476) |0> + (0.7071067811865476) |1>)"},
                             vector<string>{"|->;", "list((0.7071067811865476) |0> + (-0.7071067811865476) |1>)"},
                             vector<string>{"|7>;", "list(i2s(7))"},
                             vector<string>{"|0>;", "list(i2s(0))"},
                             vector<string>{"e;", "list((2.71828,0))"},
                             vector<string>{"pi;", "list((3.14159,0))"},
                             vector<string>{"i;", "list((0,1))"},
                             vector<string>{"1.2;", "list((1.2,0))"},
                             vector<string>{"1;2;", "list(1,2)"},
                             vector<string>{"1;", "list(1)"}));

INSTANTIATE_TEST_SUITE_P(testCompilerError,
                         CompilerErrorFixture,
                         testing::Values(
                             // Code, expected error
                             vector<string>{"qubit0(1);", "qubit0 requires 2 arguments: actual (1)"},
                             vector<string>{"qubit0(1,2,3);", "qubit0 requires 2 arguments: actual (3)"},
                             vector<string>{"qubit1(1);", "qubit1 requires 2 arguments: actual (1)"},
                             vector<string>{"qubit1(1,2,3);", "qubit1 requires 2 arguments: actual (3)"},
                             vector<string>{"normalise();", "normalise requires 1 arguments: actual (0)"},
                             vector<string>{"normalise(1,2);", "normalise requires 1 arguments: actual (2)"},
                             vector<string>{"SWAP(1);", "SWAP requires 2 arguments: actual (1)"},
                             vector<string>{"SWAP(1,2,3);", "SWAP requires 2 arguments: actual (3)"},
                             vector<string>{"CNOT(1);", "CNOT requires 2 arguments: actual (1)"},
                             vector<string>{"CNOT(1,2,3);", "CNOT requires 2 arguments: actual (3)"},
                             vector<string>{"CCNOT(1,2);", "CCNOT requires 3 arguments: actual (2)"},
                             vector<string>{"CCNOT(1,2,3,4);", "CCNOT requires 3 arguments: actual (4)"},
                             vector<string>{"I();", "I requires 1 arguments: actual (0)"},
                             vector<string>{"I(1,2);", "I requires 1 arguments: actual (2)"},
                             vector<string>{"H();", "H requires 1 arguments: actual (0)"},
                             vector<string>{"H(1,2);", "H requires 1 arguments: actual (2)"},
                             vector<string>{"X();", "X requires 1 arguments: actual (0)"},
                             vector<string>{"X(1,2);", "X requires 1 arguments: actual (2)"},
                             vector<string>{"Y();", "Y requires 1 arguments: actual (0)"},
                             vector<string>{"Y(1,2);", "Y requires 1 arguments: actual (2)"},
                             vector<string>{"Z();", "Z requires 1 arguments: actual (0)"},
                             vector<string>{"Z(1,2);", "Z requires 1 arguments: actual (2)"},
                             vector<string>{"S();", "S requires 1 arguments: actual (0)"},
                             vector<string>{"S(1,2);", "S requires 1 arguments: actual (2)"},
                             vector<string>{"T();", "T requires 1 arguments: actual (0)"},
                             vector<string>{"T(1,2);", "T requires 1 arguments: actual (2)"},
                             vector<string>{"eps(1);", "eps requires 2 arguments: actual (1)"},
                             vector<string>{"eps(1,2,3);", "eps requires 2 arguments: actual (3)"},
                             vector<string>{"sim(1);", "sim requires 2 arguments: actual (1)"},
                             vector<string>{"sim(1,2,3);", "sim requires 2 arguments: actual (3)"},
                             vector<string>{"ary();", "ary requires 2 arguments: actual (0)"},
                             vector<string>{"ary(1);", "ary requires 2 arguments: actual (1)"},
                             vector<string>{"ary(1,2,3);", "ary requires 2 arguments: actual (3)"},
                             vector<string>{"sqrt(1,2);", "sqrt requires 1 arguments: actual (2)"},
                             vector<string>{"sqrt();", "sqrt requires 1 arguments: actual (0)"}));
