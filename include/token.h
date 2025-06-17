#ifndef _token_h_
#define _token_h_

#include <ostream>
#include <string>

#include "sourceContext.h"

namespace qc
{
    /**
     * The token types
     */
    enum class TokenType
    {
        eofTokenType,
        identifierTokenType,
        integerTokenType,
        realTokenType,
        opTokenType
    };

    /**
     * Keeps the context and the token result of Tokenizer
     */
    class Token
    {

    private:
        SourceContext _context;

    protected:
        /**
         * Creates the token
         * @param context the source context
         * @param type the token type
         */
        Token(const SourceContext &context) : _context(context) {}

    public:
        /**
         * Returns the context
         */
        const SourceContext &context(void) const { return _context; }

        /**
         * Returns the token
         */
        const std::string &token(void) const { return _context.token(); }

        /**
         * Returns the token type
         */
        virtual const TokenType type(void) const = 0;

        /**
         * Returns the polymorphic copy of token
         */
        virtual Token *clone(void) const = 0;
    };

    class IntegerToken : public Token
    {
    public:
        IntegerToken(const SourceContext &context) : Token(context) {}

        /**
         * Returns the token type
         */
        virtual const TokenType type(void) const override { return TokenType::integerTokenType; }

        /**
         * Returns the integer value of token
         */
        const int value(void) const;

        /**
         * Returns the polymorphic copy of token
         */
        virtual IntegerToken *clone(void) const override;
    };

    class RealToken : public Token
    {
    public:
        RealToken(const SourceContext &context) : Token(context) {}

        /**
         * Returns the token type
         */
        virtual const TokenType type(void) const override { return TokenType::realTokenType; }

        /**
         * Returns the real value of token
         */
        const double value(void) const;

        /**
         * Returns the polymorphic copy of token
         */
        virtual RealToken *clone(void) const override;
    };

    /**
     * End of file token
     */
    class EOFToken : public Token
    {
    public:
        EOFToken(const SourceContext &context) : Token(context) {}

        /**
         * Returns the token type
         */
        virtual const TokenType type(void) const override { return TokenType::eofTokenType; }

        /**
         * Returns the polymorphic copy of token
         */
        virtual EOFToken *clone(void) const override;
    };

    /**
     * Operator token
     */
    class OperatorToken : public Token
    {
    public:
        OperatorToken(const SourceContext &context) : Token(context) {}

        /**
         * Returns the token type
         */
        virtual const TokenType type(void) const override { return TokenType::opTokenType; }

        /**
         * Returns the polymorphic copy of token
         */
        virtual OperatorToken *clone(void) const override;
    };

    /**
     * Identifier token
     */
    class IdentifierToken : public Token
    {
    public:
        IdentifierToken(const SourceContext &context) : Token(context) {}

        /**
         * Returns the token type
         */
        virtual const TokenType type(void) const override { return TokenType::identifierTokenType; }

        /**
         * Returns the polymorphic copy of token
         */
        virtual IdentifierToken *clone(void) const override;
    };
}

std::ostream &operator<<(std::ostream &os, const qc::Token &token);

#endif
