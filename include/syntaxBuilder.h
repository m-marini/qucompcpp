#ifndef _syntaxBuilder_h_
#define _syntaxBuilder_h_

#include <string>
#include <map>
#include <vector>
#include <set>
#include <functional>
#include <utility>

#include "tokenizer.h"

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
        virtual const bool parse(TokenizerIntf &tokenizer, ParseContext &context) const = 0;

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
        virtual const bool parse(TokenizerIntf &tokenizer, ParseContext &context) const override;
    };

    class TerminalRule : public SyntaxRule
    {
    public:
        TerminalRule(const std::string &id) : SyntaxRule(id) {}
        virtual const bool parse(TokenizerIntf &tokenizer, ParseContext &context) const override;
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
        virtual const bool parse(TokenizerIntf &tokenizer, ParseContext &context) const override;
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
        virtual const bool parse(TokenizerIntf &tokenizer, ParseContext &context) const override;
    };

    class OptRule : public NonTerminalRule
    {
    public:
        OptRule(const std::string &id) : NonTerminalRule(id) {}
        virtual const bool parse(TokenizerIntf &tokenizer, ParseContext &context) const override;
    };

    class OptionsRule : public NonTerminalRule
    {
    public:
        OptionsRule(const std::string &id) : NonTerminalRule(id) {}
        virtual const bool parse(TokenizerIntf &tokenizer, ParseContext &context) const override;
    };

    class RepeatRule : public NonTerminalRule
    {
    public:
        RepeatRule(const std::string &id) : NonTerminalRule(id) {}
        virtual const bool parse(TokenizerIntf &tokenizer, ParseContext &context) const override;
    };

    class RuleMap
    {
    public:
        RuleMap(const std::map<const std::string, SyntaxRule *> &map) : _map(map) {}
        ~RuleMap();
        const std::map<const std::string, SyntaxRule *> map(void) const { return _map; }

    private:
        std::map<const std::string, SyntaxRule *> _map;
    };

    /**
     * The syntax rule builder decouples the definition of syntax rules from the building
     * allowing recursive rule references.
     * <p>
     * The terminal rule composers are:
     *
     * <pre>
     * empty()                = &lt;empty> ::= ""
     * id(id)                 = &lt;id> ::= id
     * idIn(id1, ..., idn)    = &lt;any-id-in>
     * idNotIn(id1, ..., idn) = &lt;any-id-not-in>
     * intLiteral()           = &lt;any-int-literal>
     * eof()                  = &lt;eof>
     * oper(char)             = &lt;oper> ::= char
     * realLiteral()          = &lt;any-real-literal>
     * </pre>
     * <p>
     * The non-terminal rule composers are:
     *
     * <pre>
     * opt(cond, exp1, ..., expn) = &lt;opt> ::= &lt;cond> &lt;exp1> ... &lt;expn> | ""
     * options(exp1, ..., expn)   = &lt;opt> ::= &lt;exp1> |  ... | &lt;expn> | ""
     * repeat(exp)                = &lt;repeat> := &lt;exp> &lt;repeat> | ""
     * require(exp1, ..., expn)   = &lt;require> := &lt;exp1> ... &lt;expn>
     * </pre>
     */
    class SyntaxBuilder
    {
        std::map<std::string, std::function<SyntaxRule *(void)>> _builders;
        std::map<std::string, std::vector<std::string>> _binders;

    public:
        /**
         * Returns the syntax rule by building all referenced rules and binding them together
         */
        const RuleMap build(void);

        /**
         * Adds the empty rule
         *
         * @param id the rule identifier
         */
        SyntaxBuilder &empty(const std::string &id);

        /**
         * Adds the eof rule
         *
         * @param id the rule identifier
         */
        SyntaxBuilder &eof(const std::string &id);

        /**
         * Adds the int literal rule
         *
         * @param id the rule identifier
         */
        SyntaxBuilder &intLiteral(const std::string &id);

        /**
         * Adds the real literal rule
         *
         * @param id the rule identifier
         */
        SyntaxBuilder &realLiteral(const std::string &id);

        /**
         * Adds the operand rule
         *
         * @param id the rule identifier
         */
        SyntaxBuilder &oper(const std::string &id);

        /**
         * Adds the identifier rule
         *
         * @param id the rule identifier
         */
        SyntaxBuilder &id(const std::string &id);

        /**
         * Adds the identifier in rule
         *
         * @param id the rule identifier
         * @param idents the set of valid identifier
         */
        SyntaxBuilder &idIn(const std::string &id, const std::set<std::string> &idents);

        /**
         * Adds the identifier not in rule
         *
         * @param id the rule identifier
         * @param idents the set of invalid identifier
         */
        SyntaxBuilder &idNotIn(const std::string &id, const std::set<std::string> &idents);

        /**
         * Adds the require rule
         *
         * @param id the rule identifier
         * @param ruleIds the set of required rule identifiers
         */
        SyntaxBuilder &require(const std::string &id, const std::vector<std::string> &ruleIds);

        /**
         * Adds the optional rule
         *
         * @param id the rule identifier
         * @param ruleIds the set of optional rule identifiers (first rule is the conditional rule)
         */
        SyntaxBuilder &opt(const std::string &id, const std::vector<std::string> &ruleIds);

        /**
         * Adds the options rule
         *
         * @param id the rule identifier
         * @param ruleIds the set of options rule identifiers
         */
        SyntaxBuilder &options(const std::string &id, const std::vector<std::string> &ruleIds);

        /**
         * Adds the options rule
         *
         * @param id the rule identifier
         * @param ruleId the repeatable rule identifiers
         */
        SyntaxBuilder &repeat(const std::string &id, const std::string &ruleId);
    };
}

#endif
