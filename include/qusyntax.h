#ifndef _quSyntax_h_
#define _quSyntax_h_

#include "syntaxBuilder.h"

namespace qc
{
    /**
     * Creates the qu syntax
     */
    class Syntax
    {
    public:
        /**
         * Returns the qu syntax
         */
        static const RuleMap build(void);
    };
}

#endif
