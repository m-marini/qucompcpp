#ifndef _compiler_c_
#define _compiler_c_

#include <deque>
#include <string>

#include "compilerContext.h"
#include "syntaxRules.h"
#include "tokenizer.h"

namespace qc
{

    class Compiler : public ParseContext, public CompilerContext
    {
        Tokenizer &_tokenizer;
        std::map<const std::string, std::function<void(CompilerContext &, const Token &)>> _opers;
        std::deque<NodeCommand *> _stack;

    public:
        ~Compiler();

        /**
         * Returns the compiler of qu language
         * @param tokenizer the tokenizer
         */
        static Compiler *createQu(Tokenizer &tokenizer);

        /**
         * Creates the compiler
         * @param tokenizer the tokenizer
         */
        Compiler(Tokenizer &tokenizer) : _tokenizer(tokenizer) {}

        /**
         * Returns the command stack
         */
        const std::deque<NodeCommand *> stack() const { return _stack; }

        /**
         * Adds the operator for the rule
         * @èaram rule the rule identifier
         * @param op the operator (funtion handling the token)
         */
        Compiler &add(const std::string &rule, std::function<void(CompilerContext &, const Token &)> &op);

        /**
         * Handles the join event
         * @param token the token
         * @param rule the rule
         */
        virtual void join(const Token &token, const SyntaxRule &rule) override;

        /**
         * Push the command in the stack
         * @param command the command
         */
        virtual CompilerContext &pushCommand(NodeCommand *command) override;

        /**
         * Returns the last command from stack by removing
         */
        virtual NodeCommand *popCommand(void) override;
    };
}
#endif
