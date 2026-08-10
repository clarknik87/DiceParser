#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include "dice_pmf.hpp"

/**
 * CONSTRUCTORS
 */
DicePMF::DicePMF(int numsides)
{
    for(int i=1; i<=numsides; ++i)
        pmf[i] = 1.0/numsides;
}

DicePMF::DicePMF(const std::map<double,double>& m) : pmf(m)
{
    return;
}

/**
 * ARITHMETIC OPERATOR OVERLOADS
 */
DicePMF DicePMF::operator+() const
{
    return unary_operate(unary_op::plus);
}

DicePMF DicePMF::operator-() const
{
    return unary_operate(unary_op::minus);
}

DicePMF DicePMF::operator+(double rhs) const
{
    DicePMF d;
    for(const auto& [roll,prob] : pmf)
        d.pmf[roll+rhs] = prob;
    return d;
}

DicePMF operator+(double lhs, const DicePMF& rhs) {
    return rhs+lhs;
}

DicePMF DicePMF::operator-(double rhs) const
{
    DicePMF d;
    for(const auto& [roll,prob] : pmf)
        d.pmf[roll-rhs] = prob;
    return d;
}
DicePMF operator-(double lhs, const DicePMF& rhs)
{
    return (-rhs)+lhs;
}

DicePMF DicePMF::operator*(double rhs) const
{
    DicePMF d;
    for(const auto& [roll,prob] : pmf)
        d.pmf[roll*rhs] = prob;
    return d;
}

DicePMF operator*(double lhs, const DicePMF& rhs)
{
    return rhs*lhs;
}

DicePMF DicePMF::operator/(double rhs) const
{
    DicePMF d;
    for(const auto& [roll,prob] : pmf)
        d.pmf[roll/rhs] = prob;
    return d;
}

DicePMF operator/(double lhs, const DicePMF& rhs)
{
    return DicePMF(lhs)/rhs;
}

DicePMF DicePMF::unary_operate(unary_op op) const
{
    // Operator look up table
    using one_arg_func_ptr = double (*)(double);
    std::map<unary_op, one_arg_func_ptr> func_lut{
        {unary_op::plus, [](double a){return a;}},
        {unary_op::minus, [](double a){return -a;}},
    };

    //Operate elementwise across the roll "vector"
    DicePMF d;
    for(const auto& [roll,prob] : pmf)
        d.pmf[func_lut[op](roll)] = prob;
    return d;
}

DicePMF DicePMF::binary_operate(const DicePMF& other, binary_op op) const
{
    // Operator look up table
    using two_arg_func_ptr = double (*)(double, double);
    std::map<binary_op, two_arg_func_ptr> func_lut{
        {binary_op::add, [](double a, double b){return a+b;}},
        {binary_op::sub, [](double a, double b){return a-b;}},
        {binary_op::mul, [](double a, double b){return a*b;}},
        {binary_op::div, [](double a, double b){return a/b;}},
    };

    // Operate and sum probabilites
    DicePMF d;
    for(const auto& [o_roll,o_prob] : other.pmf)
    {
        for(const auto& [i_roll, i_prob] : pmf)
        {
            double merged_roll = func_lut[op](i_roll,o_roll);
            if(d.pmf.contains(merged_roll))
                d.pmf[merged_roll] += i_prob*o_prob;
            else
                d.pmf[merged_roll] = i_prob*o_prob;
        }
    }
    return d;
}

DicePMF DicePMF::operator+(const DicePMF& rhs) const
{
    return binary_operate(rhs, binary_op::add);
}

DicePMF DicePMF::operator-(const DicePMF& rhs) const
{
    return binary_operate(rhs, binary_op::sub);
}

DicePMF DicePMF::operator*(const DicePMF& rhs) const
{
    return binary_operate(rhs, binary_op::mul);
}

DicePMF DicePMF::operator/(const DicePMF& rhs) const
{
    return binary_operate(rhs, binary_op::div);
}

/**
 * STATISTICS FUNCTIONS
 */
double DicePMF::minimum() const
{
    return pmf.cbegin()->first;
}

double DicePMF::maximum() const
{
    return (--pmf.cend())->first;
}

double DicePMF::expected_value() const
{
    double ev = 0.0;
    for(const auto& [roll, prob] : pmf)
        ev += prob*roll;
    return ev;
}

double DicePMF::variance() const
{
    const double ev = expected_value();
    double var = 0.0;
    for(const auto& [roll, prob] : pmf)
        var += prob*std::pow(ev - roll, 2);
    return var;
}

double DicePMF::standard_dev() const
{
    return std::sqrt(variance());
}

/**
 * ELEMENT ACCESS
 */
const std::map<double,double>& DicePMF::get_pmf()
{
    return pmf;
}

auto DicePMF::rolls_view() const
{
    return std::views::keys(pmf);
}

auto DicePMF::probs_view() const
{
    return std::views::values(pmf);
}

/**
 * RANDOM SELECTION
 */
static std::random_device rd;
static std::mt19937 rand_gen(rd());

double DicePMF::roll() const
{
    std::uniform_real_distribution<> distr(0.0, 1.0);
    double rn = distr(rand_gen);
    double total = 0.0;
    for(const auto& [roll, prob] : pmf)
    {
        total += prob;
        if(total >= rn)
            return roll;
    }
    return 0.0;
}

