#ifndef _token_h_
#define _token_h_

#include <iostream>
#include <optional>
#include <string>

namespace qc
{
    class QuParseException;
    class QuExecException;

    /**
     * The information of token source code
     */
    class SourceContext
    {
        std::string _token;
        std::string _line;
        size_t _lineNumber;
        size_t _position;

    public:
        /**
         * Creates the source context
         * @param token the token
         * @param line the line
         * @param lineNumber the line number
         * @param position the position
         */
        SourceContext(const std::string &token, const std::string &line, const size_t lineNumber, const size_t position)
            : _token(token), _line(line), _lineNumber(lineNumber), _position(position)
        {
        }

        /**
         * Returns the token
         */
        const std::string &token(void) const { return _token; };

        /**
         * Returns the line
         */
        const std::string &line(void) const { return _line; }

        /**
         * Returns the line number
         */
        const size_t &lineNumber(void) const { return _lineNumber; }

        /**
         * Returns the position
         */
        const size_t &position(void) const { return _position; }

        /**
         * Creates the parse exception for the give context
         * @param what the exception message
         */
        const QuParseException parseException(const std::string &what) const;

        /**
         * Creates the exec exception for the give context
         * @param what the exception message
         */
        const QuExecException execException(const std::string &what) const;
    };

    class QuSourceException : public std::logic_error
    {
        std::string _what;
        SourceContext _context;

    public:
        QuSourceException(const std::string &what, const SourceContext &context)
            : std::logic_error(what), _what(what), _context(context) {}
        const SourceContext &context() const { return _context; }
        virtual const char *what() const noexcept override { return _what.c_str(); }
    };

    class QuParseException : public QuSourceException
    {
    public:
        QuParseException(const std::string &what, const SourceContext &context)
            : QuSourceException(what, context) {}
    };

    class QuExecException : public QuSourceException
    {
    public:
        QuExecException(const std::string &what, const SourceContext &context)
            : QuSourceException(what, context) {}
    };

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
        const TokenType &type(void) const { return _type; }

        /**
         * Returns the polymorphic copy of token
         */
        virtual Token *clone(void) const = 0;

    protected:
        /**
         * Creates the token
         * @param context the source context
         * @param type the token type
         */
        Token(const SourceContext &context, const TokenType &type) : _context(context), _type(type) {}

    private:
        SourceContext _context;
        TokenType _type;
    };

    class IntegerToken : public Token
    {
    public:
        IntegerToken(const SourceContext &context) : Token(context, TokenType::integerTokenType) {}

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
        RealToken(const SourceContext &context) : Token(context, TokenType::realTokenType) {}

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
        EOFToken(const SourceContext &context) : Token(context, TokenType::eofTokenType) {}

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
        OperatorToken(const SourceContext &context) : Token(context, TokenType::opTokenType) {}

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
        IdentifierToken(const SourceContext &context) : Token(context, TokenType::identifierTokenType) {}

        /**
         * Returns the polymorphic copy of token
         */
        virtual IdentifierToken *clone(void) const override;
    };
}

std::ostream &operator<<(std::ostream &os, const qc::Token &token);

#endif
