#ifndef _parser_h_
#define _parser_h_

#include <string>
#include <vector>
#include <optional>
#include <functional>

#include "qugates.h"

namespace qp
{

    /**
     * Defines a quantum gate
     */
    class Parser
    {
    public:
        Parser(std::function<const std::optional<std::string>(void)> readLine);

        const qg::gates_t parse(void);

    private:
        std::function<std::optional<const std::string>(void)> _readLine;
        std::optional<std::string> _currentLine;
        std::optional<std::string> _currentToken;
        size_t _lineNumber;
        size_t _currentPos;

        /**
         * Returns the curret character or -1 if EOF
         */
        const int currentChar(void) const
        {
            return _currentLine ? _currentLine.value().at(_currentPos) : -1;
        }

        const std::optional<std::string> &peekToken(void) const { return _currentToken; }

        /**
         * pop a token from supplier
         */
        void consumeToken(void);

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
         * Skips the blanks
         */
        void skipBlanks(void);

        /**
         * Skips the current character
         */
        void skipChar(void);

        /**
         * Returns true if eof matched
         */
        const bool eof(void) const { return !_currentLine; }

        /**
         * Return sthe current parsed text
         */
        const std::string currentText(void) const;

        /**
         * Reports the error
         * @param msg the error
         */
        void reportError(const std::string& msg) const;
    };

    extern const qg::gates_t parseString(const std::string &text);
    extern const qg::gates_t parseStream(std::istream &stream);
    extern const qg::gates_t parseArgs(const int argc, const char **argv);
}

#endif
