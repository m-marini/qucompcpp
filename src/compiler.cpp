#include <string>
#include <cmath>
#include <complex>
#include <cassert>

#include "tokenizer.h"
#include "syntaxBuilder.h"
#include "compiler.h"
#include "values.h"
#include "commands.h"
#include "matrix.h"
#include "processor.h"

using namespace std;
using namespace qc;
using namespace mx;

typedef function<void(CompilerContext &, const Token &)> RuleOper;

Compiler *Compiler::createQu(Tokenizer &tokenizer)
{
    Compiler *compiler = new Compiler(tokenizer);
    RuleOper intOper = [](CompilerContext &context, const Token &token)
    {
                        if (token.type()== TokenType::integerTokenType) {
                            context.pushCommand(new IntCommand(token.context(), ((const IntegerToken*)&token)->value()));
                        } else {
                            throw token.context().parseException("Integer token expected ()" + token.token());
                        } };
    RuleOper retriveVarOper = [](CompilerContext &context, const Token &token)
    {
                        if (token.type()== TokenType::identifierTokenType) {
                            context.pushCommand(new RetrieveVarCommand(token.context(), token.token()));
                        } else {
                            throw token.context().parseException("Identifier token expected ()" + token.token());
                        } };
    RuleOper realOper = [](CompilerContext &context, const Token &token)
    {
                        if (token.type()== TokenType::realTokenType) {
                            context.pushCommand(new ComplexCommand(token.context(), ((const RealToken*)&token)->value()));
                        } else {
                            throw token.context().parseException("Real toke expected ()" + token.token());
                        } };

    RuleOper codeUnitHeadOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new ListCommand(token.context()));
    };

    RuleOper stmOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *stm = context.popCommand();
        CompositeCommand *code = (CompositeCommand *)context.popCommand();
        context.pushCommand(&(code->add(stm)));
    };

    RuleOper imOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new ComplexCommand(token.context(), 1i));
    };

    RuleOper piOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new ComplexCommand(token.context(), M_PI));
    };

    RuleOper eOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new ComplexCommand(token.context(), M_E));
    };

    RuleOper int2StateOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new Int2StateCommand(token.context(), context.popCommand()));
    };

    RuleOper minusStateOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new MatrixCommand(token.context(), MINUS_KET));
    };

    RuleOper plusStateOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new MatrixCommand(token.context(), PLUS_KET));
    };

    RuleOper imStateOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new MatrixCommand(token.context(), I_KET));
    };

    RuleOper minusImStateOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new MatrixCommand(token.context(), MINUS_I_KET));
    };

    RuleOper daggerOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new DaggerCommand(token.context(), context.popCommand()));
    };

    RuleOper negOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new NegateCommand(token.context(), context.popCommand()));
    };

    RuleOper crossOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *right = context.popCommand();
        NodeCommand *left = context.popCommand();
        context.pushCommand(new CrossCommand(token.context(), left, right));
    };

    RuleOper mulOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *right = context.popCommand();
        NodeCommand *left = context.popCommand();
        context.pushCommand(new MultiplyCommand(token.context(), left, right));
    };

    RuleOper mulStarOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *right = context.popCommand();
        NodeCommand *left = context.popCommand();
        context.pushCommand(new MultiplyStarCommand(token.context(), left, right));
    };

    RuleOper divOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *right = context.popCommand();
        NodeCommand *left = context.popCommand();
        context.pushCommand(new DivideCommand(token.context(), left, right));
    };

    RuleOper addOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *right = context.popCommand();
        NodeCommand *left = context.popCommand();
        context.pushCommand(new AddCommand(token.context(), left, right));
    };

    RuleOper subOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *right = context.popCommand();
        NodeCommand *left = context.popCommand();
        context.pushCommand(new SubCommand(token.context(), left, right));
    };

    RuleOper retrieveVarOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new RetrieveVarCommand(token.context(), token.token()));
    };

    RuleOper assignIdOper = [](CompilerContext &context, const Token &token)
    {
                        if (token.type()== TokenType::identifierTokenType) {
        context.pushCommand(new AssignCommand(token.context(), token.token()));
                        } else {
                            throw token.context().parseException("Identifier token expected ()" + token.token());
                        } };

    RuleOper assignValueOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *value = context.popCommand();
        AssignCommand *assign = (AssignCommand *)(context.popCommand());
        context.pushCommand(&assign->add(value));
    };

    RuleOper clearOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new ClearCommand(token.context()));
    };

    RuleOper functionIdOper = [](CompilerContext &context, const Token &token)
    {
        context.pushCommand(new CallFunctionCommand(token.context(), token.token()));
    };

    RuleOper argOper = [](CompilerContext &context, const Token &token)
    {
        NodeCommand *arg = context.popCommand();
        CallFunctionCommand *func = (CallFunctionCommand *)context.popCommand();
        context.pushCommand(&func->add(arg));
    };

    RuleOper funcCheckOper = [](CompilerContext &context, const Token &token)
    {
        CallFunctionCommand *func = (CallFunctionCommand *)context.popCommand();
        const int actualArgs = func->commands().size();

        const FunctionDef &def = QU_PROCESSOR_FUNCTIONS.at(func->id());
        const int requiredArgs = def.numArgs();

        if (actualArgs != requiredArgs)
        {
            const string id = func->id();
            delete func;
            throw token.context().parseException(
                id + " requires " + std::to_string(requiredArgs) + " arguments: actual (" + std::to_string(actualArgs) +
                ")");
        }
        context.pushCommand(func);
    };

    return &(*compiler)
                .add("<code-unit-head>", codeUnitHeadOper)
                .add("<clear-stm>", clearOper)
                .add("<function-id>", functionIdOper)
                .add("<arg>", argOper)
                .add("<arg-tail>", argOper)
                .add("<function>", funcCheckOper)
                .add("<assign-var-identifier>", assignIdOper)
                .add("<assign-stm>", assignValueOper)
                .add("<var-identifier>", retrieveVarOper)
                .add("<multiply-tail>", mulOper)
                .add("<multiply-tail-star>", mulStarOper)
                .add("<divide-tail>", divOper)
                .add("<plus-tail>", addOper)
                .add("<minus-tail>", subOper)
                .add("<cross-tail-opt>", crossOper)
                .add("<negate-exp>", negOper)
                .add("^", daggerOper)
                .add("<bra>", daggerOper)
                .add("<plus-state>", plusStateOper)
                .add("<minus-state>", minusStateOper)
                .add("<im-state>", imStateOper)
                .add("<minus-im-state>", minusImStateOper)
                .add("<int-state>", int2StateOper)
                .add("<stm>", stmOper)
                .add("<im-unit>", imOper)
                .add("pi", piOper)
                .add("e", eOper)
                .add("<real-literal>", realOper)
                .add("<int-literal>", intOper);
}

Compiler::~Compiler()
{
    for (const NodeCommand *node : _stack)
    {
        delete node;
    }
}

Compiler &Compiler::add(const std::string &rule, std::function<void(CompilerContext &, const Token &)> &op)
{
    _opers[rule] = op;
    return *this;
}

CompilerContext &Compiler::pushCommand(NodeCommand *command)
{
    _stack.push_back(command);
    return *this;
}

void Compiler::join(const Token &token, const SyntaxRule &rule)
{
    const string &ruleId = rule.id();

    if (_opers.count(ruleId) > 0)
    {
        auto &op = _opers.at(ruleId);
        op(*this, token);
    }
}

NodeCommand *Compiler::popCommand(void)
{
    assert(!_stack.empty());
    NodeCommand *cmd = _stack.back();
    _stack.pop_back();
    return cmd;
}
