#ifndef _sourceContext_h_
#define _sourceContext_h_

#include <ostream>
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

        /**
         * Writes the context with a message
         */
        std::ostream &write(std::ostream &out, const std::string &message) const;
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
}

std::ostream &operator<<(std::ostream &os, const qc::SourceContext &token);

std::ostream &operator<<(std::ostream &os, const qc::QuSourceException &ex);

#endif
