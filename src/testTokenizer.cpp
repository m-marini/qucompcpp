#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include "tokenizer.h"

using namespace std;
using namespace qc;

TEST(testTokenizer, testInt)
{
    istringstream stream(string("1"));

    Tokenizer tok(stream);
    tok.open();

    auto &token = tok.currentToken();
    ASSERT_EQ(TokenType::integerTokenType, token.type());
    EXPECT_EQ("1", token.token());
    EXPECT_EQ(1, ((IntegerToken *)&token)->value());
    EXPECT_EQ("1", token.context().line());
    EXPECT_EQ(1, token.context().lineNumber());
    EXPECT_EQ(0, token.context().position());

    const Token *pt = token.clone();
    ASSERT_EQ(TokenType::integerTokenType, pt->type());
    EXPECT_EQ("1", pt->token());
    EXPECT_EQ(1, ((IntegerToken *)pt)->value());
    EXPECT_EQ("1", pt->context().line());
    EXPECT_EQ(1, pt->context().lineNumber());
    EXPECT_EQ(0, pt->context().position());
}

TEST(testTokenizer, testTokenzerSeq)
{
    istringstream stream(string("   + \n ii \n 12 1.2 1e3 1e-3 1.2e12 1.2e-12 // commend\n / /*\n other comment\n*/ a"));

    Tokenizer tok(stream);
    tok.open();

    auto &token1 = tok.currentToken();
    EXPECT_EQ(TokenType::opTokenType, token1.type());
    EXPECT_EQ("+", token1.token());
    EXPECT_EQ(1, token1.context().lineNumber());
    EXPECT_EQ(3, token1.context().position());

    tok.popToken();
    auto &token2 = tok.currentToken();

    EXPECT_EQ(TokenType::identifierTokenType, token2.type());
    EXPECT_EQ("ii", token2.token());
    EXPECT_EQ(2, token2.context().lineNumber());
    EXPECT_EQ(1, token2.context().position());

    tok.popToken();
    auto &token3 = tok.currentToken();
    ASSERT_EQ(TokenType::integerTokenType, token3.type());
    EXPECT_EQ("12", token3.token());
    EXPECT_EQ(12, ((IntegerToken *)&token3)->value());

    tok.popToken();
    auto &token4 = tok.currentToken();
    ASSERT_EQ(TokenType::realTokenType, token4.type());
    EXPECT_EQ("1.2", token4.token());
    EXPECT_EQ(1.2, ((RealToken *)&token4)->value());

    tok.popToken();
    auto &token5 = tok.currentToken();
    ASSERT_EQ(TokenType::realTokenType, token5.type());
    EXPECT_EQ("1e3", token5.token());
    EXPECT_EQ(1e3, ((RealToken *)&token5)->value());

    tok.popToken();
    auto &token6 = tok.currentToken();
    ASSERT_EQ(TokenType::realTokenType, token6.type());
    EXPECT_EQ("1e-3", token6.token());
    EXPECT_EQ(1e-3, ((RealToken *)&token6)->value());

    tok.popToken();
    auto &token7 = tok.currentToken();
    ASSERT_EQ(TokenType::realTokenType, token7.type());
    EXPECT_EQ("1.2e12", token7.token());
    EXPECT_EQ(1.2e12, ((RealToken *)&token7)->value());

    tok.popToken();
    auto &token8 = tok.currentToken();
    ASSERT_EQ(TokenType::realTokenType, token8.type());
    EXPECT_EQ("1.2e-12", token8.token());
    EXPECT_EQ(1.2e-12, ((RealToken *)&token8)->value());

    tok.popToken();
    auto &token9 = tok.currentToken();
    EXPECT_EQ(TokenType::opTokenType, token9.type());
    EXPECT_EQ("/", token9.token());

    tok.popToken();
    auto &token10 = tok.currentToken();
    EXPECT_EQ(TokenType::identifierTokenType, token10.type());
    EXPECT_EQ("a", token10.token());

    tok.popToken();
    auto &endToken = tok.currentToken();
    EXPECT_EQ(TokenType::eofTokenType, endToken.type());
}

TEST(testTokenizer, testTokenzerError1)
{
    istringstream stream(string("1E"));
    Tokenizer tok(stream);

    ASSERT_THROW({
        try
        {
            tok.open();
        }
        catch (QuParseException ex)
        {
            EXPECT_STREQ("Missing exponent", ex.what());
            throw;
        } }, QuParseException);
}

TEST(testTokenizer, testTokenzerError2)
{
    istringstream stream(string("1E+"));
    Tokenizer tok(stream);

    ASSERT_THROW({
        try
        {
            tok.open();
        }
        catch (QuParseException ex)
        {
            EXPECT_STREQ("Missing exponent", ex.what());
            throw;
        } }, QuParseException);
}

TEST(testTokenizer, testTokenzerError3)
{
    istringstream stream(string("1E-"));
    Tokenizer tok(stream);

    ASSERT_THROW({
        try
        {
            tok.open();
        }
        catch (QuParseException ex)
        {
            EXPECT_STREQ("Missing exponent", ex.what());
            throw;
        } }, QuParseException);
}