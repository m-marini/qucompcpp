#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <utility>
#include <tuple>
#include <vector>

#include "matrix.h"

#define HALF_SQRT2 (sqrt(2) / 2)

using namespace std;
using namespace mx;
using namespace vu;

TEST(testMatrix, fmt)
{
    EXPECT_EQ("0", mx::fmt(0));
    EXPECT_EQ("1", mx::fmt(1));
    EXPECT_EQ("1.2", mx::fmt(1.2));
    EXPECT_EQ("i", mx::fmt(1i));
    EXPECT_EQ("-i", mx::fmt(-1i));
    EXPECT_EQ("1 +i", mx::fmt({1, 1}));
    EXPECT_EQ("-1 -i", mx::fmt({-1, -1}));
    EXPECT_EQ("1.1 +1.2 i", mx::fmt({1.1, 1.2}));
    EXPECT_EQ("-1.1 -1.2 i", mx::fmt({-1.1, -1.2}));
    EXPECT_EQ("0.7071067811865476 +0.7071067811865476 i", mx::fmt({HALF_SQRT2, HALF_SQRT2}));
}

TEST(testMatrix, tostring)
{
    EXPECT_EQ("|0>", to_string(ketBase(0)));
    EXPECT_EQ("|3>", to_string(ketBase(3)));
    EXPECT_EQ("|0> + |3>", to_string(ketBase(3) + ketBase(0)));
    EXPECT_EQ("|0> + (1.2 i) |3>", to_string(ketBase(3) * 1.2i + ketBase(0)));
    EXPECT_EQ("<0|", to_string(ketBase(0).dagger()));
    EXPECT_EQ("<3|", to_string(ketBase(3).dagger()));
    EXPECT_EQ("<0| + <3|", to_string((ketBase(3) + ketBase(0)).dagger()));
    EXPECT_EQ("<0| + (-1.2 i) <3|", to_string((ketBase(3) * 1.2i + ketBase(0)).dagger()));
    EXPECT_EQ("[ 0, -1\n  1,  0 ]", to_string(eps(0, 1)));
}

TEST(testMatrix, create)
{
    Matrix a(2, 2,
             {1.0, 1.0,
              1.0, 1.0});
    EXPECT_EQ(2, a.numCols());
    EXPECT_EQ(2, a.numRows());
    EXPECT_EQ(vector<complex<double>>({1.0, 1.0,
                                       1.0, 1.0}),
              a.cells());
}

TEST(testMatrix, reference)
{
    Matrix a(2, 2,
             {1.0, 1.0,
              1.0, 1.0});
    const Matrix &b = a;

    EXPECT_EQ(2, b.numCols());
    EXPECT_EQ(2, b.numRows());
    EXPECT_EQ(vector<complex<double>>({1.0, 1.0,
                                       1.0, 1.0}),
              b.cells());
}

TEST(testMatrix, copy)
{
    Matrix a(2, 2,
             {1.0, 1.0,
              1.0, 1.0});
    const Matrix &b = a;

    Matrix c(b);

    EXPECT_EQ(2, c.numCols());
    EXPECT_EQ(2, c.numRows());
    EXPECT_EQ(vector<complex<double>>({1.0, 1.0,
                                       1.0, 1.0}),
              c.cells());
}

TEST(testMatrix, cross)
{
    Matrix a(2, 2,
             {1, 2,
              3, 4});
    Matrix b(2, 2,
             {2, 3,
              4, 5});
    Matrix exp(4, 4,
               {2, 3, 4, 6,
                4, 5, 8, 10,
                6, 9, 8, 12,
                12, 15, 16, 20});
    Matrix c = a.cross(b);

    EXPECT_EQ(4, c.numCols());
    EXPECT_EQ(4, c.numRows());
    EXPECT_EQ(to_string(exp), to_string(c));
}

