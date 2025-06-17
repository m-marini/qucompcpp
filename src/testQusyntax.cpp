#include <gtest/gtest.h>

#include "qusyntax.h"
#include "qumocks.h"

using namespace std;
using namespace qc;
using namespace qm;

MockParseContext::~MockParseContext()
{
    for (auto &p : _joins)
    {
        delete p.first;
    }
}

void MockParseContext::join(const Token &token, const SyntaxRule &rule)
{
    Token *t = token.clone();
    _joins.push_back({t, &rule});
}

//------------------------------

class SyntaxFixture : public testing::TestWithParam<std::vector<std::string>>
{
};

TEST_P(SyntaxFixture, testSyntaxRule)
{
    const vector<string> params = GetParam();
    const string ruleId = params.at(0);
    const string code = params.at(1);

    RuleMap ruleMap = Syntax::build();
    const map<const string, SyntaxRule *> rules = ruleMap.map();
    ASSERT_EQ(1, rules.count(ruleId));

    SyntaxRule *rule = rules.at(ruleId);
    ASSERT_EQ(ruleId, rule->id());

    stringstream stream(code);
    Tokenizer tokenizer(stream);
    tokenizer.open();

    MockParseContext ctx;
    ASSERT_TRUE(rule->parse(tokenizer, ctx));
    int expectedJoins = params.size() - 2;
    ASSERT_EQ(expectedJoins, ctx.joins().size());

    for (int i = 0; i < expectedJoins; i++)
    {
        const auto *rule = ctx.joins().at(i).second;
        const auto &expectedRule = params.at(i + 2);
        EXPECT_EQ(expectedRule, rule->id());
    }
}

//------------------------------

class SyntaxErrorFixture : public testing::TestWithParam<std::vector<std::string>>
{
};

TEST_P(SyntaxErrorFixture, testSyntaxRuleError)
{
    const vector<string> params = GetParam();
    const string ruleId = params.at(0);
    const string code = params.at(1);
    const string expectedMessage = params.at(2);

    RuleMap ruleMap = Syntax::build();
    const map<const string, SyntaxRule *> rules = ruleMap.map();
    ASSERT_EQ(1, rules.count(ruleId));

    SyntaxRule *rule = rules.at(ruleId);
    ASSERT_EQ(ruleId, rule->id());

    stringstream stream(code);
    Tokenizer tokenizer(stream);
    tokenizer.open();

    ASSERT_THROW(
        {
            try
            {
                MockParseContext ctx;
                rule->parse(tokenizer, ctx);
            }
            catch (QuParseException ex)
            {
                EXPECT_STREQ(expectedMessage.c_str(), ex.what());
                throw;
            }
        },
        QuParseException);
}

