#include "qusyntax.h"
#include "processor.h"

using namespace std;
using namespace qc;

const RuleMap Syntax::build(void)
{
    set<string> functionId;

    QU_PROCESSOR_FUNCTIONS;

    for (const auto &[key, value] : QU_PROCESSOR_FUNCTIONS)
    {
        functionId.insert(key);
    }

    set<string> statementKeywords{
        "clear",
        "let",
    };
    set<string> reservedKeywords{
        "i",  // imaginary unit
        "e",  // imaginary unit
        "pi", // imaginary unit
        "x",  // cross-operator
        // Functions
        "exp",
        "pow",
        "sin",
        "cos",
        "tan",
        "acos",
        "asin",
        "atan",
        "arg",
        "sinh",
        "cosh",
        "tanh"};
    set<string> keywords = functionId;
    set<string> tmp = statementKeywords;
    keywords.merge(tmp);
    tmp = reservedKeywords;
    keywords.merge(tmp);

    SyntaxBuilder builder;

    return builder
        .require("<code-unit>", {"<code-unit-head>", "<statement-list>", "<eof>"})
        .repeat("<statement-list>", "<stm>")
        .opt("<stm>", {"<stm-opt>", ";"})
        .options("<stm-opt>", {"<clear-stm>", "<assign-stm>", "<exp-opt>"})
        .require("<eof>", {"<end>"})
        .eof("<end>")
        .empty("<code-unit-head>")

        .opt("<clear-stm>", {"clear", "(", ")"})
        .opt("<assign-stm>", {"let", "<assign-var-identifier>", "=", "<exp>"})

        .require("<exp>", {"<exp-opt>"})
        .opt("<exp-opt>", {"<add-exp>"})

        .opt("<add-exp>", {"<multiply-exp>", "<add-tail>"})
        .repeat("<add-tail>", "<add-tail-opt>")
        .options("<add-tail-opt>", {"<plus-tail>", "<minus-tail>"})
        .opt("<plus-tail>", {"+", "<multiply-exp>"})
        .opt("<minus-tail>", {"-", "<multiply-exp>"})

        .opt("<multiply-exp>", {"<cross-exp>", "<mul-tail>"})
        .repeat("<mul-tail>", "<mul-tail-opt>")
        .options("<mul-tail-opt>", {"<multiply-tail>", "<multiply-tail-star>", "<divide-tail>"})
        .opt("<multiply-tail>", {".", "<cross-exp>"})
        .opt("<multiply-tail-star>", {"*", "<cross-exp>"})
        .opt("<divide-tail>", {"/", "<cross-exp>"})

        .opt("<cross-exp>", {"<unary-exp>", "<cross-tail>"})
        .repeat("<cross-tail>", "<cross-tail-opt>")
        .opt("<cross-tail-opt>", {"x", "<unary-exp>"})

        .options("<unary-exp>", {"<plus-exp>", "<negate-exp>", "<conj>"})
        .opt("<plus-exp>", {"+", "<unary-exp>"})
        .opt("<negate-exp>", {"-", "<unary-exp>"})

        .opt("<conj>", {"<primary-exp>", "<conj-tail>"})
        .repeat("<conj-tail>", "^")

        .options("<primary-exp>",
                 {"<priority-exp>",
                  "<bra>",
                  "<ket>",
                  "<im-unit>",
                  "pi",
                  "e",
                  "<function>",
                  "<var-identifier>",
                  "<int-literal>",
                  "<real-literal>"})

        .opt("<priority-exp>", {"(", "<exp>", ")"})
        .opt("<bra>", {"<", "<state-exp>", "|"})
        .opt("<ket>", {"|", "<state-exp>", ">"})

        .require("<state-exp>", {"<state-exp-opt>"})
        .options("<state-exp-opt>", {"<im-state>", "<plus-state>", "<minus-state-exp>", "<int-state>"})
        .opt("<minus-state-exp>", {"-", "<minus-state-exp-opt>"})
        .options("<minus-state-exp-opt>", {"<minus-im-state>", "<minus-state>"})
        .empty("<minus-state>")

        .require("<int-state>", {"<exp-opt>"})

        .opt("<im-unit>", {"i"})
        .opt("<im-state>", {"i"})
        .opt("<minus-im-state>", {"i"})
        .opt("<plus-state>", {"+"})

        .opt("<function>", {"<function-id>", "<args-exp>"})
        .require("<args-exp>", {"(", "<arg-list>", ")"})
        .options("<arg-list>", {"<arg-list-opt>", "<empty-arg>"})
        .opt("<arg-list-opt>", {"<arg>", "<arg-list-tail>"})
        .repeat("<arg-list-tail>", "<arg-tail>")
        .opt("<arg-tail>", {",", "<exp>"})
        .empty("<empty-arg>")
        .opt("<arg>", {"<exp-opt>"})

        .idNotIn("<var-identifier>", keywords)
        .idNotIn("<assign-var-identifier>", keywords)

        .idIn("<function-id>", functionId)

        .id("let")
        .id("clear")

        .id("e")
        .id("pi")
        .id("i")
        .intLiteral("<int-literal>")
        .realLiteral("<real-literal>")

        .id("x")
        .oper("+")
        .oper("-")
        .oper("<")
        .oper("|")
        .oper(">")
        .oper("(")
        .oper(")")
        .oper(",")
        .oper("^")
        .oper("*")
        .oper("/")
        .oper("=")
        .oper(";")
        .oper(".")

        .build();
}
