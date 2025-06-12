#ifndef _processor_h_
#define _processor_h_

#include <string>
#include <map>

#include "values.h"
#include "processContext.h"

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

    class Processor : public ProcessContext
    {
        std::map<std::string, const Value *> _variables;

    public:
        virtual const Value *int2Ket(const SourceContext &source, const Value *arg) override;
        virtual const Value *dagger(const SourceContext &source, const Value *arg) override;
        virtual const Value *neg(const SourceContext &source, const Value *arg) override;
        virtual const Value *assign(const SourceContext &source, const std::string &id, const Value *arg) override;
        virtual const Value *retrieveVar(const SourceContext &source, const std::string &id) override;
    };

    extern const std::map<std::string, qc::FunctionDef>
        QU_PROCESSOR_FUNCTIONS;
}

#endif
