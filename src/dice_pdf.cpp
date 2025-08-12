#include <cmath>
#include <random>
#include <algorithm>
#include <vector>
#include <iostream>
#include "dice_pdf.hpp"

using namespace Eigen;

static std::random_device rd;
static std::mt19937 rand_gen(rd());

Eigen::RowVectorXd convolve(const Eigen::RowVectorXd& a, const Eigen::RowVectorXd& b)
{
    int n = a.size() + b.size() - 1;
    Eigen::RowVectorXd result = Eigen::RowVectorXd::Zero(n);

    for (int i = 0; i < a.size(); ++i)
        for (int j = 0; j < b.size(); ++j)
            result[i + j] += a[i] * b[j];

    return result;
}

/**
 * CONSTRUCTORS
 */
DicePDF::DicePDF(int numsides)
{
    RowVectorXd probs = RowVectorXd::Constant(numsides, 1.0/numsides);
    RowVectorXd rolls = RowVectorXd::LinSpaced(numsides, 1, numsides);
    pdf.resize(2, numsides);
    pdf.row(prob_idx) = probs;
    pdf.row(roll_idx) = rolls;
}

DicePDF::DicePDF(const Eigen::Matrix2Xd& m) : pdf(m)
{
    return;
}

/**
 * COMPARISON OPERATOR OVERLOADS
 */
double operator>(double scalar, const DicePDF& rhs)
{
    return (rhs < scalar);
}

double DicePDF::operator>(double scalar) const
{
    double result = 0.0;
    for(int i=0; i < pdf.row(roll_idx).size(); ++i)
    {
        if(pdf.row(roll_idx)[i] > scalar)
            result += pdf.row(prob_idx)[i];
    }
    return result;
}

double operator<(double scalar, const DicePDF& rhs)
{
    return (rhs > scalar);
}

double DicePDF::operator<(double scalar) const
{
    return 1-((*this) >= scalar);
}

double operator>=(double scalar, const DicePDF& rhs)
{
    return (rhs <= scalar);
}

double DicePDF::operator>=(double scalar) const
{
    double result = 0.0;
    for(int i=0; i < pdf.row(roll_idx).size(); ++i)
    {
        if(pdf.row(roll_idx)[i] >= scalar)
            result += pdf.row(prob_idx)[i];
    }
    return result;
}

double operator<=(double scalar, const DicePDF& rhs)
{
    return (rhs >= scalar);
}

double DicePDF::operator<=(double scalar) const
{
    return 1-((*this) > scalar);
}

double operator==(double scalar, const DicePDF& rhs)
{
    return (rhs == scalar);
}

double DicePDF::operator==(double scalar) const
{
    for(int i=0; i < pdf.row(roll_idx).size(); ++i)
    {
        if(pdf.row(roll_idx)[i] == scalar)
            return pdf.row(prob_idx)[i];
    }
    return 0.0;
}

double operator!=(double scalar, const DicePDF& rhs)
{
    return (rhs != scalar);
}

double DicePDF::operator!=(double scalar) const
{
    return 1-((*this) == scalar);
}

/**
 * ARITHMETIC OPERATOR OVERLOADS
 */
DicePDF DicePDF::operator+() const
{
    return *this;
}

DicePDF DicePDF::operator-() const
{
    DicePDF d(*this);
    d.pdf.row(roll_idx) *= -1;
    d.pdf = d.pdf.rowwise().reverse().eval();
    return d;
}

DicePDF DicePDF::operator+(const DicePDF& rhs) const
{
    RowVectorXd new_probs = convolve(pdf.row(prob_idx), rhs.pdf.row(prob_idx));
    new_probs /= new_probs.sum(); //ensure probability equals 1.0

    double min_roll = pdf.row(roll_idx).minCoeff() + rhs.pdf.row(roll_idx).minCoeff();
    double max_roll = pdf.row(roll_idx).maxCoeff() + rhs.pdf.row(roll_idx).maxCoeff();
    RowVectorXd new_rolls = RowVectorXd::LinSpaced(new_probs.size(), min_roll, max_roll);

    Matrix2Xd result(2, new_probs.size());
    result.row(prob_idx) = new_probs;
    result.row(roll_idx) = new_rolls;
    return DicePDF(result);
}

DicePDF DicePDF::operator+(double scalar) const
{
    DicePDF d(*this);
    d.pdf.row(roll_idx) = d.pdf.row(roll_idx).array() + scalar;
    return d;
}

DicePDF operator+(double scalar, const DicePDF& rhs) {
    return rhs + scalar;
}

DicePDF DicePDF::operator-(const DicePDF& rhs) const
{
    return (*this)+(-rhs);
}

DicePDF DicePDF::operator-(double scalar) const
{
    return (*this) + (-scalar);
}

DicePDF operator-(double scalar, const DicePDF& rhs) {
    return -rhs + scalar;
}

DicePDF DicePDF::operator*(double scalar) const
{
    DicePDF d(*this);
    d.pdf.row(roll_idx) = d.pdf.row(roll_idx).array()*scalar;
    return d;
}

DicePDF operator*(double scalar, const DicePDF& rhs)
{
    return rhs*scalar;
}

/**
 * STATISTICS FUNCTIONS
 */
double DicePDF::minimum() const
{
    return pdf.row(roll_idx)[0];
}

double DicePDF::maximum() const
{
    return pdf.row(roll_idx)[pdf.row(roll_idx).size()-1];
}

