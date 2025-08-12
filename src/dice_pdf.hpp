#ifndef DICE_PDF_HPP_GUARD
#define DICE_PDF_HPP_GUARD

#include <Eigen/Dense>

constexpr Eigen::Index prob_idx = 0;
constexpr Eigen::Index roll_idx = 1;

/**
 * A DicePDF stores the probability density function of a dice formula as a 2D
 * Eigen matrix. The second row stors the roll value, the first row stores the
 * asscoiated probability. A standard d4 would look like this:
 *
 *          [[0.25 0.25 0.25 0.25],
 *           [1    2    3    4]]
 */
class DicePDF
{
private:
    Eigen::Matrix2Xd pdf;
public:
    // Constructors
    DicePDF() = default;
    DicePDF(int numsides);
    explicit DicePDF(const Eigen::Matrix2Xd& m);

    // Comparison Operator overloads
    double operator>(double scalar) const;
    double operator<(double scalar) const;
    double operator>=(double scalar) const;
    double operator<=(double scalar) const;
    double operator==(double scalar) const;
    double operator!=(double scalar) const;

    // Arithmetic Operator overloads
    DicePDF operator+() const;
    DicePDF operator-() const;
    DicePDF operator+(const DicePDF& rhs) const;
    DicePDF operator+(double scalar) const;
    DicePDF operator-(const DicePDF& rhs) const;
    DicePDF operator-(double scalar) const;
    DicePDF operator*(double scalar) const;

    // Stats functions
    double minimum() const;
    double maximum() const;
    double expected_value() const;
    double variance() const;
    double standard_dev() const;

    // Element Access
    Eigen::RowVectorXd get_probs() const;
    Eigen::RowVectorXd get_rolls() const;
    Eigen::Matrix2Xd   get_pdfmatrix() const;

    // Random selection
    double roll() const;
};

double operator>(double scalar, const DicePDF& rhs);
double operator>=(double scalar, const DicePDF& rhs);
double operator<(double scalar, const DicePDF& rhs);
double operator<=(double scalar, const DicePDF& rhs);
double operator==(double scalar, const DicePDF& rhs);
double operator!=(double scalar, const DicePDF& rhs);

DicePDF operator+(double scalar, const DicePDF& rhs);
DicePDF operator-(double scalar, const DicePDF& rhs);
DicePDF operator*(double scalar, const DicePDF& rhs);

// Multiple Dice Factory Methods
DicePDF nds_distribution(int numdice, int numsides);
DicePDF max_distribution(int numdice, int numsides);
DicePDF min_distribution(int numdice, int numsides);
DicePDF compound_max_distribution(int numdice, int totaldice, int numsides);
DicePDF compound_min_distribution(int numdice, int totaldice, int numsides);

#endif//DICE_PDF_HPP_GUARD
