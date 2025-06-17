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
        virtual const Value *callFunction(const SourceContext &source, const std::string &id, const Value *args) = 0;
        virtual const Value *cross(const SourceContext &source, const Value *left, const Value *right) = 0;
        virtual const Value *mul(const SourceContext &source, const Value *left, const Value *right) = 0;
        virtual const Value *mulStar(const SourceContext &source, const Value *left, const Value *right) = 0;
        virtual const Value *div(const SourceContext &source, const Value *left, const Value *right) = 0;
        virtual const Value *add(const SourceContext &source, const Value *left, const Value *right) = 0;
        virtual const Value *sub(const SourceContext &source, const Value *left, const Value *right) = 0;
        virtual const Value *clear(const SourceContext &source) = 0;
    };
}

#endif