INSTANTIATE_TEST_SUITE_P(testSyntax,
                         SyntaxFixture,
                         testing::Values(
                             // ruleId, code, ... expectedRules
                             vector<string>{"pi", "pi", "pi"},
                             vector<string>{"i", "i", "i"},
                             vector<string>{"<int-literal>", "1234", "<int-literal>"},
                             vector<string>{"<real-literal>", "1.234", "<real-literal>"},
                             vector<string>{"<im-unit>", "i", "i", "<im-unit>"},
                             // 5
                             vector<string>{"<im-state>", "i", "i", "<im-state>"},
                             vector<string>{"<minus-im-state>", "i", "i", "<minus-im-state>"},
                             vector<string>{"<plus-state>", "+", "+", "<plus-state>"},
                             vector<string>{"<minus-state-exp-opt>", "", "<minus-state>", "<minus-state-exp-opt>"},
                             vector<string>{"<minus-state-exp-opt>", "i", "i", "<minus-im-state>", "<minus-state-exp-opt>"},
                             // 10
                             vector<string>{"<state-exp-opt>", "i", "i", "<im-state>", "<state-exp-opt>"},
                             vector<string>{"<state-exp-opt>", "-", "-", "<minus-state>", "<minus-state-exp-opt>", "<minus-state-exp>", "<state-exp-opt>"},
                             vector<string>{"<state-exp-opt>", "-i", "-", "i", "<minus-im-state>", "<minus-state-exp-opt>", "<minus-state-exp>", "<state-exp-opt>"},
                             vector<string>{"<state-exp-opt>", "+", "+", "<plus-state>", "<state-exp-opt>"},
                             vector<string>{"<state-exp-opt>", "0",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-exp>", "<add-exp>", "<exp-opt>",
                                            "<int-state>", "<state-exp-opt>"},
                             // 15
                             vector<string>{"<function>", "sqrt()",
                                            "<function-id>", "(", "<empty-arg>", "<arg-list>", ")", "<args-exp>", "<function>"},

                             vector<string>{"<function>", "sqrt(1)",
                                            "<function-id>", "(",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-exp>", "<add-exp>", "<exp-opt>",
                                            "<arg>", "<arg-list-opt>", "<arg-list>", ")", "<args-exp>", "<function>"},
                             vector<string>{"<function>", "sqrt(1,2)",
                                            "<function-id>", "(",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-exp>", "<add-exp>", "<exp-opt>", "<arg>", ",",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-exp>", "<add-exp>", "<exp-opt>",
                                            "<exp>", "<arg-tail>", "<arg-list-opt>", "<arg-list>", ")", "<args-exp>", "<function>"},
                             vector<string>{"<var-identifier>", "a", "<var-identifier>"},
                             vector<string>{"<assign-var-identifier>", "a", "<assign-var-identifier>"},
                             // 20
                             vector<string>{"<primary-exp>", "1", "<int-literal>", "<primary-exp>"},
                             vector<string>{"<primary-exp>", "1.2", "<real-literal>", "<primary-exp>"},
                             vector<string>{"<primary-exp>", "pi", "pi", "<primary-exp>"},
                             vector<string>{"<primary-exp>", "e", "e", "<primary-exp>"},
                             vector<string>{"<primary-exp>", "i", "i", "<im-unit>", "<primary-exp>"},
                             // 25
                             vector<string>{"<primary-exp>", "(1)", "(",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-exp>", "<add-exp>", "<exp-opt>", "<exp>",
                                            ")", "<priority-exp>", "<primary-exp>"},
                             vector<string>{"<primary-exp>", "a", "<var-identifier>", "<primary-exp>"},
                             vector<string>{"<primary-exp>", "<+|", "<", "+", "<plus-state>", "<state-exp-opt>",
                                            "<state-exp>", "|", "<bra>", "<primary-exp>"},
                             vector<string>{"<primary-exp>", "|+>", "|", "+", "<plus-state>", "<state-exp-opt>",
                                            "<state-exp>", ">", "<ket>", "<primary-exp>"},
                             vector<string>{"<conj>", "1", "<int-literal>", "<primary-exp>", "<conj>"},
                             // 30
                             vector<string>{"<conj>", "1^", "<int-literal>", "<primary-exp>", "^", "<conj>"},
                             vector<string>{"<conj>", "1^^", "<int-literal>", "<primary-exp>", "^", "^", "<conj>"},
                             vector<string>{"<unary-exp>", "1", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>"},
                             vector<string>{"<unary-exp>", "+1", "+", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>",
                                            "<plus-exp>", "<unary-exp>"},
                             vector<string>{"<unary-exp>", "-1", "-", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>",
                                            "<negate-exp>", "<unary-exp>"},
                             // 35
                             vector<string>{"<unary-exp>", "-+1", "-", "+", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>",
                                            "<plus-exp>", "<unary-exp>", "<negate-exp>", "<unary-exp>"},
                             vector<string>{"<cross-exp>", "1", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>"},
                             vector<string>{"<cross-exp>", "1 x 2", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>",
                                            "x", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-tail-opt>",
                                            "<cross-exp>"},
                             vector<string>{"<cross-exp>", "1 x 2 x 3", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>",
                                            "x", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-tail-opt>",
                                            "x", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-tail-opt>",
                                            "<cross-exp>"},
                             vector<string>{"<multiply-exp>", "1", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>"},
                             // 40
                             vector<string>{"<multiply-exp>", "1 . 2",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            ".", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-tail>", "<mul-tail-opt>",
                                            "<multiply-exp>"},
                             vector<string>{"<multiply-exp>", "1 * 2",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "*", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-tail-star>", "<mul-tail-opt>",
                                            "<multiply-exp>"},
                             vector<string>{"<multiply-exp>", "1 / 2",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "/", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<divide-tail>", "<mul-tail-opt>",
                                            "<multiply-exp>"},
                             vector<string>{"<multiply-exp>", "1 * 2 / 3",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "*", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-tail-star>", "<mul-tail-opt>",
                                            "/", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<divide-tail>", "<mul-tail-opt>",
                                            "<multiply-exp>"},
                             vector<string>{"<add-exp>", "1", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>", "<add-exp>"},
                             vector<string>{"<add-exp>", "1+2",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>",
                                            "+", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>",
                                            "<plus-tail>", "<add-tail-opt>",
                                            "<add-exp>"},
                             // 45
                             vector<string>{"<add-exp>", "1-2",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>",
                                            "-", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>",
                                            "<minus-tail>", "<add-tail-opt>",
                                            "<add-exp>"},
                             vector<string>{"<add-exp>", "1+2-3",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>",
                                            "+", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>",
                                            "<plus-tail>", "<add-tail-opt>",
                                            "-", "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>", "<multiply-exp>",
                                            "<minus-tail>", "<add-tail-opt>",
                                            "<add-exp>"},
                             vector<string>{"<exp>", "1",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-exp>", "<add-exp>", "<exp-opt>", "<exp>"},
                             vector<string>{"<code-unit>", "clear();1;",
                                            "<code-unit-head>", "clear", "(", ")", "<clear-stm>", "<stm-opt>", ";", "<stm>",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-exp>", "<add-exp>", "<exp-opt>", "<stm-opt>", ";", "<stm>", "<eof>", "<code-unit>"},
                             vector<string>{"<code-unit>", "let a = 1;",
                                            "<code-unit-head>", "let", "<assign-var-identifier>", "=",
                                            "<int-literal>", "<primary-exp>", "<conj>", "<unary-exp>", "<cross-exp>",
                                            "<multiply-exp>", "<add-exp>", "<exp-opt>", "<exp>",
                                            "<assign-stm>", "<stm-opt>", ";", "<stm>", "<eof>", "<code-unit>"},
                             vector<string>{"e", "e", "e"}));

INSTANTIATE_TEST_SUITE_P(testSyntaxError,
                         SyntaxErrorFixture,
                         testing::Values(
                             // ruleId, code, expectedError
                             vector<string>{"<bra>", "<", "Missing <exp-opt>"},
                             vector<string>{"<bra>", "<0", "Missing |"},
                             vector<string>{"<ket>", "|", "Missing <exp-opt>"},
                             vector<string>{"<ket>", "|0", "Missing >"},
                             vector<string>{"<priority-exp>", "(", "Missing <exp-opt>"},
                             // 5
                             vector<string>{"<priority-exp>", "(0", "Missing )"},
                             vector<string>{"<function>", "sqrt", "Missing ("},
                             vector<string>{"<function>", "sqrt(", "Missing )"},
                             vector<string>{"<function>", "sqrt(0", "Missing )"},
                             vector<string>{"<function>", "sqrt(0,", "Missing <exp-opt>"},
                             // 10
                             vector<string>{"<function>", "sqrt(0,0", "Missing )"},
                             vector<string>{"<state-exp>", "", "Missing <exp-opt>"}));
