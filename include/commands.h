#ifndef _commands_h_
#define _commands_h_

#include <complex>
#include <string>
#include <ostream>

#include "token.h"
#include "matrix.h"
#include "values.h"
#include "processor.h"

namespace qc
{

    class CommandVisitor;

    class NodeCommand
    {
        SourceContext _source;

    protected:
        NodeCommand(const SourceContext &source) : _source(source) {}

    public:
        const SourceContext &source(void) const { return _source; }

        virtual std::ostream &write(std::ostream &ostream) const = 0;

        virtual const Value *eval(ProcessContext &context) const { return NULL; }
    };

    class ClearCommand : public NodeCommand
    {
    public:
        ClearCommand(const SourceContext &source) : NodeCommand(source) {}
        virtual std::ostream &write(std::ostream &stream) const override
        {
            return stream << "clear";
        }

        virtual const Value *eval(ProcessContext &context) const
        {
            return context.clear(source());
            ;
        }
    };

    class IntCommand : public NodeCommand
    {
        int _value;

    public:
        IntCommand(const SourceContext &source, const int value) : NodeCommand(source), _value(value) {}
        const int value(void) const { return _value; }
        virtual std::ostream &write(std::ostream &stream) const override
        {
            return stream << _value;
        }

        virtual const IntValue *eval(ProcessContext &context) const override;
    };

    class ComplexCommand : public NodeCommand
    {
        std::complex<double> _value;

    public:
        ComplexCommand(const SourceContext &source, const std::complex<double> &value) : NodeCommand(source), _value(value) {}

        const std::complex<double> &value(void) const { return _value; }

        virtual std::ostream &write(std::ostream &stream) const override
        {
            return stream << _value;
        }

        virtual const ComplexValue *eval(ProcessContext &context) const override;
    };

    class MatrixCommand : public NodeCommand
    {
        mx::Matrix _value;

    public:
        MatrixCommand(const SourceContext &source, const mx::Matrix &value) : NodeCommand(source), _value(value) {}

        const mx::Matrix &value(void) const { return _value; }

        virtual std::ostream &write(std::ostream &stream) const override
        {
            return stream << _value;
        }

        virtual const MatrixValue *eval(ProcessContext &context) const override;
    };

    class RetrieveVarCommand : public NodeCommand
    {
        std::string _id;

    public:
        RetrieveVarCommand(const SourceContext &source, const std::string &id) : NodeCommand(source), _id(id) {}

        virtual std::ostream &write(std::ostream &stream) const override
        {
            return stream << _id;
        }

        virtual const Value *eval(ProcessContext &context) const override
        {
            return context.retrieveVar(source(), _id);
        }
    };

    class CompositeCommand : public NodeCommand
    {
    protected:
        std::vector<NodeCommand *> _commands;

    public:
        CompositeCommand(const SourceContext &source) : NodeCommand(source) {}
        ~CompositeCommand();
        CompositeCommand &add(NodeCommand *command)
        {
            _commands.push_back(command);
            return *this;
        }

        const std::vector<NodeCommand *> &commands(void) const { return _commands; }

        virtual std::ostream &write(std::ostream &stream) const override;
    };

    class ListCommand : public CompositeCommand
    {
    public:
        ListCommand(const SourceContext &source) : CompositeCommand(source) {}

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const ListValue *eval(ProcessContext &context) const override;
    };

    class CallFunctionCommand : public CompositeCommand
    {
        std::string _id;

    public:
        CallFunctionCommand(const SourceContext &source, const std::string &id) : CompositeCommand(source), _id(id) {}

        const std::string &id(void) const { return _id; }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override;
    };

    class Int2StateCommand : public CompositeCommand
    {
    public:
        Int2StateCommand(const SourceContext &source, NodeCommand *arg) : CompositeCommand(source)
        {
            add(arg);
        }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override
        {
            return context.int2Ket(source(), _commands.at(0)->eval(context));
        }
    };

    class DaggerCommand : public CompositeCommand
    {
    public:
        DaggerCommand(const SourceContext &source, NodeCommand *arg) : CompositeCommand(source)
        {
            add(arg);
        }
        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override
        {
            return context.dagger(source(), _commands.at(0)->eval(context));
        }
    };

    class NegateCommand : public CompositeCommand
    {
    public:
        NegateCommand(const SourceContext &source, NodeCommand *arg) : CompositeCommand(source)
        {
            add(arg);
        }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override
        {
            return context.neg(source(), _commands.at(0)->eval(context));
        }
    };

    class AssignCommand : public CompositeCommand
    {
        std::string _id;

    public:
        AssignCommand(const SourceContext &source, const std::string &id) : CompositeCommand(source), _id(id) {}

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override
        {
            return context.assign(source(), _id, _commands.at(0)->eval(context));
        }
    };

    class CrossCommand : public CompositeCommand
    {
    public:
        CrossCommand(const SourceContext &source, NodeCommand *left, NodeCommand *right) : CompositeCommand(source)
        {
            add(left).add(right);
        }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override;
    };

    class MultiplyCommand : public CompositeCommand
    {
    public:
        MultiplyCommand(const SourceContext &source, NodeCommand *left, NodeCommand *right) : CompositeCommand(source)
        {
            add(left).add(right);
        }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override;
    };

    class MultiplyStarCommand : public CompositeCommand
    {
    public:
        MultiplyStarCommand(const SourceContext &source, NodeCommand *left, NodeCommand *right) : CompositeCommand(source)
        {
            add(left).add(right);
        }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override;
    };

    class DivideCommand : public CompositeCommand
    {
    public:
        DivideCommand(const SourceContext &source, NodeCommand *left, NodeCommand *right) : CompositeCommand(source)
        {
            add(left).add(right);
        }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override;
    };

    class AddCommand : public CompositeCommand
    {
    public:
        AddCommand(const SourceContext &source, NodeCommand *left, NodeCommand *right) : CompositeCommand(source)
        {
            add(left).add(right);
        }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override;
    };

    class SubCommand : public CompositeCommand
    {
    public:
        SubCommand(const SourceContext &source, NodeCommand *left, NodeCommand *right) : CompositeCommand(source)
        {
            add(left).add(right);
        }

        virtual std::ostream &write(std::ostream &stream) const override;

        virtual const Value *eval(ProcessContext &context) const override;
    };
}

std::ostream &operator<<(std::ostream &stream, const qc::NodeCommand &cmd);
const std::string to_string(const qc::NodeCommand &command);

#endif
