
#include "commands.h"

using namespace std;
using namespace qc;
using namespace mx;





CompositeCommand::~CompositeCommand()
{
    for (auto *cmd : _commands)
    {
        delete cmd;
    }
}
std::ostream &CompositeCommand::write(std::ostream &stream) const
{
    stream << "(";
    bool first = true;
    for (const auto *cmd : _commands)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            stream << ",";
        }
        cmd->write(stream);
    }
    return stream << ")";
}

std::ostream &ListCommand::write(std::ostream &stream) const
{
    stream << "list";
    return CompositeCommand::write(stream);
}

std::ostream &Int2StateCommand::write(std::ostream &stream) const
{
    stream << "i2s";
    return CompositeCommand::write(stream);
}

std::ostream &DaggerCommand::write(std::ostream &stream) const
{
    stream << "^";
    return CompositeCommand::write(stream);
}

std::ostream &NegateCommand::write(std::ostream &stream) const
{
    stream << "neg";
    return CompositeCommand::write(stream);
}

std::ostream &CrossCommand::write(std::ostream &stream) const
{
    stream << "x";
    return CompositeCommand::write(stream);
}

std::ostream &AddCommand::write(std::ostream &stream) const
{
    stream << "add";
    return CompositeCommand::write(stream);
}

std::ostream &SubCommand::write(std::ostream &stream) const
{
    stream << "sub";
    return CompositeCommand::write(stream);
}

std::ostream &MultiplyCommand::write(std::ostream &stream) const
{
    stream << "mul";
    return CompositeCommand::write(stream);
}

std::ostream &DivideCommand::write(std::ostream &stream) const
{
    stream << "div";
    return CompositeCommand::write(stream);
}

std::ostream &AssignCommand::write(std::ostream &stream) const
{
    stream << "let(" << _id << ",";
    return CompositeCommand::write(stream) << ")";
}

std::ostream &CallFunctionCommand::write(std::ostream &stream) const
{
    stream << _id;
    return CompositeCommand::write(stream);
}

const string to_string(const NodeCommand &command)
{
    stringstream stream;
    command.write(stream);
    return stream.str();
}

ostream &operator<<(ostream &stream, const NodeCommand &cmd)
{
    return cmd.write(stream);
}
