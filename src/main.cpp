#include <iostream>

#include "qubit.h"

using namespace std;
using namespace qb;
using namespace vu;

int main(const int argc, const char **argv)
{
    cout << "|0> = " << Ket::zero << endl
         << "|1> = " << Ket::one << endl
         << "|+> = " << Ket::plus << endl
         << "|-> = " << Ket::minus << endl
         << "|i> = " << Ket::i << endl
         << "|-i> = " << Ket::minus_i << endl
         << "|2> = " << Ket::base(2, 2) << endl;

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

    cout << "identity=" << endl
         << Matrix::identity << endl;

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
         << cross(Matrix::identity, Matrix::x) << endl;
}