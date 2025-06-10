#ifndef _tokenizer_h_
#define _tokenizer_h_

#include <iostream>
#include <optional>
#include <string>
#include "token.h"

namespace qc
{
    class TokenizerIntf
    {
    public:
        virtual void popToken(void) = 0;
        virtual const Token &currentToken(void) const = 0;
    };

    // Parse a stream producing token
    class Tokenizer : public TokenizerIntf
    {
        std::istream &_stream;
        Token *_currentToken;
        std::optional<std::string> _currentLine;
        size_t _lineNumber;
        size_t _currentPos;
        size_t _tokenLineNumber;
        size_t _tokenPos;
        std::optional<std::string> _tokenLine;

        void popChar(void);
        const bool eof(void) const;
        const int currentChar(void) const;
        void readNextLine(void);
        void parseNumber(void);
        void parseIdentifier(void);
        void parseSlash(void);
        void parseFract(const std::string &prefix);
        void parseExp(const std::string &prefix);
        void skipBlanks(void);
        void skipComment(void);
        const SourceContext createTokenContext(const std::string &token) const;
        const SourceContext createCharContext(void) const;

    public:
        /**
         * Creates the tokenizer
         * @param stream the stream
         */
        Tokenizer(std::istream &stream);

        /**
         * Opens the tokenizer
         */
        Tokenizer &open(void);

        /**
         * Pops a token from tokenizer
         */
        virtual void popToken(void) override;

        /**
         * Returns the current token
         */
        virtual const Token &currentToken(void) const override { return *_currentToken; }
    };
}

#endif
