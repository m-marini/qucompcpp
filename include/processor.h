#ifndef _processor_h_
#define _processor_h_

#include <string>
#include <map>
#include <functional>

#include "values.h"
#include "processContext.h"

namespace qc
{
    typedef std::function<const Value *(const SourceContext &, const ListValue &)> FunctionMapper;

    class FunctionDef
    {
        std::string _id;
        int _numArgs;
        const FunctionMapper _mapper;

    public:
        FunctionDef(const std::string &id, const int numArgs, const FunctionMapper mapper) : _id(id), _numArgs(numArgs), _mapper(mapper) {}

        const std::string &id(void) const
        {
            return _id;
        }
        const int numArgs(void) const
        {
            return _numArgs;
        }

        const FunctionMapper &mapper(void) const { return _mapper; }
    };

    class Processor : public ProcessContext
    {
        std::map<std::string, const Value *> _variables;

    public:
        ~Processor();
        
        const std::map<std::string, const Value *> &variables(void) { return _variables; }

        virtual const Value *int2Ket(const SourceContext &source, const Value *arg) override;
        virtual const Value *dagger(const SourceContext &source, const Value *arg) override;
        virtual const Value *neg(const SourceContext &source, const Value *arg) override;
        virtual const Value *assign(const SourceContext &source, const std::string &id, const Value *arg) override;
        virtual const Value *retrieveVar(const SourceContext &source, const std::string &id) override;
        virtual const Value *callFunction(const SourceContext &source, const std::string &id, const Value *args) override;
        virtual const Value *cross(const SourceContext &source, const Value *left, const Value *right) override;
        virtual const Value *mul(const SourceContext &source, const Value *left, const Value *right) override;
        virtual const Value *mulStar(const SourceContext &source, const Value *left, const Value *right) override;
        virtual const Value *div(const SourceContext &source, const Value *left, const Value *right) override;
        virtual const Value *add(const SourceContext &source, const Value *left, const Value *right) override;
        virtual const Value *sub(const SourceContext &source, const Value *left, const Value *right) override;
        virtual const Value *clear(const SourceContext &source) override;
    };

    extern const std::map<std::string, qc::FunctionDef>
        QU_PROCESSOR_FUNCTIONS;
}

#endif
