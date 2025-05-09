#include <iostream>

#include "qubit.h"

using namespace std;
using namespace ket;

int main(const int argc, const char **argv)
{
    cout << "|0> = " << ket::zero << endl;
    cout << "|1> = " << ket::one << endl;
    cout << "|+> = " << ket::plus << endl;
    cout << "|-> = " << ket::minus << endl;
    cout << "|i> = " << ket::i << endl;
    cout << "|-i> = " << ket::minus_i << endl;
    cout << "|2> = " << ket::base(2, 2) << endl;

    cout << endl;

    cout << "<0| = " << bra::zero << endl;
    cout << "<1| = " << bra::one << endl;
    cout << "<+| = " << bra::plus << endl;
    cout << "<-| = " << bra::minus << endl;
    cout << "<i| = " << bra::i << endl;
    cout << "<-i| = " << bra::minus_i << endl;

    cout << endl;

    cout << "-|i> = " << (-ket::i) << endl;
    cout << "|+> 2 = " << (ket::plus * 2.0f) << endl;
    cout << "|+> i = " << (ket::plus * 1if) << endl;
    cout << "2 |+> = " << (2.0f * ket::plus) << endl;
    cout << "i |+> = " << (1if * ket::plus) << endl;

    cout << endl;

    Ket ket1(vector{complex<float>(sqrt(2) / 2), complex<float>(0.0)});
    auto ket2 = ket1 + ket1;

    cout << "ket1 = " << ket1 << endl;
    cout << "ket2 = " << ket2 << endl;

    cout << "ket1.norm = " << ket1.norm() << endl;
    cout << "ket2.norm = " << ket2.norm() << endl;
    cout << "(ket1+ket2).norm = " << (ket1 + ket2).norm() << endl;

    cout << endl;

    cout << "|i>* = " << (ket::i).conj() << endl;
    cout << "<i|* = " << (bra::i).conj() << endl;

    cout << endl;

    cout << "<0|i> = " << bra::zero * ket::i << endl;
}