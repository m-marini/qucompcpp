#include <gtest/gtest.h>

#include <utility>

#include "tokenizer.h"
#include "syntaxBuilder.h"
#include "qumocks.h"

using namespace std;
using namespace qc;
using namespace qm;

TEST(testSyntaxRule, EofRuleTrue)
{
  EofRule rule("<eof>");
  EXPECT_EQ("<eof>", rule.id());

  stringstream stream("");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  EXPECT_TRUE(rule.parse(tokenizer, ctx));
}

TEST(testSyntaxRule, EofRuleFalse)
{
  EofRule rule("<eof>");
  EXPECT_EQ("<eof>", rule.id());

  stringstream stream("1");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  EXPECT_FALSE(rule.parse(tokenizer, ctx));
}

TEST(testSyntaxRule, EmptyRule)
{
  EmptyRule rule("<empty>");
  EXPECT_EQ("<empty>", rule.id());

  stringstream stream("1");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  EXPECT_TRUE(rule.parse(tokenizer, ctx));
  EXPECT_EQ(1, ctx.joins().size());
  EXPECT_EQ("<empty>", ctx.joins()[0].second->id());
  EXPECT_EQ("1", ctx.joins()[0].first->token());
}

TEST(testSyntaxBuilder, EmptyRule)
{
  SyntaxBuilder builder;

  builder.empty("<empty>");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("<empty>"));
  EXPECT_EQ("<empty>", ruleMap.at("<empty>")->id());
}

TEST(testSyntaxBuilder, EOFRule)
{
  SyntaxBuilder builder;

  builder.eof("<eof>");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("<eof>"));
  EXPECT_EQ("<eof>", ruleMap.at("<eof>")->id());
}

TEST(testSyntaxBuilder, intLiteralTrue)
{
  SyntaxBuilder builder;

  builder.intLiteral("<int>");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("<int>"));
  const SyntaxRule *rule = ruleMap.at("<int>");
  EXPECT_EQ("<int>", rule->id());

  stringstream stream("1");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(1, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("1", tok0->token());
  EXPECT_EQ("<int>", rule0->id());
}

TEST(testSyntaxBuilder, intLiteralFalse)
{
  SyntaxBuilder builder;

  builder.intLiteral("<int>");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("<int>"));
  const SyntaxRule *rule = ruleMap.at("<int>");
  EXPECT_EQ("<int>", rule->id());

  stringstream stream("a");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, realLiteralTrue)
{
  SyntaxBuilder builder;

  builder.realLiteral("<rule>");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("<rule>"));
  const SyntaxRule *rule = ruleMap.at("<rule>");
  EXPECT_EQ("<rule>", rule->id());

  stringstream stream("1.2");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(1, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("1.2", tok0->token());
  EXPECT_EQ("<rule>", rule0->id());
}

TEST(testSyntaxBuilder, realLiteralFalse)
{
  SyntaxBuilder builder;

  builder.realLiteral("<rule>");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("<rule>"));
  const SyntaxRule *rule = ruleMap.at("<rule>");
  EXPECT_EQ("<rule>", rule->id());

  stringstream stream("a");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, operTrue)
{
  SyntaxBuilder builder;

  builder.oper("/");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("/"));
  const SyntaxRule *rule = ruleMap.at("/");
  EXPECT_EQ("/", rule->id());

  stringstream stream("/");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(1, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("/", tok0->token());
  EXPECT_EQ("/", rule0->id());
}

TEST(testSyntaxBuilder, operFalse)
{
  SyntaxBuilder builder;

  builder.oper("/");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("/"));
  const SyntaxRule *rule = ruleMap.at("/");
  EXPECT_EQ("/", rule->id());

  stringstream stream("a");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, idTrue)
{
  SyntaxBuilder builder;

  builder.id("a");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("a"));
  const SyntaxRule *rule = ruleMap.at("a");
  EXPECT_EQ("a", rule->id());

  stringstream stream("a");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(1, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("a", tok0->token());
  EXPECT_EQ("a", rule0->id());
}

