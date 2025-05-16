#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <regex>
#include <unistd.h>
#include <getopt.h>

#include "version.h"
#include "qubit.h"
#include "qugates.h"
#include "parser.h"

using namespace std;
using namespace qb;
using namespace qg;
using namespace qp;
using namespace vu;

static const struct option options[] = {
    {"gates", required_argument, 0, 'g'},
    {"file", required_argument, 0, 'f'},
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};
static char const *optString = "g:f:vh";

static void usage(const char *prog)
{
     cout << endl
          << DESCRIPTION << " " << VERSION << endl
          << endl
          << "Usage: " << prog << " [options]" << endl
          << endl
          << "Options" << endl
          << "  -f --file <file>        Load gates from file" << endl
          << "  -g --gates <gates_list> Define the gate list" << endl
          << "  -h --help               Print usage" << endl
          << "  -v --version            Print version" << endl
          << endl
          << "The stdin is used to load the gates if no file nor gates are defined."
          << endl;
}

static void printVersion(void)
{
     cout << VERSION << endl;
}

/**
 * Parse command arguments
 */
static const tuple<bool, optional<string>, optional<string>> parseArgs(int argc, char **argv)
{
     optional<string> gates = nullopt;
     optional<string> file = nullopt;
     int optIndex = 0;
     bool exit = false;
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
          case 'g':
               gates = optional(optarg);
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
     return {exit, file, gates};
}

int main(int argc, char **argv)
{
     bool exit;
     optional<string> file;
     optional<string> gatesArg;
     tie(exit, file, gatesArg) = parseArgs(argc, argv);

     if (exit)
     {
          return 0;
     }

     gates_t gates;
     if (file)
     {
          auto in = ifstream(file.value());
          gates = parseStream(in);
     }
     else if (gatesArg)
     {
          auto in = istringstream(gatesArg.value());
          gates = parseStream(in);
     }
     else
     {
          gates = parseStream(cin);
     }

     cout << "Gates:" << endl;
     for (const QuGate &gate : gates)
     {
          cout << "  " << gate << endl;
     }

     cout << "Building ..." << endl;
     cout.flush();

     Matrix circuit = buildCircuit(gates);
     const size_t n = numQuBits(gates);

     cout << "Processing ..." << endl;
     cout.flush();
     Ket output(circuit * Ket::base(0, n));

     cout << "Result:" << endl
          << "  " << output << endl
          << endl;
     for (size_t i = 0; i < n; i++)
     {
          cout << "  P(qubit " << i << ")=" << output.bitProb(i) << endl;
     }
}
