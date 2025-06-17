#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <regex>
#include <unistd.h>
#include <getopt.h>

#include "version.h"
#include "tokenizer.h"
#include "compiler.h"
#include "qusyntax.h"
#include "values.h"

using namespace std;
using namespace qc;

static const struct option options[] = {
    {"dump", no_argument, 0, 'd'},
    {"file", required_argument, 0, 'f'},
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};
static char const *optString = "df:vh";

static void usage(const char *prog)
{
     cout << endl
          << DESCRIPTION << " " << VERSION << endl
          << endl
          << "Usage: " << prog << " [options]" << endl
          << endl
          << "Options" << endl
          << "  -d --dump               Specify variable dump" << endl
          << "  -f --file <file>        Specify qu source file" << endl
          << "  -h --help               Print usage" << endl
          << "  -v --version            Print version" << endl
          << endl;
}

static void printVersion(void)
{
     cout << VERSION << endl;
}

/**
 * Parse command arguments
 */
static const tuple<bool, bool, optional<string>> parseArgs(int argc, char **argv)
{
     optional<string> file = nullopt;
     int optIndex = 0;
     bool exit = false;
     bool dump = false;
     for (;;)
     {
          int option = getopt_long(argc, argv, optString, options, &optIndex);
          if (option == -1)
          {
               break;
          }
          switch (option)
          {
          case '?':
               // Error
               exit = true;
               break;
          case 'd':
               dump = true;
               break;
          case 'f':
               file = optional(optarg);
               break;
          case 'h':
               usage(argv[0]);
               exit = true;
               break;
          case 'v':
               printVersion();
               exit = true;
               break;
          default:
               cerr << "Unmanaged option " << (char)option << endl;
               exit = true;
               break;
          }
     }
     return {exit, dump, file};
}

int main(int argc, char **argv)
{
     bool exit;
     bool dump;
     optional<string> file;
     optional<string> gatesArg;
     tie(exit, dump, file) = parseArgs(argc, argv);

     if (exit)
     {
          return 0;
     }

     if (file)
     {
          auto in = ifstream(file.value());
     }

     cout << "Processing ..." << endl;
     cout.flush();
     ifstream source(file.value_or("qucomp.qu"));

     Tokenizer tokenizer(source);
     tokenizer.open();
     Compiler &compiler = *Compiler::createQu(tokenizer);
     RuleMap rules = Syntax::build();
     const SyntaxRule *rule = rules.map().at("<code-unit>");

     try
     {
          rule->parse(tokenizer, compiler);
          const NodeCommand *cmd = compiler.popCommand();
          Processor processor;
          const Value *result = cmd->eval(processor);

          for (const auto &v : ((const ListValue *)result)->values())
          {
               v->source().write(cout, "value: " + to_string(*v));
          }

          if (dump)
          {
               for (const auto &[id, v] : processor.variables())
               {
                    v->source().write(cout, "value: " + to_string(*v));
               }
          }

          delete result;
          delete cmd;
     }
     catch (QuSourceException ex)
     {
          cerr << ex << endl;
     }
     return 0;
}
