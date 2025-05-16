#ifndef _parser_h_
#define _parser_h_

#include <string>
#include <vector>
#include <optional>
#include <functional>

#include "qugates.h"

namespace qp
{
    // Parse a stream producing token
    class Tokenizer
    {
    public:
        /**
         * Creates the tokenizer
         * @param stream the stream
         */
        Tokenizer(std::istream &stream);

        /**
         * Pops a token from tokenizer
         */
        void popToken(void);

        /**
         * Returns true if the tokenizer has readh the eon of file
         */
        const bool eof(void) const;

        /**
         * Returns the current token
         */
        const std::optional<std::string> &currentToken(void) const { return _currentToken; }

        /**
         * Returns the token line
         */
        const std::optional<std::string> &tokenLine() const { return _tokenLine; }

        /**
         * Returns the token line number
         */
        const std::size_t &tokenLineNum() const { return _tokenLineNum; }

        /**
         * Returns the token position
         */
        const std::size_t &tokenPos() const { return _tokenPos; }

    private:
        std::istream &_stream;
        std::optional<std::string> _currentLine;
        std::size_t _currentPos;
        std::size_t _lineNum;
        std::optional<std::string> _currentToken;
        std::optional<std::string> _tokenLine;
        std::size_t _tokenPos;
        std::size_t _tokenLineNum;

        /**
         * Pops a character from tokenizer
         */
        Tokenizer &popChar(void);

        /**
         * Returns the current character
         */
        const int currentChar(void) const;

        void parseNumber(void);
        void parseIdentifier(void);
        void readNextLine(void);
        void parseSlash(void);
        void skipBlanks(void);
        void skipComment(void);
    };

    /**
     * Defines a quantum gate
     */
    class Parser
    {
    public:
        Parser(Tokenizer &tokenizer) : _tokenizer(tokenizer) {}

        const qg::gates_t parse(void);

    private:
        Tokenizer &_tokenizer;

        /**
         * Return the optional next gate
         */
        const std::optional<qg::QuGate> parseGateOpt(void);

        /**
         * Return the optional next unary gate
         */
        const std::optional<qg::QuGate> parseUnaryGate(void);

        /**
         * Return the optional next binary gate
         */
        const std::optional<qg::QuGate> parseBinaryGate(void);

        /**
         * Return the optional next trinary gate
         */
        const std::optional<qg::QuGate> parseTrinaryGate(void);

        /**
         * Return the parsed number
         */
        const int parseNumber(void);

        /**
         * Skip a required token
         */
        void requiredToken(const std::string &token);

        /**
         * Reports the error
         * @param msg the error
         */
        void reportError(const std::string &msg) const;
    };

    extern const qg::gates_t parseString(const std::string &text);
    extern const qg::gates_t parseStream(std::istream &stream);
    //    extern const qg::gates_t parseArgs(const int argc, const char **argv);
}

#endif
