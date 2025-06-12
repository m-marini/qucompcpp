#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include "qubit.h"
#include "qugates.h"
#include "parser.h"
#include "matrix.h"

using namespace std;
using namespace mx;
using namespace vu;

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