double DicePDF::expected_value() const
{
    double ev = 0.0;
    for(int i=0; i<pdf.row(prob_idx).size(); ++i)
    {
        ev += pdf.row(prob_idx)[i]*pdf.row(roll_idx)[i];
    }
    return ev;
}

double DicePDF::variance() const
{
    double ev = expected_value();
    double var = 0.0;
    for(int i=0; i<pdf.row(prob_idx).size(); ++i)
    {
        var += pdf.row(prob_idx)[i]*std::pow((ev - pdf.row(roll_idx)[i]), 2);
    }
    return var;
}

double DicePDF::standard_dev() const
{
    return std::sqrt(variance());
}

/**
 * ELEMENT ACCESS
 */
RowVectorXd DicePDF::get_probs() const
{
    return pdf.row(prob_idx);
}

RowVectorXd DicePDF::get_rolls() const
{
    return pdf.row(roll_idx);
}

Matrix2Xd DicePDF::get_pdfmatrix() const
{
    return pdf;
}

/**
 * RANDOM SELECTION
 */
double DicePDF::roll() const
{
    std::discrete_distribution<> weighted_distr(pdf.row(prob_idx).begin(), pdf.row(prob_idx).end());
    return pdf.row(roll_idx)[weighted_distr(rand_gen)];
}

/**
 * MULTIPLE DICE FACTORY METHODS
 */
DicePDF nds_distribution(int numdice, int numsides)
{
    DicePDF ret_dice(numsides);
    for(int i=0; i<numdice-1; ++i)
        ret_dice = ret_dice + DicePDF(numsides);
    return ret_dice;
}

DicePDF max_distribution(int numdice, int numsides)
{
    RowVectorXd probs = RowVectorXd::Zero(numsides);
    RowVectorXd rolls = RowVectorXd::LinSpaced(probs.size(), 1, numsides);
    double px = 0.0;
    double pxprev = 0.0;
    for(int i=1; i<numsides+1; ++i)
    {
        px = std::pow(i, numdice)-pxprev;
        probs[i-1] = px/std::pow(numsides, numdice);
        pxprev += px;
    }
    Matrix2Xd result(2, probs.size());
    result.row(prob_idx) = probs;
    result.row(roll_idx) = rolls;
    return DicePDF(result);
}

DicePDF min_distribution(int numdice, int numsides)
{
    RowVectorXd probs = RowVectorXd::Zero(numsides);
    RowVectorXd rolls = RowVectorXd::LinSpaced(probs.size(), 1, numsides);
    double px = 0.0;
    double pxprev = std::pow(numsides, numdice);
    for(int i=numsides-1; i>=0; --i)
    {
        px = std::pow(i, numdice)-pxprev;
        probs[numsides-i-1] = -px/std::pow(numsides, numdice);
        pxprev += px;
    }
    Matrix2Xd result(2, probs.size());
    result.row(prob_idx) = probs;
    result.row(roll_idx) = rolls;
    return DicePDF(result);
}

static std::vector<std::vector<int>> accel_asc(int n)
{
    std::vector<std::vector<int>> result;
    std::vector<int> a(n + 1, 0);
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
            result.emplace_back(a.begin(), a.begin() + k + 2);
            x += 1;
            y -= 1;
        }
        a[k] = x + y;
        y = x + y - 1;
        result.emplace_back(a.begin(), a.begin() + k + 1);
    }
    return result;
}

static std::vector<std::vector<int>> get_valid_integer_partitions(int n, int part_len, int max_val, int min_val)
{
    auto partitions = accel_asc(n);
    for(auto it = partitions.cbegin(); it != partitions.cend(); )
    {
        if(it->size() != part_len ||
          (*std::max_element(it->cbegin(), it->cend())) > max_val ||
          (*std::min_element(it->cbegin(), it->cend())) < min_val)
            it = partitions.erase(it);
        else
            ++it;
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

long int factorial(int n)
{
    long f = 1;
    for(int i=1; i<=n; ++i)
        f*=i;
    return f;
}

static DicePDF compound_distribution(int numdice, int totaldice, int numsides, bool is_max)
{
    Matrix2Xd pdf = nds_distribution(numdice, numsides).get_pdfmatrix();
    for(int i=0; i<pdf.row(roll_idx).size(); ++i)
    {
        double total_permutations = 0.0;
        std::vector<std::vector<int>> roll_list;
        int roll_value = static_cast<int>(pdf.row(roll_idx)[i]);
        auto int_partitions = get_valid_integer_partitions(roll_value, numdice, numsides, 1);
        for(auto partition : int_partitions)
        {
            std::vector<std::vector<int>> temp_list;
            extend_partition(partition, temp_list, totaldice, numsides, is_max);
            roll_list.insert(roll_list.end(), temp_list.begin(), temp_list.end());
        }
        for(auto roll : roll_list)
        {
            int product = 1;
            for(int d=1; d<= numsides; ++d)
                product *= factorial(std::count(roll.cbegin(), roll.cend(), d));
            total_permutations += factorial(totaldice)/static_cast<double>(product);
        }
        pdf.row(prob_idx)[i] = static_cast<double>(total_permutations)/std::pow(numsides, totaldice);
    }
    return DicePDF(pdf);
}

DicePDF compound_max_distribution(int numdice, int totaldice, int numsides)
{
    return compound_distribution(numdice, totaldice, numsides, true);
}

DicePDF compound_min_distribution(int numdice, int totaldice, int numsides)
{
    return compound_distribution(numdice, totaldice, numsides, false);
}
