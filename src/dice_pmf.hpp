#ifndef DICE_PMF_HPP_GUARD
#define DICE_PMF_HPP_GUARD

#include <map>
#include <ranges>

/**
 * A DicePDF stores the probability density function of a dice formula. Though
 * stored internally as a std::map, the roll (key) and probability (value)
 * pairs can be conceptualized as a 2d matrix. The second row stores the roll
 * value, the first row stores the associated probability. A standard d4 can
 * be visualized as:
 *
 *      values ->   [[0.25 0.25 0.25 0.25],
 *      keys ->      [1    2    3    4]]
 */
class DicePMF
{
private:
    enum class unary_op{
        plus,
        minus,
        abs,
        ceil,
        floor,
        trunc,
        round,
        sqrt
    };
    enum class binary_op{
        add,
        sub,
        mul,
        div,
        pow   
    };

    std::map<double,double> pmf;
public:
    // Constructors
    DicePMF() = default;
    DicePMF(int numsides);
    explicit DicePMF(const std::map<double,double>& m);

    friend DicePMF scalar_distr(double value);
    friend DicePMF nds_distr(int numdice, int numsides);
    friend DicePMF max_distr(int numdice, int numsides);
    friend DicePMF min_distr(int numdice, int numsides);
    friend DicePMF compound_distr(int numdice, int totaldice, int numsides, bool is_max);
    friend DicePMF compound_max_distr(int numdice, int totaldice, int numsides);
    friend DicePMF compound_min_distr(int numdice, int totaldice, int numsides);

    // Arithmetic Operator overloads
    DicePMF operator+() const;
    DicePMF operator-() const;
    DicePMF operator+(double rhs) const;
    DicePMF operator-(double rhs) const;
    DicePMF operator*(double rhs) const;
    DicePMF operator/(double rhs) const;

    DicePMF unary_operate(unary_op op) const;
    DicePMF binary_operate(const DicePMF& other, binary_op op) const;
    DicePMF operator+(const DicePMF& rhs) const;
    DicePMF operator-(const DicePMF& rhs) const;
    DicePMF operator*(const DicePMF& rhs) const;
    DicePMF operator/(const DicePMF& rhs) const;

    // Comparison Operator overloads
    double operator>(double scalar) const;
    double operator<(double scalar) const;
    double operator>=(double scalar) const;
    double operator<=(double scalar) const;
    double operator==(double scalar) const;
    double operator!=(double scalar) const;

    double operator>(const DicePMF& rhs) const;
    double operator<(const DicePMF& rhs) const;
    double operator>=(const DicePMF& rhs) const;
    double operator<=(const DicePMF& rhs) const;
    double operator==(const DicePMF& rhs) const;
    double operator!=(const DicePMF& rhs) const;

    // Builtin functions
    DicePMF abs() const;
    DicePMF ceil() const;
    DicePMF floor() const;
    DicePMF trunc() const;
    DicePMF round() const;
    DicePMF sqrt() const;
    DicePMF pow(double rhs) const;
    DicePMF pow(const DicePMF& rhs) const;

    // Stats functions
    double minimum() const;
    double maximum() const;
    double expected_value() const;
    double variance() const;
    double standard_dev() const;

    // Element Access
    const std::map<double,double>& get_pmf();
    auto rolls_view() const;
    auto probs_view() const;

    // Random selection
    double roll() const;

    // Debug/Utility functions
    friend std::ostream& operator<< (std::ostream& stream, const DicePMF& pmf);
};

// Arithmetic Operator overloads
DicePMF operator+(double lhs, const DicePMF& rhs);
DicePMF operator-(double lhs, const DicePMF& rhs);
DicePMF operator*(double lhs, const DicePMF& rhs);
DicePMF operator/(double lhs, const DicePMF& rhs);

// Comparison Operator overloads
double operator>(double scalar, const DicePMF& rhs);
double operator>=(double scalar, const DicePMF& rhs);
double operator<(double scalar, const DicePMF& rhs);
double operator<=(double scalar, const DicePMF& rhs);
double operator==(double scalar, const DicePMF& rhs);
double operator!=(double scalar, const DicePMF& rhs);

// Dice Factory Methods
DicePMF scalar_distr(double value);
DicePMF nds_distr(int numdice, int numsides);
DicePMF max_distr(int numdice, int numsides);
DicePMF min_distr(int numdice, int numsides);
DicePMF compound_max_distr(int numdice, int totaldice, int numsides);
DicePMF compound_min_distr(int numdice, int totaldice, int numsides);

#endif//DICE_PMF_HPP_GUARD
