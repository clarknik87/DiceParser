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

/*
TEST(DicePMF, roll)
{
    auto chi_square_test_distr = [](DicePMF d){
        // Chi Square Table Column for p=0.05
        // each entry corresponds to a degree 
        // of freedom, 1, 2, 3, ...
        const double chi_table[] = {
            5.0240,
            7.3780,
            9.3480,
            11.143,
            12.833,
            14.449,
            16.013,
            17.535,
            19.023,
            20.483,
            21.920,
            23.337,
            24.736,
            26.119,
            27.488,
            28.845,
            30.191,
            31.526,
            32.852,
            34.170,
            35.479,
            36.781,
            38.076,
            39.364,
            40.646,
            41.923,
            43.195,
            44.461,
            45.722,
            46.979,
        };
        constexpr int total_rolls = 100000;
        std::map<double, int> observed;
        for(int i=0; i<total_rolls; ++i)
        {
            double r = d.roll();
            observed[r] += 1;
        }

        int df = d.get_pmf().size()-1; //degrees of freedom
        ASSERT_GT(df, 0);
        ASSERT_LT(df, sizeof(chi_table)/sizeof(double));
        double chi_squared = 0.0;
        for(const auto& [roll,prob] : d.get_pmf())
        {
            if(observed.contains(roll))
                chi_squared += std::pow(observed[roll]-(prob*total_rolls),2)/(prob*total_rolls);
            else
                chi_squared += std::pow(0.0-(prob*total_rolls),2)/(prob*total_rolls);
        }
        EXPECT_LE(chi_squared, chi_table[df-1]) << df;
    };

    // single die
    chi_square_test_distr(DicePMF(2));
    chi_square_test_distr(DicePMF(4));
    chi_square_test_distr(DicePMF(6));
    chi_square_test_distr(DicePMF(8));
    chi_square_test_distr(DicePMF(10));
    chi_square_test_distr(DicePMF(12));
    chi_square_test_distr(DicePMF(20));

    // nds
    chi_square_test_distr(nds_distr(2,2));
    chi_square_test_distr(nds_distr(2,6));
    chi_square_test_distr(nds_distr(3,8));

    // min/max
    chi_square_test_distr(max_distr(2,6));
    chi_square_test_distr(min_distr(2,6));

    // compound ctor
    chi_square_test_distr(compound_max_distr(2,3,6));
    chi_square_test_distr(compound_min_distr(2,3,6));
}
*/

TEST(DIcePMF, arithmetic)
{
    // unary operations
    EXPECT_NEAR((+DicePMF(1)).expected_value(), 1, float_epsilon);
    EXPECT_NEAR((-DicePMF(1)).expected_value(), -1, float_epsilon);

    // scalar arithmetic
    EXPECT_NEAR((DicePMF(1)+1).expected_value(), 2, float_epsilon);
    EXPECT_NEAR((DicePMF(1)-1).expected_value(), 0, float_epsilon);
    EXPECT_NEAR((DicePMF(1)*1).expected_value(), 1, float_epsilon);
    EXPECT_NEAR((DicePMF(1)/1).expected_value(), 1, float_epsilon);

    EXPECT_NEAR((1+DicePMF(1)).expected_value(), 2, float_epsilon);
    EXPECT_NEAR((1-DicePMF(1)).expected_value(), 0, float_epsilon);
    EXPECT_NEAR((1*DicePMF(1)).expected_value(), 1, float_epsilon);
    EXPECT_NEAR((1/DicePMF(1)).expected_value(), 1, float_epsilon);

    EXPECT_NEAR((2+scalar_distr(2)).expected_value(), 4, float_epsilon);
    EXPECT_NEAR((2-scalar_distr(2)).expected_value(), 0, float_epsilon);
    EXPECT_NEAR((2*scalar_distr(2)).expected_value(), 4, float_epsilon);
    EXPECT_NEAR((4/scalar_distr(2)).expected_value(), 2, float_epsilon);

    EXPECT_NEAR((0.5+scalar_distr(-3)).expected_value(), -2.5, float_epsilon);
    EXPECT_NEAR((0.5-scalar_distr(-3)).expected_value(), 3.5, float_epsilon);
    EXPECT_NEAR((0.5*scalar_distr(-3)).expected_value(), -1.5, float_epsilon);
    EXPECT_NEAR((0.5/scalar_distr(-4)).expected_value(), -0.125, float_epsilon);
}