TEST(testSyntaxBuilder, idFalse)
{
  SyntaxBuilder builder;

  builder.id("a");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("a"));
  const SyntaxRule *rule = ruleMap.at("a");
  EXPECT_EQ("a", rule->id());

  stringstream stream("1");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, idInTrueA)
{
  SyntaxBuilder builder;

  builder.idIn("ab", {"a", "b"});
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("a");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(1, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("a", tok0->token());
  EXPECT_EQ("ab", rule0->id());
}

TEST(testSyntaxBuilder, idInTrueB)
{
  SyntaxBuilder builder;

  builder.idIn("ab", {"a", "b"});
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("b");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(1, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("b", tok0->token());
  EXPECT_EQ("ab", rule0->id());
}

TEST(testSyntaxBuilder, idInFalse)
{
  SyntaxBuilder builder;

  builder.idIn("ab", {"a", "b"});
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("1");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, idInFalse1)
{
  SyntaxBuilder builder;

  builder.idIn("ab", {"a", "b"});
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("c");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

//------------------------

TEST(testSyntaxBuilder, idNotInTrue)
{
  SyntaxBuilder builder;

  builder.idNotIn("ab", {"a", "b"});
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("c");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  rule->parse(tokenizer, ctx);
  ASSERT_EQ(1, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("c", tok0->token());
  EXPECT_EQ("ab", rule0->id());
}

TEST(testSyntaxBuilder, idNotInFalse)
{
  SyntaxBuilder builder;

  builder.idNotIn("ab", {"a", "b"});
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("1");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, idNotInFalseA)
{
  SyntaxBuilder builder;

  builder.idNotIn("ab", {"a", "b"});
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("a");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, idNotInFalseB)
{
  SyntaxBuilder builder;

  builder.idNotIn("ab", {"a", "b"});
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("b");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

// ------------------------------------

TEST(testSyntaxBuilder, require)
{
  SyntaxBuilder builder;

  builder.require("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("a b");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(3, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("a", tok0->token());
  EXPECT_EQ("a", rule0->id());

  const auto &[tok1, rule1] = ctx.joins()[1];
  EXPECT_EQ("b", tok1->token());
  EXPECT_EQ("b", rule1->id());

  const auto &[tok2, rule2] = ctx.joins()[2];
  EXPECT_EQ("a", tok2->token());
  EXPECT_EQ("ab", rule2->id());
}

TEST(testSyntaxBuilder, requireErrBind)
{
  SyntaxBuilder builder;

  builder.require("ab", {"a", "b"});
  ASSERT_THROW({
        try
        {
          builder.build();
          }
        catch (logic_error ex)
        {
            EXPECT_STREQ("Missing dependant rule a for rule ab", ex.what());
            throw;
        } }, logic_error);
}

TEST(testSyntaxBuilder, requireErrA)
{
  SyntaxBuilder builder;

  builder.require("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("c");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_THROW({
    try
    {
      rule->parse(tokenizer, ctx);
     } catch (QuParseException ex)
      {
        EXPECT_STREQ("Missing a", ex.what());
        throw;
      } }, QuParseException);
}

TEST(testSyntaxBuilder, requireErrB)
{
  SyntaxBuilder builder;

  builder.require("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("a c");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_THROW({
      try
      {
        rule->parse(tokenizer, ctx);
       } catch (QuParseException ex)
        {
          EXPECT_STREQ("Missing b", ex.what());
          throw;
        } }, QuParseException);
}

//----------------------------------

TEST(testSyntaxBuilder, optFalse)
{
  SyntaxBuilder builder;

  builder.opt("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("c b");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, optTrue)
{
  SyntaxBuilder builder;

  builder.opt("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("a b");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(3, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("a", tok0->token());
  EXPECT_EQ("a", rule0->id());

  const auto &[tok1, rule1] = ctx.joins()[1];
  EXPECT_EQ("b", tok1->token());
  EXPECT_EQ("b", rule1->id());

  const auto &[tok2, rule2] = ctx.joins()[2];
  EXPECT_EQ("a", tok2->token());
  EXPECT_EQ("ab", rule2->id());
}

TEST(testSyntaxBuilder, optError)
{
  SyntaxBuilder builder;

  ASSERT_THROW({
    try {
    builder.opt("ab", {});
    } catch (invalid_argument ex){
      ASSERT_STREQ("Missing condition for rule ab", ex.what());
      throw;
    } }, invalid_argument);
}

TEST(testSyntaxBuilder, optErrB)
{
  SyntaxBuilder builder;

  builder.opt("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("a c");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_THROW({
    try{
rule->parse(tokenizer, ctx);
    } catch (QuParseException ex){
      EXPECT_STREQ("Missing b", ex.what());
      throw;
    } }, QuParseException);
}

//--------------------------------

TEST(testSyntaxBuilder, optionsA)
{
  SyntaxBuilder builder;

  builder.options("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("a");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(2, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("a", tok0->token());
  EXPECT_EQ("a", rule0->id());

  const auto &[tok1, rule1] = ctx.joins()[1];
  EXPECT_EQ("a", tok1->token());
  EXPECT_EQ("ab", rule1->id());
}

TEST(testSyntaxBuilder, optionsB)
{
  SyntaxBuilder builder;

  builder.options("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("b");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(2, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("b", tok0->token());
  EXPECT_EQ("b", rule0->id());

  const auto &[tok1, rule1] = ctx.joins()[1];
  EXPECT_EQ("b", tok1->token());
  EXPECT_EQ("ab", rule1->id());
}

TEST(testSyntaxBuilder, optionsFalse)
{
  SyntaxBuilder builder;

  builder.options("ab", {"a", "b"});
  builder.id("a");
  builder.id("b");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("ab"));
  const SyntaxRule *rule = ruleMap.at("ab");
  EXPECT_EQ("ab", rule->id());

  stringstream stream("c");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_FALSE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

//-------------------------------------------

TEST(testSyntaxBuilder, repeat0)
{
  SyntaxBuilder builder;

  builder.repeat("a*", "a");
  builder.id("a");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("a*"));
  const SyntaxRule *rule = ruleMap.at("a*");
  EXPECT_EQ("a*", rule->id());

  stringstream stream("c");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(0, ctx.joins().size());
}

TEST(testSyntaxBuilder, repeat2)
{
  SyntaxBuilder builder;

  builder.repeat("a*", "a");
  builder.id("a");
  RuleMap rules = builder.build();
  const auto &ruleMap = rules.map();

  ASSERT_EQ(1, ruleMap.count("a*"));
  const SyntaxRule *rule = ruleMap.at("a*");
  EXPECT_EQ("a*", rule->id());

  stringstream stream("a a ");
  Tokenizer tokenizer(stream);
  tokenizer.open();

  MockParseContext ctx;
  ASSERT_TRUE(rule->parse(tokenizer, ctx));
  ASSERT_EQ(2, ctx.joins().size());

  const auto &[tok0, rule0] = ctx.joins()[0];
  EXPECT_EQ("a", tok0->token());
  EXPECT_EQ("a", rule0->id());

  const auto &[tok1, rule1] = ctx.joins()[1];
  EXPECT_EQ("a", tok1->token());
  EXPECT_EQ("a", rule1->id());
}
