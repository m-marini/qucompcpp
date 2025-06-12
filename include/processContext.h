#ifndef _processContext_h_
#define _processContext_h_

#include "values.h"
#include "sourceContext.h"

namespace qc
{
    class ProcessContext
    {
    public:
        virtual const Value *int2Ket(const SourceContext &source, const Value *arg) = 0;
        virtual const Value *dagger(const SourceContext &source, const Value *arg) = 0;
        virtual const Value *neg(const SourceContext &source, const Value *arg) = 0;
        virtual const Value *assign(const SourceContext &source, const std::string &id, const Value *arg) = 0;
        virtual const Value *retrieveVar(const SourceContext &source, const std::string &id) = 0;
    };
}

#endif
