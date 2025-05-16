#include <iostream>
#include <sstream>

#include "qubit.h"
#include "qugates.h"
#include "parser.h"

using namespace std;
using namespace qb;
using namespace qg;
using namespace qp;
using namespace vu;

#define LOG_ENTRY(s) cout << endl \
                          << "### Entry " << s << " ###" << endl
#define LOG_EXIT(s) cout << "### Exit " << s << " ###" << endl

static void testGates(void)
{
     Matrix swap01 = QuGate::swap(0, 1).build(3);
     for (int i = 0; i < 8; i++)
     {
          Ket t = swap01 * Ket::base(i, 3);
          cout << "swap(0,1) |" << i << "> -> " << t << endl;
     }

     QuGate i(QuGate::i(2));
     QuGate h(QuGate::h(1));
     QuGate swap(QuGate::swap(0, 1));
     gates_t gates({i, h, swap});
     cout << endl;
     cout << "ports:" << endl;
     for (const QuGate gate : gates)
     {
          cout << gate << endl;
     }

     cout << endl;
     cout << "number of qubits = " << numQuBits(gates) << endl;
     Matrix circuit(buildCircuit(gates));

     cout << "circuit = " << endl
          << circuit << endl;

     cout << endl;
     for (int i = 0; i < 8; i++)
     {
          Ket t = circuit * Ket::base(i, 3);

          cout << "circuit |" << i << "> -> " << t << endl;
          for (size_t i = 0; i < 3; i++)
          {
               cout << "b" << i << "=" << t.bitProb(i) << " ";
          }
          cout << endl;
     }
}

static void testEngine(void)
{
     cout << "|0> = " << Ket::zero << endl
          << "|1> = " << Ket::one << endl
          << "|+> = " << Ket::plus << endl
          << "|-> = " << Ket::minus << endl
          << "|i> = " << Ket::i << endl
          << "|-i> = " << Ket::minus_i << endl
          << "|2> = " << Ket::base(2, 2) << endl;

     cout << endl;
     cout << "|0>: b0=" << (Ket::zero).bitProb(0) << endl
          << "|1>: b0=" << (Ket::one).bitProb(0) << endl
          << "|+>: b0=" << (Ket::plus).bitProb(0) << endl
          << "|->: b0=" << (Ket::minus).bitProb(0) << endl
          << "|0>: b0=" << (Ket::base(0, 2)).bitProb(0) << ", b1=" << (Ket::base(0, 2)).bitProb(1) << endl
          << "|1>: b0=" << (Ket::base(1, 2)).bitProb(0) << ", b1=" << (Ket::base(1, 2)).bitProb(1) << endl
          << "|2>: b0=" << (Ket::base(2, 2)).bitProb(0) << ", b1=" << (Ket::base(2, 2)).bitProb(1) << endl
          << "|3>: b0=" << (Ket::base(3, 2)).bitProb(0) << ", b1=" << (Ket::base(3, 2)).bitProb(1) << endl;

     cout << endl;
     cout << "<0| = " << Bra::zero << endl
          << "<1| = " << Bra::one << endl
          << "<+| = " << Bra::plus << endl
          << "<-| = " << Bra::minus << endl
          << "<i| = " << Bra::i << endl
          << "<-i| = " << Bra::minus_i << endl
          << "<3| = " << Bra::base(3, 2) << endl;

     cout << endl;

     cout << "-|i> = " << (-Ket::i) << endl
          << "|+> 2 = " << (Ket::plus * 2.0f) << endl
          << "|+> i = " << (Ket::plus * 1if) << endl
          << "2 |+> = " << (2.0f * Ket::plus) << endl
          << "i |+> = " << (1if * Ket::plus) << endl;

     cout << endl;

     Ket ket1(vector{complex<float>(sqrt(2) / 2), complex<float>(0.0)});
     auto ket2 = ket1 + ket1;

     cout << "ket1 = " << ket1 << endl
          << "ket2 = " << ket2 << endl
          << "ket1 - ket1 = " << ket1 - ket1 << endl;

     cout << "ket1.norm = " << ket1.norm() << endl
          << "ket2.norm = " << ket2.norm() << endl
          << "(ket1+ket2).norm = " << (ket1 + ket2).norm() << endl;

     cout << endl;

     cout << "|i>* = " << (Ket::i).conj() << endl
          << "<i|* = " << (Bra::i).conj() << endl;

     cout << endl;

     cout << "<0|i> = " << Bra::zero * Ket::i << endl;

     cout << endl;

     cout << "|00> " << (Ket::zero * Ket::zero) << endl
          << "|01> " << (Ket::zero * Ket::one) << endl
          << "|10> " << (Ket::one * Ket::zero) << endl
          << "|11> " << (Ket::one * Ket::one) << endl;

     cout << endl;

     cout << "<00| " << (Bra::zero * Bra::zero) << endl
          << "<01| " << (Bra::zero * Bra::one) << endl
          << "<10| " << (Bra::one * Bra::zero) << endl
          << "<11| " << (Bra::one * Bra::one) << endl;

     cout << endl;

     cout << "I=" << endl
          << Matrix::i << endl;

     cout << endl;

     cout << "permutation(3,2,1,0) = " << endl
          << Matrix::permute({3, 2, 1, 0}) << endl;

     cout << endl;
     const Matrix split = Matrix::permute({1, 0});
     cout << "split |i> = " << (split * Ket::i) << endl;
     cout << "split <i| = " << (Bra::i * split) << endl;

     cout << endl;
     cout << "split x split = " << (split * split) << endl;

     cout << endl;
     cout << "x =" << endl
          << Matrix::x << endl
          << "y =" << endl
          << Matrix::y << endl
          << "z =" << endl
          << Matrix::z << endl
          << "h =" << endl
          << Matrix::h << endl
          << "s =" << endl
          << Matrix::s << endl
          << "t =" << endl
          << Matrix::t << endl
          << "swap =" << endl
          << Matrix::swap << endl
          << "cnot =" << endl
          << Matrix::cnot << endl
          << "ccnot =" << endl
          << Matrix::ccnot << endl;

     cout << endl;
     cout << "I x X = " << endl
          << cross(Matrix::i, Matrix::x) << endl;
}

void testToken()
{
     cout << "testToken()" << endl;
     istringstream stream(string("   i ( 8) \n") + "a // asdasdasda\n" + "b /* skjadajhsdjksahdkja */ c\n" + "\n");
     Tokenizer tok(stream);
     for (int i = 0; !tok.eof(); i++)
     {
          cout << "read " << tok.currentToken().value_or("EOF") << endl
               << "line " << tok.tokenLine().value_or("EOF") << endl
               << "@" << tok.tokenLineNum() << ":" << tok.tokenPos() << endl;
          tok.popToken();
          cout << "pop" << endl
               << endl;
     }
}

void testParser()
{
     auto gates = parseString("i(1)\nswap(2,1) /* asdasd*/ h(0)x(0)y(0)z(0)\n\ns(0)t(0)cnot(1,0)ccnot(1,2,3)");
     cout << endl;
     cout << "Gates:" << endl;
     for (const QuGate gate : gates)
     {
          cout << gate << endl;
     }
     gates = parseString("   ");
}