TEST(testMatrix, divideComplex)
{
    const Matrix a(2, 2,
                   {1, 2,
                    3, 4});
    const Matrix exp(2, 2,
                     {0.5, 1,
                      1.5, 2});
    const Matrix c = a / 2.0;

    EXPECT_EQ(to_string(exp), to_string(c));
}

TEST(testMatrix, extendsRows)
{
    const Matrix a(2, 2,
                   {1, 2,
                    3, 4});
    const Matrix exp(4, 2,
                     {1, 2,
                      3, 4,
                      0, 0,
                      0, 0});
    const Matrix result = a.extendsRows(4);
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, extendsCols)
{
    const Matrix a(2, 2,
                   {1, 2,
                    3, 4});
    const Matrix exp(2, 4,
                     {1, 2, 0, 0,
                      3, 4, 0, 0});
    const Matrix result = a.extendsCols(4);
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, extends0)
{
    const Matrix a(2, 2,
                   {1, 2,
                    3, 4});
    const Matrix exp(4, 4,
                     {1, 2, 0, 0,
                      3, 4, 0, 0,
                      0, 0, 0, 0,
                      0, 0, 0, 0});
    const Matrix result = a.extends0(4, 4);
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, sim01)
{
    const Matrix exp(2, 2,
                     {0, 1,
                      1, 0});
    const Matrix result = sim(0, 1);
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, sim0)
{
    const Matrix exp(2, 2,
                     {1, 0,
                      0, 0});
    const Matrix result = sim(0, 0);
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, eps01)
{
    const Matrix exp(2, 2,
                     {0, -1,
                      1, 0});
    const Matrix result = eps(0, 1);
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, eps10)
{
    const Matrix exp(2, 2,
                     {0, -1,
                      1, 0});
    const Matrix result = eps(1, 0);
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, eps00)
{
    const Matrix exp(2, 2,
                     {0, 0,
                      0, 0});
    const Matrix result = eps(0, 0);
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, permute01)
{
    const Matrix exp(2, 2,
                     {1, 0,
                      0, 1});
    const Matrix result = permute({0, 1});
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, permute10)
{
    const Matrix exp(2, 2,
                     {0, 1,
                      1, 0});
    const Matrix result = permute({1, 0});
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, permute0123)
{
    const Matrix exp(4, 4,
                     {1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1});
    const Matrix result = permute({0, 1, 2, 3});
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, permute3210)
{
    const Matrix exp(4, 4,
                     {0, 0, 0, 1,
                      0, 0, 1, 0,
                      0, 1, 0, 0,
                      1, 0, 0, 0});
    const Matrix result = permute({3, 2, 1, 0});
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, permute0213)
{
    const Matrix exp(4, 4,
                     {1, 0, 0, 0,
                      0, 0, 1, 0,
                      0, 1, 0, 0,
                      0, 0, 0, 1});
    const Matrix result = permute({0, 2, 1, 3});
    EXPECT_EQ(to_string(exp), to_string(result));
}

TEST(testMatrix, ARY)
{
    Matrix exp(2, 4,
               {0, 0, 1, 0,
                0, 0, 0, 0});
    Matrix c = ary(0, 2);

    EXPECT_EQ(to_string(exp), to_string(c));
}

TEST(testMatrix, X0)
{
    Matrix exp(2, 2,
               {0, 1,
                1, 0});
    Matrix c = X(0);

    EXPECT_EQ(to_string(exp), to_string(c));
}

static const Matrix X1(4, 4,
                       {0, 0, 1, 0,
                        0, 0, 0, 1,
                        1, 0, 0, 0,
                        0, 1, 0, 0});

TEST(testMatrix, X1)
{
    Matrix c = X(1);

    EXPECT_EQ(to_string(X1), to_string(c));
}

static const Matrix Y0(2, 2,
                       {0, {0, -1}, 1i, 0});

static const Matrix Y1(4, 4,
                       {0, 0, {0, -1}, 0, 0, 0, 0, {0, -1}, 1i, 0, 0, 0, 0, 1i, 0, 0});
