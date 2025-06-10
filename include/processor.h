#ifndef _processor_h_
#define _processor_h_

#include <string>
#include <map>

#include "values.h"
#include "token.h"

namespace qc
{
    class FunctionDef
    {
        std::string _id;
        int _numArgs;

    public:
        FunctionDef(const std::string &id, const int numArgs) : _id(id), _numArgs(numArgs) {}

        const std::string &id(void) const { return _id; }
        const int numArgs(void) const { return _numArgs; }
    };

    class ProcessContext
    {
    public:
        virtual const Value *int2Ket(const SourceContext &source, const Value *arg) = 0;
    };

    class Processor : public ProcessContext
    {
    public:
        virtual const Value *int2Ket(const SourceContext &source, const Value *arg) override;
    };

    extern const std::map<std::string, qc::FunctionDef>
        QU_PROCESSOR_FUNCTIONS;
}

#endif
