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
    };
}

#endif