TEST(testMatrix, Y0)
{
    Matrix c = Y(0);

    EXPECT_EQ(to_string(Y0), to_string(c));
}
TEST(testMatrix, Y1)
{
    Matrix c = Y(1);

    EXPECT_EQ(to_string(Y1), to_string(c));
}

static const Matrix Z0(2, 2,
                       {1, 0,
                        0, -1});

static const Matrix Z1(4, 4,
                       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, -1, 0,
                        0, 0, 0, -1});

TEST(testMatrix, Z0)
{
    Matrix c = Z(0);

    EXPECT_EQ(to_string(Z0), to_string(c));
}
TEST(testMatrix, Z1)
{
    Matrix c = Z(1);

    EXPECT_EQ(to_string(Z1), to_string(c));
}

static const Matrix H0(2, 2,
                       {HALF_SQRT2, HALF_SQRT2,
                        HALF_SQRT2, -HALF_SQRT2});

static const Matrix H1(4, 4,
                       {HALF_SQRT2, 0, HALF_SQRT2, 0,
                        0, HALF_SQRT2, 0, HALF_SQRT2,
                        HALF_SQRT2, 0, -HALF_SQRT2, 0,
                        0, HALF_SQRT2, 0, -HALF_SQRT2});

TEST(testMatrix, H0)
{
    Matrix c = H(0);

    EXPECT_EQ(to_string(H0), to_string(c));
}

TEST(testMatrix, H1)
{
    Matrix c = H(1);

    EXPECT_EQ(to_string(H1), to_string(c));
}

static const Matrix S0(2, 2,
                       {1, 0,
                        0, 1i});

static const Matrix S1(4, 4,
                       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1i, 0,
                        0, 0, 0, 1i});

TEST(testMatrix, S0)
{
    Matrix c = S(0);

    EXPECT_EQ(to_string(S0), to_string(c));
}

TEST(testMatrix, S1)
{
    Matrix c = S(1);

    EXPECT_EQ(to_string(S1), to_string(c));
}

static const Matrix T0(2, 2,
                       {1, 0, 0, {HALF_SQRT2, HALF_SQRT2}});

static const Matrix T1(4, 4,
                       {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, {HALF_SQRT2, HALF_SQRT2}, 0, 0, 0, 0, {HALF_SQRT2, HALF_SQRT2}});

TEST(testMatrix, T0)
{
    Matrix c = T(0);

    EXPECT_EQ(to_string(T0), to_string(c));
}

TEST(testMatrix, T1)
{
    Matrix c = T(1);

    EXPECT_EQ(to_string(T1), to_string(c));
}

TEST(testMatrix, CNOTError)
{
    EXPECT_THROW({ CNOT(0, 0); }, invalid_argument);
}

//------------------------------------

const Matrix QUBIT0_01(2, 2,
                       {1, 0,
                        0, 0});

TEST(testMatrix, qubit0_01)
{
    Matrix c = qubit0(0, 1);

    EXPECT_EQ(to_string(QUBIT0_01), to_string(c));
}

const Matrix QUBIT0_02(4, 4,
                       {1, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0});

TEST(testMatrix, qubit0_02)
{
    Matrix c = qubit0(0, 2);

    EXPECT_EQ(to_string(QUBIT0_02), to_string(c));
}

const Matrix QUBIT0_12(4, 4,
                       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0});

TEST(testMatrix, qubit0_12)
{
    Matrix c = qubit0(1, 2);

    EXPECT_EQ(to_string(QUBIT0_12), to_string(c));
}

const Matrix QUBIT0_03(8, 8,
                       {1, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 1, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 1, 0,
                        0, 0, 0, 0, 0, 0, 0, 0});

TEST(testMatrix, qubit0_03)
{
    Matrix c = qubit0(0, 3);

    EXPECT_EQ(to_string(QUBIT0_03), to_string(c));
}

