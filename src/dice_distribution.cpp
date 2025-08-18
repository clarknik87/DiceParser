#include "dice_distribution.hpp"

#include <regex>
#include <stdexcept>
#include <cmath>
#include <iostream>

DiceDistr::DiceDistr(std::string expr) : m_expr(expr)
{
    // numdice and numsides format (nds)
    if(std::regex_match(expr, std::regex("^[0-9]+d[0-9]+$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int numsides = std::stoi(match[2]);
        if(numdice <= 0 || numsides <= 0) throw std::invalid_argument(expr);
        m_pdf = nds_distribution(numdice, numsides);
    }
    // advantage (adv)
    else if(std::regex_match(expr, std::regex("^adv$")))
    {
        m_pdf = max_distribution(2,20);
    }
    // disadvantage (dis)
    else if(std::regex_match(expr, std::regex("^dis$")))
    {
        m_pdf = min_distribution(2,20);
    }
    // min(nds) format
    else if(std::regex_match(expr, std::regex("^min\\([0-9]+d[0-9]+\\)$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int numsides = std::stoi(match[2]);
        if(numdice <= 0 || numsides <= 0) throw std::invalid_argument(expr);
        m_pdf = min_distribution(numdice, numsides);
    }
    // max(nds) format
    else if(std::regex_match(expr, std::regex("^max\\([0-9]+d[0-9]+\\)$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int numsides = std::stoi(match[2]);
        if(numdice <= 0 || numsides <= 0) throw std::invalid_argument(expr);
        m_pdf = max_distribution(numdice, numsides);
    }
    // min(3,4d6) format
    else if(std::regex_match(expr, std::regex("^min\\([0-9]+,[0-9]+d[0-9]+\\)$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+),([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int totaldice = std::stoi(match[2]);
        int numsides = std::stoi(match[3]);
        if(numdice <= 0 || numsides <= 0 || totaldice <= 0) throw std::invalid_argument(expr);
        m_pdf = compound_min_distribution(numdice, totaldice, numsides);
    }
    // max(3,4d6) format
    else if(std::regex_match(expr, std::regex("^max\\([0-9]+,[0-9]+d[0-9]+\\)$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+),([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int totaldice = std::stoi(match[2]);
        int numsides = std::stoi(match[3]);
        if(numdice <= 0 || numsides <= 0 || totaldice <= 0) throw std::invalid_argument(expr);
        m_pdf = compound_max_distribution(numdice, totaldice, numsides);
    }
    else
    {
        throw std::invalid_argument(expr);
    }
};

DiceDistr::DiceDistr(std::string expr, DicePDF pdf) : m_expr(expr), m_pdf(pdf) {}

// Comparison Operator Overloads
double DiceDistr::operator>(double scalar) const { return (m_pdf>scalar); }
double DiceDistr::operator<(double scalar) const { return (m_pdf<scalar); }
double DiceDistr::operator>=(double scalar) const { return (m_pdf>=scalar); }
double DiceDistr::operator<=(double scalar) const { return (m_pdf<=scalar); }
double DiceDistr::operator==(double scalar) const { return (m_pdf==scalar); }
double DiceDistr::operator!=(double scalar) const { return (m_pdf!=scalar); }
double operator>(double scalar, const DiceDistr& rhs) { return (scalar>rhs.m_pdf); }
double operator>=(double scalar, const DiceDistr& rhs) { return (scalar>=rhs.m_pdf); }
double operator<(double scalar, const DiceDistr& rhs) { return (scalar<rhs.m_pdf); }
double operator<=(double scalar, const DiceDistr& rhs) { return (scalar<=rhs.m_pdf); }
double operator==(double scalar, const DiceDistr& rhs) { return (scalar==rhs.m_pdf); }
double operator!=(double scalar, const DiceDistr& rhs) { return (scalar!=rhs.m_pdf); }

// Arithmetic Operator overloads
DiceDistr DiceDistr::operator+() const { return (*this); }
DiceDistr DiceDistr::operator-() const { return DiceDistr("-"+(this->m_expr), -(this->m_pdf)); }
DiceDistr DiceDistr::operator+(const DiceDistr& rhs) const { return DiceDistr(this->m_expr + "+" + rhs.m_expr, this->m_pdf + rhs.m_pdf); }
DiceDistr DiceDistr::operator+(int scalar) const { return DiceDistr(this->m_expr + "+" + std::to_string(scalar), this->m_pdf + scalar); }
DiceDistr DiceDistr::operator+(double scalar) const { return DiceDistr(this->m_expr + "+" + std::to_string(scalar), this->m_pdf + scalar); }
DiceDistr DiceDistr::operator-(const DiceDistr& rhs) const { return DiceDistr(this->m_expr + "-" + rhs.m_expr, this->m_pdf - rhs.m_pdf); }
DiceDistr DiceDistr::operator-(int scalar) const { return DiceDistr(this->m_expr + "-" + std::to_string(scalar), this->m_pdf - scalar); }
DiceDistr DiceDistr::operator-(double scalar) const { return DiceDistr(this->m_expr + "-" + std::to_string(scalar), this->m_pdf - scalar); }
DiceDistr DiceDistr::operator*(int scalar) const { return DiceDistr("(" + this->m_expr + ")*" + std::to_string(scalar), this->m_pdf * scalar); }
DiceDistr DiceDistr::operator*(double scalar) const { return DiceDistr("(" + this->m_expr + ")*" + std::to_string(scalar), this->m_pdf * scalar); }
DiceDistr operator+(int scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "+" + rhs.m_expr, scalar + rhs.m_pdf); }
DiceDistr operator-(int scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "-" + rhs.m_expr, scalar - rhs.m_pdf); }
DiceDistr operator*(int scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "*(" + rhs.m_expr + ")", scalar * rhs.m_pdf); }
DiceDistr operator+(double scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "+" + rhs.m_expr, scalar + rhs.m_pdf); }
DiceDistr operator-(double scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "-" + rhs.m_expr, scalar - rhs.m_pdf); }
DiceDistr operator*(double scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "*(" + rhs.m_expr + ")", scalar * rhs.m_pdf); }

// Stats functions
double DiceDistr::minimum() const { return m_pdf.minimum(); }
double DiceDistr::maximum() const { return m_pdf.maximum(); }
double DiceDistr::expected_value() const { return m_pdf.expected_value(); }
double DiceDistr::variance() const { return m_pdf.variance(); }
double DiceDistr::standard_dev() const { return m_pdf.standard_dev(); }

// Member Access
Eigen::RowVectorXd DiceDistr::get_probs() const { return m_pdf.get_probs(); }
Eigen::RowVectorXd DiceDistr::get_rolls() const { return m_pdf.get_rolls(); }
Eigen::Matrix2Xd   DiceDistr::get_pdfmatrix() const { return m_pdf.get_pdfmatrix(); }
std::string        DiceDistr::get_expr() const { return m_expr; }

// Random selection
double DiceDistr::roll() { return m_pdf.roll(); }