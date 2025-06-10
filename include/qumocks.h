#ifndef _qumocks_h_
#define _qumocks_h_

#include <string>

#include "tokenizer.h"
#include "syntaxBuilder.h"
#include "commands.h"

namespace qm
{
    class MockParseContext : public qc::ParseContext
    {
        std::vector<std::pair<const qc::Token *, const qc::SyntaxRule *>> _joins;

    public:
        ~MockParseContext();
        virtual void join(const qc::Token &token, const qc::SyntaxRule &rule) override;
        std::vector<std::pair<const qc::Token *, const qc::SyntaxRule *>> &joins(void) { return _joins; }
    };
}

#endif