const Matrix QUBIT0_13(8, 8,
                       {1, 0, 0, 0, 0, 0, 0, 0,
                        0, 1, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 0, 0, 0,
                        0, 0, 0, 0, 0, 1, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0});

TEST(testMatrix, qubit0_13)
{
    Matrix c = qubit0(1, 3);

    EXPECT_EQ(to_string(QUBIT0_13), to_string(c));
}

const Matrix QUBIT0_23(8, 8,
                       {1, 0, 0, 0, 0, 0, 0, 0,
                        0, 1, 0, 0, 0, 0, 0, 0,
                        0, 0, 1, 0, 0, 0, 0, 0,
                        0, 0, 0, 1, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0});

TEST(testMatrix, qubit0_23)
{
    Matrix c = qubit0(2, 3);

    EXPECT_EQ(to_string(QUBIT0_23), to_string(c));
}

//------------------------------------

const Matrix QUBIT1_01(2, 2,
                       {0, 0,
                        0, 1});

TEST(testMatrix, qubit1_01)
{
    Matrix c = qubit1(0, 1);

    EXPECT_EQ(to_string(QUBIT1_01), to_string(c));
}

const Matrix QUBIT1_02(4, 4,
                       {0, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 1});

TEST(testMatrix, qubit1_02)
{
    Matrix c = qubit1(0, 2);

    EXPECT_EQ(to_string(QUBIT1_02), to_string(c));
}

const Matrix QUBIT1_12(4, 4,
                       {0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1});

TEST(testMatrix, qubit1_12)
{
    Matrix c = qubit1(1, 2);

    EXPECT_EQ(to_string(QUBIT1_12), to_string(c));
}

const Matrix QUBIT1_03(8, 8,
                       {0, 0, 0, 0, 0, 0, 0, 0,
                        0, 1, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 1, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 1, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 1});

TEST(testMatrix, qubit1_03)
{
    Matrix c = qubit1(0, 3);

    EXPECT_EQ(to_string(QUBIT1_03), to_string(c));
}

const Matrix QUBIT1_13(8, 8,
                       {0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 1, 0, 0, 0, 0, 0,
                        0, 0, 0, 1, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 1, 0,
                        0, 0, 0, 0, 0, 0, 0, 1});

TEST(testMatrix, qubit1_13)
{
    Matrix c = qubit1(1, 3);

    EXPECT_EQ(to_string(QUBIT1_13), to_string(c));
}

const Matrix QUBIT1_23(8, 8,
                       {0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 0, 0, 0,
                        0, 0, 0, 0, 0, 1, 0, 0,
                        0, 0, 0, 0, 0, 0, 1, 0,
                        0, 0, 0, 0, 0, 0, 0, 1});

TEST(testMatrix, qubit1_23)
{
    Matrix c = qubit1(2, 3);

    EXPECT_EQ(to_string(QUBIT1_23), to_string(c));
}

//------------------------------------------

class AddFixture : public testing::TestWithParam<std::vector<Matrix>>
{
};

