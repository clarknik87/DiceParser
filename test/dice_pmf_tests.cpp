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

    // max ctor
    EXPECT_NEAR(max_distr(2,2).get_pmf().at(1.0), 0.25, float_epsilon);
    EXPECT_NEAR(max_distr(2,4).get_pmf().at(1.0), 0.0625, float_epsilon);
    EXPECT_NEAR(max_distr(2,6).get_pmf().at(1.0), 0.0278, float_epsilon);
    EXPECT_NEAR(max_distr(2,20).get_pmf().at(1.0), 0.0025, float_epsilon);

    // min ctor
    EXPECT_NEAR(min_distr(2,2).get_pmf().at(2.0), 0.25, float_epsilon);
    EXPECT_NEAR(min_distr(2,4).get_pmf().at(4.0), 0.0625, float_epsilon);
    EXPECT_NEAR(min_distr(2,6).get_pmf().at(6.0), 0.0278, float_epsilon);
    EXPECT_NEAR(min_distr(2,20).get_pmf().at(20.0), 0.0025, float_epsilon);

    // max(x,nds) ctor
    EXPECT_NEAR(compound_max_distr(2,3,2).get_pmf().at(2.0), 0.125, float_epsilon);
    EXPECT_NEAR(compound_max_distr(2,3,4).get_pmf().at(4.0), 0.1094, float_epsilon);
    EXPECT_NEAR(compound_max_distr(2,3,6).get_pmf().at(6.0), 0.0880, float_epsilon);
    EXPECT_NEAR(compound_max_distr(2,3,20).get_pmf().at(20.0), 0.0339, float_epsilon);

    // min(x,nds) ctor
    EXPECT_NEAR(compound_min_distr(2,3,2).get_pmf().at(2.0), 0.5, float_epsilon);
    EXPECT_NEAR(compound_min_distr(2,3,4).get_pmf().at(4.0), 0.25, float_epsilon);
    EXPECT_NEAR(compound_min_distr(2,3,6).get_pmf().at(6.0), 0.1574, float_epsilon);
    EXPECT_NEAR(compound_min_distr(2,3,20).get_pmf().at(20.0), 0.0410, float_epsilon);
}

TEST(DicePMF, stats)
{
    // minimum
    EXPECT_NEAR(nds_distr(2,2).minimum(), 2.0, float_epsilon);
    EXPECT_NEAR(nds_distr(3,4).minimum(), 3.0, float_epsilon);
    EXPECT_NEAR(nds_distr(4,6).minimum(), 4.0, float_epsilon);
    EXPECT_NEAR(nds_distr(5,8).minimum(), 5.0, float_epsilon);

    // maximum
    EXPECT_NEAR(nds_distr(2,2).maximum(), 4.0, float_epsilon);
    EXPECT_NEAR(nds_distr(3,4).maximum(), 12.0, float_epsilon);
    EXPECT_NEAR(nds_distr(4,6).maximum(), 24.0, float_epsilon);
    EXPECT_NEAR(nds_distr(5,8).maximum(), 40.0, float_epsilon);

    // expected value
    EXPECT_NEAR(nds_distr(2,2).expected_value(), 3.0, float_epsilon);
    EXPECT_NEAR(nds_distr(3,4).expected_value(), 7.5, float_epsilon);
    EXPECT_NEAR(nds_distr(4,6).expected_value(), 14.0, float_epsilon);
    EXPECT_NEAR(nds_distr(5,8).expected_value(), 22.5, float_epsilon);

    // variance
    EXPECT_NEAR(nds_distr(2,2).variance(), 0.5, float_epsilon);
    EXPECT_NEAR(nds_distr(3,4).variance(), 3.75, float_epsilon);
    EXPECT_NEAR(nds_distr(4,6).variance(), 11.6666, float_epsilon);
    EXPECT_NEAR(nds_distr(5,8).variance(), 26.25, float_epsilon);

    // standard dev
    EXPECT_NEAR(nds_distr(2,2).standard_dev(), 0.7071, float_epsilon);
    EXPECT_NEAR(nds_distr(3,4).standard_dev(), 1.9365, float_epsilon);
    EXPECT_NEAR(nds_distr(4,6).standard_dev(), 3.4157, float_epsilon);
    EXPECT_NEAR(nds_distr(5,8).standard_dev(), 5.1235, float_epsilon);
}
