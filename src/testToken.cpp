#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include "token.h"

using namespace std;
using namespace qc;

TEST(testToken, testEOFToken)
{
    SourceContext ctx("", "", 1, 0);
    EOFToken token(ctx);

    EXPECT_EQ(TokenType::eofTokenType, token.type());
    EXPECT_EQ("", token.token());
    EXPECT_EQ(1, token.context().lineNumber());
    EXPECT_EQ(0, token.context().position());
}

TEST(testToken, testEOFTokenClone)
{
    SourceContext ctx("", "", 1, 0);
    EOFToken token(ctx);

    const Token *tokenPtr = &token;

    const Token &token1 = *(tokenPtr->clone());

    EXPECT_EQ(TokenType::eofTokenType, token.type());
    EXPECT_EQ("", token.token());
    EXPECT_EQ(1, token.context().lineNumber());
    EXPECT_EQ(0, token.context().position());
}