TEST_P(AddFixture, testAdd)
{
    auto &params = GetParam();
    const Matrix &a = params.at(0);
    const Matrix &b = params.at(1);
    const Matrix &exp = params.at(2);
    const Matrix c = a + b;

    EXPECT_EQ(to_string(exp), to_string(c));
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         AddFixture,
                         testing::Values(
                             // Code, expected error
                             vector<Matrix>{
                                 Matrix(3, 3,
                                        {1, 2, 3,
                                         4, 5, 6,
                                         7, 8, 9}),
                                 Matrix(2, 2,
                                        {1, 2,
                                         3, 4}),
                                 Matrix(3, 3,
                                        {2, 4, 3,
                                         7, 9, 6,
                                         7, 8, 9})},

                             vector<Matrix>{
                                 Matrix(2, 3,
                                        {1, 2, 3,
                                         4, 5, 6}),
                                 Matrix(2, 2,
                                        {2, 3,
                                         4, 5}),
                                 Matrix(2, 3,
                                        {3, 5, 3,
                                         8, 10, 6})},

                             vector<Matrix>{
                                 Matrix(3, 2,
                                        {1, 2,
                                         3, 4,
                                         5, 6}),
                                 Matrix(2, 2,
                                        {2, 3,
                                         4, 5}),
                                 Matrix(3, 2,
                                        {3, 5,
                                         7, 9,
                                         5, 6})},

                             vector<Matrix>{
                                 Matrix(2, 2,
                                        {1, 2,
                                         3, 4}),
                                 Matrix(2, 2,
                                        {2, 3,
                                         4, 5}),
                                 Matrix(2, 2,
                                        {3, 5,
                                         7, 9})},

                             vector<Matrix>{
                                 Matrix(2, 2,
                                        {1, 2,
                                         3, 4}),
                                 Matrix(2, 3,
                                        {2, 3, 4,
                                         5, 6, 7}),
                                 Matrix(2, 3,
                                        {3, 5, 4,
                                         8, 10, 7})},

                             vector<Matrix>{
                                 Matrix(2, 2,
                                        {1, 2,
                                         3, 4}),
                                 Matrix(3, 2,
                                        {2, 3,
                                         4, 5,
                                         6, 7}),
                                 Matrix(3, 2,
                                        {3, 5,
                                         7, 9,
                                         6, 7})}));
//---------------------------

class MulStarFixture : public testing::TestWithParam<std::vector<Matrix>>
{
};
TEST_P(MulStarFixture, testMulStar)
{
    auto &params = GetParam();
    const Matrix &a = params.at(0);
    const Matrix &b = params.at(1);
    const Matrix &exp = params.at(2);
    const Matrix c = a * b;

    EXPECT_EQ(to_string(exp), to_string(c));
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         MulStarFixture,
                         testing::Values(
                             // Code, expected error
                             vector<Matrix>{Matrix(2, 2,
                                                   {1, 2,
                                                    3, 4}),
                                            Matrix(2, 2,
                                                   {4, 3,
                                                    2, 1}),
                                            Matrix(2, 2,
                                                   {8, 5,
                                                    20, 13})},
                             vector<Matrix>{Matrix(2, 2,
                                                   {1, 2,
                                                    3, 4}),
                                            Matrix(4, 4,
                                                   {1, 2, 3, 4,
                                                    2, 3, 4, 5,
                                                    3, 4, 5, 6,
                                                    4, 5, 6, 7}),
                                            Matrix(4, 4,
                                                   {5, 8, 11, 14,
                                                    11, 18, 25, 32,
                                                    11, 14, 17, 20,
                                                    25, 32, 39, 46})}));
//---------------------------

class MulFixture : public testing::TestWithParam<std::vector<Matrix>>
{
};
TEST_P(MulFixture, testMultiply)
{
    auto &params = GetParam();
    const Matrix &a = params.at(0);
    const Matrix &b = params.at(1);
    const Matrix &exp = params.at(2);
    const Matrix c = a.multiply(b);

    EXPECT_EQ(to_string(exp), to_string(c));
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         MulFixture,
                         testing::Values(
                             // Code, expected error
                             vector<Matrix>{Matrix(2, 2,
                                                   {1, 2,
                                                    3, 4}),
                                            Matrix(2, 2,
                                                   {4, 3,
                                                    2, 1}),
                                            Matrix(2, 2,
                                                   {8, 5,
                                                    20, 13})},
                             vector<Matrix>{Matrix(2, 2,
                                                   {1, 2,
                                                    3, 4}),
                                            Matrix(4, 4,
                                                   {1, 2, 3, 4,
                                                    2, 3, 4, 5,
                                                    3, 4, 5, 6,
                                                    4, 5, 6, 7}),
                                            Matrix(2, 4,
                                                   {5, 8, 11, 14,
                                                    11, 18, 25, 32})},
                             vector<Matrix>{Matrix(4, 4,
                                                   {1, 2, 3, 4,
                                                    2, 3, 4, 5,
                                                    3, 4, 5, 6,
                                                    4, 5, 6, 7}),
                                            Matrix(2, 2,
                                                   {1, 2,
                                                    3, 4}),
                                            Matrix(4, 2,
                                                   {7, 10,
                                                    11, 16,
                                                    15, 22,
                                                    19, 28})}));

