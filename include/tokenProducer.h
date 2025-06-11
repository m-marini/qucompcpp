#ifndef _tokenProducer_h_
#define _tokenProducer_h_

#include "token.h"

namespace qc
{
    class TokenProducer
    {
    public:
        virtual void popToken(void) = 0;
        virtual const Token &currentToken(void) const = 0;
    };
}

#endif
