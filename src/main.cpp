#include <iostream>
#include <sstream>
#include <regex>

#include "qubit.h"
#include "qugates.h"
#include "parser.h"

using namespace std;
using namespace qb;
using namespace qg;
using namespace qp;
using namespace vu;

int main(const int argc, const char **argv)
{
     //     const gates_t gates = parseStream(cin);
     const gates_t gates = argc > 1
                               ? parseArgs(argc, argv)
                               : parseStream(cin);

     cout << "Gates:" << endl;
     for (const QuGate gate : gates)
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