//-------------------------------

class ExtCrossFixture : public testing::TestWithParam<std::tuple<Matrix, int, Matrix>>
{
};

TEST_P(ExtCrossFixture, testExtCross)
{
    const auto &[a, n, exp] = GetParam();
    const Matrix c = a.extendsCross(n);

    EXPECT_EQ(to_string(exp), to_string(c));
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         ExtCrossFixture,
                         testing::Values(
                             // Code, expected error
                             tuple<Matrix, int, Matrix>{Matrix(2, 2,
                                                               {1, 2,
                                                                3, 4}),
                                                        2,
                                                        Matrix(2, 2,
                                                               {1, 2,
                                                                3, 4})},
                             tuple<Matrix, int, Matrix>{Matrix(2, 2,
                                                               {1, 2,
                                                                3, 4}),
                                                        4,
                                                        Matrix(4, 4,
                                                               {1, 2, 0, 0,
                                                                3, 4, 0, 0,
                                                                0, 0, 1, 2,
                                                                0, 0, 3, 4})}));

//-------------------------------

class BitsPermFixture : public testing::TestWithParam<pair<indices_t, indices_t>>
{
};

TEST_P(BitsPermFixture, computeBitsPermutation)
{
    const auto &[perm, exp] = GetParam();
    const indices_t c = mx::computeBitsPermutation(perm);

    EXPECT_EQ(exp, c);
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         BitsPermFixture,
                         testing::Values(
                             // Code, expected error
                             pair<indices_t, indices_t>{{0, 2}, {0, 2, 1}},
                             pair<indices_t, indices_t>{{0, 3}, {0, 3, 2, 1}},
                             pair<indices_t, indices_t>{{1, 0}, {1, 0}},

                             pair<indices_t, indices_t>{{1, 2}, {2, 0, 1}},    //  0->2, 1->0, 2->1
                             pair<indices_t, indices_t>{{1, 3}, {3, 0, 2, 1}}, // 0->3, 1->0, 2->2, 3->1
                             pair<indices_t, indices_t>{{2, 0}, {1, 2, 0}},
                             pair<indices_t, indices_t>{{2, 1}, {2, 1, 0}},

                             pair<indices_t, indices_t>{{2, 3}, {2, 3, 0, 1}},
                             pair<indices_t, indices_t>{{3, 0}, {1, 3, 2, 0}}, // inp[0] = 1, inp[1] = 3, inp[2] = 2, inp[3] = 0
                             pair<indices_t, indices_t>{{3, 1}, {3, 1, 2, 0}}, // inp[0] = 3, inp[1] = 1, inp[2] = 2, inp[3] = 0
                             pair<indices_t, indices_t>{{3, 2}, {2, 3, 1, 0}}, // inp[0] = 2, inp[1] = 3, inp[2] = 1, inp[3] = 0
                             pair<indices_t, indices_t>{{0, 1}, {0, 1}}));

//-------------------------------

class StatePermFixture : public testing::TestWithParam<pair<indices_t, indices_t>>
{
};

