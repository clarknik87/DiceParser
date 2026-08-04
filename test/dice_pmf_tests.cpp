#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

#include "unit_test.hpp"
#include "dice_pmf.hpp"
// #include "dice_parser/dice_parser.hpp"

TEST(DicePMF, valid_ctor)
{
    // scalar ctor
    EXPECT_EQ(scalar_distr(1.0).get_pmf().at(1.0), 1.0);
    EXPECT_EQ(scalar_distr(2.0).get_pmf().at(2.0), 1.0);
    EXPECT_EQ(scalar_distr(5.6).get_pmf().at(5.6), 1.0);

    // 1dn ctor
    EXPECT_EQ(DicePMF(1).get_pmf().at(1.0), 1.0/1.0);
    EXPECT_EQ(DicePMF(2).get_pmf().at(1.0), 1.0/2.0);
    EXPECT_EQ(DicePMF(4).get_pmf().at(1.0), 1.0/4.0);
    EXPECT_EQ(DicePMF(6).get_pmf().at(1.0), 1.0/6.0);
    EXPECT_EQ(DicePMF(8).get_pmf().at(1.0), 1.0/8.0);
    EXPECT_EQ(DicePMF(10).get_pmf().at(1.0), 1.0/10.0);
    EXPECT_EQ(DicePMF(20).get_pmf().at(1.0), 1.0/20.0);

    // nds ctor
    EXPECT_NEAR(nds_distr(2,2).get_pmf().at(2.0), 0.25, float_epsilon);
    EXPECT_NEAR(nds_distr(2,4).get_pmf().at(2.0), 0.0625, float_epsilon);
    EXPECT_NEAR(nds_distr(2,6).get_pmf().at(2.0), 0.0278, float_epsilon);
    EXPECT_NEAR(nds_distr(2,8).get_pmf().at(2.0), 0.0156, float_epsilon);

    return;
}