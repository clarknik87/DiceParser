#ifndef DICE_DISTRIBUTION_HPP_GUARD
#define DICE_DISTRIBUTION_HPP_GUARD

#include <string>
#include "dice_pmf.hpp"

class DiceDistr
{
private:
    std::string m_expr;
    DicePMF m_pmf;
public:
    // Constructors
    DiceDistr() = default;
    DiceDistr(double scalar);
    DiceDistr(std::string expr);
    DiceDistr(std::string expr, DicePMF pmf);

    // Type Conversion
    bool is_scalar() const;
    double get_scalar() const;

    // Comparison Operator Overloads
    DiceDistr operator>(const DiceDistr& rhs) const;
    DiceDistr operator<(const DiceDistr& rhs) const;
    DiceDistr operator>=(const DiceDistr& rhs) const;
    DiceDistr operator<=(const DiceDistr& rhs) const;
    DiceDistr operator==(const DiceDistr& rhs) const;
    DiceDistr operator!=(const DiceDistr& rhs) const;
    double operator>(double scalar) const;
    double operator<(double scalar) const;
    double operator>=(double scalar) const;
    double operator<=(double scalar) const;
    double operator==(double scalar) const;
    double operator!=(double scalar) const;
    friend double operator>(double scalar, const DiceDistr& rhs);
    friend double operator>=(double scalar, const DiceDistr& rhs);
    friend double operator<(double scalar, const DiceDistr& rhs);
    friend double operator<=(double scalar, const DiceDistr& rhs);
    friend double operator==(double scalar, const DiceDistr& rhs);
    friend double operator!=(double scalar, const DiceDistr& rhs);

    // Arithmetic Operator overloads
    DiceDistr operator+() const;
    DiceDistr operator-() const;
    DiceDistr operator+(const DiceDistr& rhs) const;
    DiceDistr operator+(int scalar) const;
    DiceDistr operator+(double scalar) const;
    DiceDistr operator-(const DiceDistr& rhs) const;
    DiceDistr operator-(int scalar) const;
    DiceDistr operator-(double scalar) const;
    DiceDistr operator*(int scalar) const;
    DiceDistr operator*(double scalar) const;
    DiceDistr operator*(const DiceDistr& scalar) const;
    DiceDistr operator/(int scalar) const;
    DiceDistr operator/(double scalar) const;
    DiceDistr operator/(const DiceDistr& scalar) const;
    friend DiceDistr operator+(int scalar, const DiceDistr& pdf);
    friend DiceDistr operator-(int scalar, const DiceDistr& pdf);
    friend DiceDistr operator*(int scalar, const DiceDistr& pdf);
    friend DiceDistr operator+(double scalar, const DiceDistr& pdf);
    friend DiceDistr operator-(double scalar, const DiceDistr& pdf);
    friend DiceDistr operator*(double scalar, const DiceDistr& pdf);

    // Builtin functions
    DiceDistr abs() const;
    DiceDistr ceil() const;
    DiceDistr floor() const;
    DiceDistr trunc() const;
    DiceDistr round() const;
    DiceDistr sqrt() const;
    DiceDistr pow(double rhs) const;
    DiceDistr pow(const DiceDistr& rhs) const;

    // Stats functions
    double minimum() const;
    double maximum() const;
    double expected_value() const;
    double variance() const;
    double standard_dev() const;
    DiceDistr minimum_as_distr() const;
    DiceDistr maximum_as_distr() const;
    DiceDistr expected_value_as_distr() const;
    DiceDistr variance_as_distr() const;
    DiceDistr standard_dev_as_distr() const;

    // Member Access
    const std::map<double,double>& get_pmf() const;   
    std::string get_expr() const;

    // Random selection
    double roll() const;
    DiceDistr roll_as_distr() const;

    // Debug/Utility functions
    friend std::ostream& operator<< (std::ostream& stream, const DiceDistr& distr);
};

double operator>(double scalar, const DiceDistr& rhs);
double operator>=(double scalar, const DiceDistr& rhs);
double operator<(double scalar, const DiceDistr& rhs);
double operator<=(double scalar, const DiceDistr& rhs);
double operator==(double scalar, const DiceDistr& rhs);
double operator!=(double scalar, const DiceDistr& rhs);

DiceDistr operator+(int scalar, const DiceDistr& pdf);
DiceDistr operator-(int scalar, const DiceDistr& pdf);
DiceDistr operator*(int scalar, const DiceDistr& pdf);
DiceDistr operator+(double scalar, const DiceDistr& pdf);
DiceDistr operator-(double scalar, const DiceDistr& pdf);
DiceDistr operator*(double scalar, const DiceDistr& pdf);

#endif//DICE_DISTRIBUTION_HPP_GUARD
