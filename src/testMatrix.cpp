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