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
    DicePMF unary_operate(unary_op op) const;
    DicePMF binary_operate(const DicePMF& other, binary_op op) const;

    std::map<double,double> pmf;
public:
    // Constructors
    DicePMF() = default;
    DicePMF(int numsides);
    explicit DicePMF(const std::map<double,double>& m);

    // Factory methods
    static DicePMF scalar_distr(double value);
    static DicePMF nds_distr(int numdice, int numsides);
    static DicePMF max_distr(int numdice, int numsides);
    static DicePMF min_distr(int numdice, int numsides);
    static DicePMF compound_distr(int numdice, int totaldice, int numsides, bool is_max);
    static DicePMF compound_max_distr(int numdice, int totaldice, int numsides);
    static DicePMF compound_min_distr(int numdice, int totaldice, int numsides);

    // Type conversion
    bool is_scalar() const;
    double get_scalar() const;

    // Arithmetic Operator overloads
    DicePMF operator+() const;
    DicePMF operator-() const;
    DicePMF operator+(double rhs) const;
    DicePMF operator-(double rhs) const;
    DicePMF operator*(double rhs) const;
    DicePMF operator/(double rhs) const;

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
    const std::map<double,double>& get_pmf() const;
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

#endif//DICE_PMF_HPP_GUARD
