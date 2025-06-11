#ifndef _compilerContext_h_
#define _compilerContext_h_

#include "commands.h"

namespace qc
{

    class CompilerContext
    {
    public:
        virtual CompilerContext &pushCommand(NodeCommand *command) = 0;
        virtual NodeCommand *popCommand(void) = 0;
    };
}
#endif