/**
 * DICE FACTORY METHODS
 */
DicePMF scalar_distr(double value)
{
    return DicePMF(1)*value;
}

DicePMF nds_distr(int numdice, int numsides)
{
    DicePMF ret_dice(numsides);
    for(int i=0; i<numdice-1; ++i)
        ret_dice = ret_dice + DicePMF(numsides);
    return ret_dice;
}

DicePMF max_distr(int numdice, int numsides)
{
    DicePMF ret_dice(numsides);
    double px = 0.0;
    double pxprev = 0.0;
    for(int i=1; i<=numsides; ++i)
    {
        px = std::pow(i, numdice)-pxprev;
        ret_dice.pmf.at(static_cast<double>(i)) = px/std::pow(numsides, numdice);
        pxprev += px;
    }
    return ret_dice;
}

DicePMF min_distr(int numdice, int numsides)
{
    DicePMF ret_dice(numsides);
    double px = 0.0;
    double pxprev = std::pow(numsides, numdice);
    for(int i=numsides-1; i>=0; --i)
    {
        px = std::pow(i, numdice)-pxprev;
        ret_dice.pmf.at(static_cast<double>(numsides-i)) = -px/std::pow(numsides, numdice);
        pxprev += px;
    }
    return ret_dice;
}

static std::vector<std::vector<int>> get_valid_integer_partitions(int n, int part_len, int max_val, int min_val)
{
    std::vector<std::vector<int>> partitions;
    std::vector<int> a(n + 1, 0);
    auto is_valid_partition = [&a, part_len, max_val, min_val](int size){
        return (size == part_len
                && (*std::max_element(a.begin(), a.begin()+size) <= max_val)
                && (*std::min_element(a.begin(), a.begin()+size) >= min_val)
            );
    };
    // accel_asc alogorithm that excludes invalid partitions
    int k = 1;
    int y = n - 1;
    while (k != 0) {
        int x = a[k - 1] + 1;
        k -= 1;
        while (2 * x <= y) {
            a[k] = x;
            y -= x;
            k += 1;
        }
        int l = k + 1;
        while (x <= y) {
            a[k] = x;
            a[l] = y;
            if(is_valid_partition(k+2))
                partitions.emplace_back(a.begin(), a.begin() + k + 2);
            x += 1;
            y -= 1;
        }
        a[k] = x + y;
        y = x + y - 1;
        if(is_valid_partition(k+1))
            partitions.emplace_back(a.begin(), a.begin() + k + 1);
    }
    return partitions;
}

static void extend_partition(std::vector<int> partition, std::vector<std::vector<int>>& roll_list, int totaldice, int numsides, bool is_max)
{
    if(partition.size() == totaldice)
    {
        roll_list.push_back(partition);
    }
    else
    {
        if(is_max)
        {
            for(int s=1; s<=(*std::min_element(partition.cbegin(), partition.cend())); ++s)
            {
                auto new_partition = partition;
                new_partition.push_back(s);
                extend_partition(new_partition, roll_list, totaldice, numsides, is_max);
            }
        }
        else
        {
            for(int s=(*std::max_element(partition.cbegin(), partition.cend())); s<=numsides; ++s)
            {
                auto new_partition = partition;
                new_partition.push_back(s);
                extend_partition(new_partition, roll_list, totaldice, numsides, is_max);
            }
        }
    }
}

static long int factorial(int n)
{
    long f = 1;
    for(int i=1; i<=n; ++i)
        f*=i;
    return f;
}

DicePMF compound_distr(int numdice, int totaldice, int numsides, bool is_max)
{
    auto pmf = nds_distr(numdice, numsides).pmf;
    for(auto& [roll,prob] : pmf)
    {
        double total_permutations = 0.0;
        std::vector<std::vector<int>> roll_list;
        auto int_partitions = get_valid_integer_partitions(roll, numdice, numsides, 1);
        for(auto partition : int_partitions)
        {
            std::vector<std::vector<int>> temp_list;
            extend_partition(partition, temp_list, totaldice, numsides, is_max);
            roll_list.insert(roll_list.end(), temp_list.begin(), temp_list.end());
        }
        for(auto roll_i : roll_list)
        {
            int product = 1;
            for(int d=1; d<= numsides; ++d)
                product *= factorial(std::count(roll_i.cbegin(), roll_i.cend(), d));
            total_permutations += factorial(totaldice)/static_cast<double>(product);
        }
        prob = static_cast<double>(total_permutations)/std::pow(numsides, totaldice);
    }
    return DicePMF(pmf);
}

DicePMF compound_max_distr(int numdice, int totaldice, int numsides)
{
    return compound_distr(numdice, totaldice, numsides, true);
}

DicePMF compound_min_distr(int numdice, int totaldice, int numsides)
{
    return compound_distr(numdice, totaldice, numsides, false);
}

/**
 * DEBUG/UTILITY METHODS
 */
std::ostream& operator<< (std::ostream& stream, const DicePMF& pmf)
{
    stream << std::setprecision(4);
    stream << "[";
    for(auto p : pmf.probs_view())
        stream << std::setw(8) << p;
    stream << "]\r\n[";
    for(auto r : pmf.rolls_view())
        stream << std::setw(8) << r;
    stream << "]";
    return stream;
}