TEST_P(StatePermFixture, computeStatePermutation)
{
    const auto &[perm, exp] = GetParam();
    const indices_t c = mx::computeStatePermutation(perm);

    EXPECT_EQ(to_string(exp), to_string(c));
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         StatePermFixture,
                         testing::Values(
                             // Code, expected error
                             pair<indices_t, indices_t>{{0, 1, 2}, {0, 1, 2, 3, 4, 5, 6, 7}},
                             pair<indices_t, indices_t>{{1, 0, 2}, {0, 2, 1, 3, 4, 6, 5, 7}},
                             pair<indices_t, indices_t>{{2, 1, 0}, {0, 4, 2, 6, 1, 5, 3, 7}},
                             pair<indices_t, indices_t>{{1, 2, 0}, {0, 2, 4, 6, 1, 3, 5, 7}}));

//-------------------------------

class CNotFixture : public testing::TestWithParam<tuple<indices_t, Matrix, Matrix>>
{
};

TEST_P(CNotFixture, cnot)
{
    const auto &[bits, ket, exp] = GetParam();
    const Matrix gate = mx::CNOT(bits[0], bits[1]);
    const Matrix result = gate * ket;

    EXPECT_EQ(to_string(exp), to_string(result));
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         CNotFixture,
                         testing::Values(
                             // Code, expected error
                             tuple<indices_t, Matrix, Matrix>{{0, 1}, ketBase(0), ketBase(0).extends0(4, 1)},
                             tuple<indices_t, Matrix, Matrix>{{0, 1}, ketBase(1), ketBase(1).extends0(4, 1)},
                             tuple<indices_t, Matrix, Matrix>{{0, 1}, ketBase(2), ketBase(3)},
                             tuple<indices_t, Matrix, Matrix>{{0, 1}, ketBase(3), ketBase(2)},
                             tuple<indices_t, Matrix, Matrix>{{1, 0}, ketBase(0), ketBase(0).extends0(4, 1)},
                             tuple<indices_t, Matrix, Matrix>{{1, 0}, ketBase(1), ketBase(3)},
                             tuple<indices_t, Matrix, Matrix>{{1, 0}, ketBase(2), ketBase(2)},
                             tuple<indices_t, Matrix, Matrix>{{1, 0}, ketBase(3), ketBase(1).extends0(4, 1)}));

//-------------------------------

class SwapFixture : public testing::TestWithParam<tuple<indices_t, Matrix, Matrix>>
{
};

