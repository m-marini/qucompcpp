#ifndef _syntaxBuilder_h_
#define _syntaxBuilder_h_

#include <string>
#include <map>
#include <functional>

#include "syntaxRules.h"

namespace qc
{

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
