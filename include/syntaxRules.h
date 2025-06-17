#ifndef _syntaxRules_h_
#define _syntaxRules_h_

#include <vector>
#include <string>
#include <set>

#include "token.h"
#include "tokenProducer.h"

namespace qc
{
    class SyntaxRule;

    class ParseContext
    {
    public:
        virtual void join(const Token &token, const SyntaxRule &rule) = 0;
    };

    /**
     * Parses the parse context.
     * If the rule matches the current token, it joins itself with the token and discards the token from parse context
     */
    class SyntaxRule
    {
    public:
        /**
         * Creates the syntax rule
         * @param id the rule identifier
         */
        SyntaxRule(const std::string &id) : _id(id) {}

        /**
         * Retruns the rule identifier
         */
        const std::string &id(void) const { return _id; }

        /**
         * Parses the context applying the rule.
         * Returns true if the rule is applied
         *
         * @param context the parse context
         */
        virtual const bool parse(TokenProducer &tokenizer, ParseContext &context) const = 0;

        /**
         * Binds the rule to dependency rules
         * @param dependencies the rule dependencies
         */
        virtual void bind(const std::vector<SyntaxRule *> &dependencies) {};

    private:
        std::string _id;
    };

    class EofRule : public SyntaxRule
    {
    public:
        EofRule(const std::string &id) : SyntaxRule(id) {}
        virtual const bool parse(TokenProducer &tokenizer, ParseContext &context) const override;
    };

    class TerminalRule : public SyntaxRule
    {
    public:
        TerminalRule(const std::string &id) : SyntaxRule(id) {}
        virtual const bool parse(TokenProducer &tokenizer, ParseContext &context) const override;
        virtual const bool match(const Token &token) const = 0;
    };

    class IntRule : public TerminalRule
    {
    public:
        IntRule(const std::string &id) : TerminalRule(id) {}
        virtual const bool match(const Token &token) const override;
    };

    class RealRule : public TerminalRule
    {
    public:
        RealRule(const std::string &id) : TerminalRule(id) {}
        virtual const bool match(const Token &token) const override;
    };

    class OperRule : public TerminalRule
    {
    public:
        OperRule(const std::string &id) : TerminalRule(id) {}
        virtual const bool match(const Token &token) const override;
    };

    class IdInRule : public TerminalRule
    {
        std::set<std::string> _idents;

    public:
        IdInRule(const std::string &id, const std::set<std::string> &idents) : TerminalRule(id), _idents(idents) {}
        virtual const bool match(const Token &token) const override;
    };

    class IdNotInRule : public TerminalRule
    {
        std::set<std::string> _idents;

    public:
        IdNotInRule(const std::string &id, const std::set<std::string> &idents) : TerminalRule(id), _idents(idents) {}
        virtual const bool match(const Token &token) const override;
    };

    class IdRule : public TerminalRule
    {
    public:
        IdRule(const std::string &id) : TerminalRule(id) {}
        virtual const bool match(const Token &token) const override;
    };

    class EmptyRule : public SyntaxRule
    {
    public:
        EmptyRule(const std::string &id) : SyntaxRule(id) {}
        virtual const bool parse(TokenProducer &tokenizer, ParseContext &context) const override;
    };

    class NonTerminalRule : public SyntaxRule
    {
        std::vector<SyntaxRule *> _rules;

    public:
        NonTerminalRule(const std::string &id) : SyntaxRule(id) {}

        /**
         * Binds the rule to dependency rules
         * @param rules the rule dependencies
         */
        virtual void bind(const std::vector<SyntaxRule *> &rules) override
        {
            _rules = rules;
        }

        /**
         * Returns the dependency rules
         */
        const std::vector<SyntaxRule *> &rules(void) const { return _rules; }
    };

    class RequireRule : public NonTerminalRule
    {
    public:
        RequireRule(const std::string &id) : NonTerminalRule(id) {}
        virtual const bool parse(TokenProducer &tokenizer, ParseContext &context) const override;
    };

    class OptRule : public NonTerminalRule
    {
    public:
        OptRule(const std::string &id) : NonTerminalRule(id) {}
        virtual const bool parse(TokenProducer &tokenizer, ParseContext &context) const override;
    };

    class OptionsRule : public NonTerminalRule
    {
    public:
        OptionsRule(const std::string &id) : NonTerminalRule(id) {}
        virtual const bool parse(TokenProducer &tokenizer, ParseContext &context) const override;
    };

    class RepeatRule : public NonTerminalRule
    {
    public:
        RepeatRule(const std::string &id) : NonTerminalRule(id) {}
        virtual const bool parse(TokenProducer &tokenizer, ParseContext &context) const override;
    };
}

#endif