TEST_P(SwapFixture, swap)
{
    const auto &[bits, ket, exp] = GetParam();
    const Matrix gate = mx::SWAP(bits[0], bits[1]);
    const Matrix result = gate * ket;

    EXPECT_EQ(to_string(exp), to_string(result));
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         SwapFixture,
                         testing::Values(
                             // Code, expected error
                             tuple<indices_t, Matrix, Matrix>{{0, 1}, {ketBase(0)}, {ketBase(0).extendsRows(4)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 1}, {ketBase(1)}, {ketBase(2).extendsRows(4)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 1}, {ketBase(2)}, {ketBase(1).extendsRows(4)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 1}, {ketBase(3)}, {ketBase(3).extendsRows(4)}},

                             tuple<indices_t, Matrix, Matrix>{{1, 2}, {ketBase(0)}, {ketBase(0).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2}, {ketBase(1)}, {ketBase(1).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2}, {ketBase(2)}, {ketBase(4).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2}, {ketBase(3)}, {ketBase(5).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{1, 2}, {ketBase(4)}, {ketBase(2).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2}, {ketBase(5)}, {ketBase(3).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2}, {ketBase(6)}, {ketBase(6).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2}, {ketBase(7)}, {ketBase(7).extendsRows(8)}}));

//-------------------------------

class CCNotFixture : public testing::TestWithParam<tuple<indices_t, Matrix, Matrix>>
{
};

TEST_P(CCNotFixture, ccnot)
{
    const auto &[bits, ket, exp] = GetParam();
    const Matrix gate = mx::CCNOT(bits.at(0), bits.at(1), bits.at(2));
    const Matrix result = gate * ket;

    EXPECT_EQ(to_string(exp), to_string(result));
}

INSTANTIATE_TEST_SUITE_P(testMatrix,
                         CCNotFixture,
                         testing::Values(
                             // Code, expected error

                             tuple<indices_t, Matrix, Matrix>{{0, 1, 2}, {ketBase(0)}, {ketBase(0).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 1, 2}, {ketBase(1)}, {ketBase(1).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 1, 2}, {ketBase(2)}, {ketBase(2).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 1, 2}, {ketBase(3)}, {ketBase(3).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{0, 1, 2}, {ketBase(4)}, {ketBase(4).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 1, 2}, {ketBase(5)}, {ketBase(5).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{0, 1, 2}, {ketBase(6)}, {ketBase(7).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{0, 1, 2}, {ketBase(7)}, {ketBase(6).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{0, 2, 1}, {ketBase(0)}, {ketBase(0).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 2, 1}, {ketBase(1)}, {ketBase(1).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 2, 1}, {ketBase(2)}, {ketBase(2).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 2, 1}, {ketBase(3)}, {ketBase(3).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{0, 2, 1}, {ketBase(4)}, {ketBase(4).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{0, 2, 1}, {ketBase(5)}, {ketBase(5).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{0, 2, 1}, {ketBase(6)}, {ketBase(7).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{0, 2, 1}, {ketBase(7)}, {ketBase(6).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{1, 0, 2}, {ketBase(0)}, {ketBase(0).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 0, 2}, {ketBase(1)}, {ketBase(1).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 0, 2}, {ketBase(2)}, {ketBase(2).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 0, 2}, {ketBase(3)}, {ketBase(3).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{1, 0, 2}, {ketBase(4)}, {ketBase(4).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{1, 0, 2}, {ketBase(5)}, {ketBase(7).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 0, 2}, {ketBase(6)}, {ketBase(6).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{1, 0, 2}, {ketBase(7)}, {ketBase(5).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{1, 2, 0}, {ketBase(0)}, {ketBase(0).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2, 0}, {ketBase(1)}, {ketBase(1).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2, 0}, {ketBase(2)}, {ketBase(2).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2, 0}, {ketBase(3)}, {ketBase(3).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{1, 2, 0}, {ketBase(4)}, {ketBase(4).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{1, 2, 0}, {ketBase(5)}, {ketBase(7).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{1, 2, 0}, {ketBase(6)}, {ketBase(6).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{1, 2, 0}, {ketBase(7)}, {ketBase(5).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{2, 0, 1}, {ketBase(0)}, {ketBase(0).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{2, 0, 1}, {ketBase(1)}, {ketBase(1).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{2, 0, 1}, {ketBase(2)}, {ketBase(2).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{2, 0, 1}, {ketBase(3)}, {ketBase(7).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{2, 0, 1}, {ketBase(4)}, {ketBase(4).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{2, 0, 1}, {ketBase(5)}, {ketBase(5).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{2, 0, 1}, {ketBase(6)}, {ketBase(6).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{2, 0, 1}, {ketBase(7)}, {ketBase(3).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{2, 1, 0}, {ketBase(0)}, {ketBase(0).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{2, 1, 0}, {ketBase(1)}, {ketBase(1).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{2, 1, 0}, {ketBase(2)}, {ketBase(2).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{2, 1, 0}, {ketBase(3)}, {ketBase(7).extendsRows(8)}},

                             tuple<indices_t, Matrix, Matrix>{{2, 1, 0}, {ketBase(4)}, {ketBase(4).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{2, 1, 0}, {ketBase(5)}, {ketBase(5).extendsRows(8)}},
                             tuple<indices_t, Matrix, Matrix>{{2, 1, 0}, {ketBase(6)}, {ketBase(6).extendsRows(8)}},
                             /**/ tuple<indices_t, Matrix, Matrix>{{2, 1, 0}, {ketBase(7)}, {ketBase(3).extendsRows(8)}}));
