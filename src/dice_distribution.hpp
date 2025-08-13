#ifndef DICE_DISTRIBUTION_HPP_GUARD
#define DICE_DISTRIBUTION_HPP_GUARD

#include <string>
#include "dice_pdf.hpp"

class DiceDistr
{
private:
    std::string m_expr;
    DicePDF m_pdf;
public:
    // Constructors
    DiceDistr() = default;
    DiceDistr(std::string expr);
    DiceDistr(std::string expr, DicePDF pdf);

    // Comparison Operator Overloads
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
    friend DiceDistr operator+(int scalar, const DiceDistr& pdf);
    friend DiceDistr operator-(int scalar, const DiceDistr& pdf);
    friend DiceDistr operator*(int scalar, const DiceDistr& pdf);
    friend DiceDistr operator+(double scalar, const DiceDistr& pdf);
    friend DiceDistr operator-(double scalar, const DiceDistr& pdf);
    friend DiceDistr operator*(double scalar, const DiceDistr& pdf);

    // Stats functions
    double minimum();
    double maximum();
    double expected_value();
    double variance();
    double standard_dev();

    // Member Access
    Eigen::RowVectorXd get_probs();
    Eigen::RowVectorXd get_rolls();
    Eigen::Matrix2Xd   get_pdfmatrix();
    std::string        get_expr();

    // Random selection
    double roll();
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
