#include "dice_distribution.hpp"
#include "dice_parser/action_code.hpp"

#include <regex>
#include <stdexcept>
#include <cmath>
#include <iostream>

DiceDistr::DiceDistr(std::string expr) : m_expr(expr)
{
    // integer scalar format
    if(std::regex_match(expr, std::regex("^[0-9]+$")))
    {
        m_pmf = DicePMF::scalar_distr(std::stoi(expr));
    }
    // double scalar format
    else if(std::regex_match(expr, std::regex("^[0-9]+\\.[0-9]+$")))
    {
        m_pmf = DicePMF::scalar_distr(std::stod(expr));
    }
    // numdice and numsides format (nds)
    else if(std::regex_match(expr, std::regex("^[0-9]+d[0-9]+$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int numsides = std::stoi(match[2]);
        if(numdice <= 0 || numsides <= 0) throw action_code::invalid_formula;
        m_pmf = DicePMF::nds_distr(numdice, numsides);
    }
    // advantage (adv)
    else if(std::regex_match(expr, std::regex("^adv$")))
    {
        m_pmf = DicePMF::max_distr(2,20);
    }
    // disadvantage (dis)
    else if(std::regex_match(expr, std::regex("^dis$")))
    {
        m_pmf = DicePMF::min_distr(2,20);
    }
    // min(nds) format
    else if(std::regex_match(expr, std::regex("^min\\([0-9]+d[0-9]+\\)$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int numsides = std::stoi(match[2]);
        if(numdice <= 0 || numsides <= 0) throw action_code::invalid_formula;
        m_pmf = DicePMF::min_distr(numdice, numsides);
    }
    // max(nds) format
    else if(std::regex_match(expr, std::regex("^max\\([0-9]+d[0-9]+\\)$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int numsides = std::stoi(match[2]);
        if(numdice <= 0 || numsides <= 0) throw action_code::invalid_formula;
        m_pmf = DicePMF::max_distr(numdice, numsides);
    }
    // min(3,4d6) format
    else if(std::regex_match(expr, std::regex("^min\\([0-9]+,[0-9]+d[0-9]+\\)$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+),([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int totaldice = std::stoi(match[2]);
        int numsides = std::stoi(match[3]);
        if(numdice <= 0 || numsides <= 0 || totaldice <= 0 || numdice > totaldice) throw action_code::invalid_formula;
        m_pmf = DicePMF::compound_min_distr(numdice, totaldice, numsides);
    }
    // max(3,4d6) format
    else if(std::regex_match(expr, std::regex("^max\\([0-9]+,[0-9]+d[0-9]+\\)$")))
    {
        std::smatch match;
        std::regex_search(expr, match, std::regex("([0-9]+),([0-9]+)d([0-9]+)"));
        int numdice = std::stoi(match[1]);
        int totaldice = std::stoi(match[2]);
        int numsides = std::stoi(match[3]);
        if(numdice <= 0 || numsides <= 0 || totaldice <= 0 || numdice > totaldice) throw action_code::invalid_formula;
        m_pmf = DicePMF::compound_max_distr(numdice, totaldice, numsides);
    }
    else
    {
        throw action_code::invalid_formula;
    }
};

DiceDistr::DiceDistr(std::string expr, DicePMF pmf) : m_expr(expr), m_pmf(pmf) {}

// Type conversion
bool DiceDistr::is_scalar() const { return m_pmf.is_scalar(); }
double DiceDistr::get_scalar() const { return m_pmf.get_scalar(); }

// Comparison Operator Overloads
DiceDistr DiceDistr::operator>(const DiceDistr& rhs) const  { return DiceDistr("(" + (this->m_expr) + ">" + (rhs.m_expr) + ")",  DicePMF::scalar_distr(m_pmf>rhs.m_pmf)); }
DiceDistr DiceDistr::operator<(const DiceDistr& rhs) const  { return DiceDistr("(" + (this->m_expr) + "<" + (rhs.m_expr) + ")",  DicePMF::scalar_distr(m_pmf<rhs.m_pmf)); }
DiceDistr DiceDistr::operator>=(const DiceDistr& rhs) const { return DiceDistr("(" + (this->m_expr) + ">=" + (rhs.m_expr) + ")", DicePMF::scalar_distr(m_pmf>=rhs.m_pmf)); }
DiceDistr DiceDistr::operator<=(const DiceDistr& rhs) const { return DiceDistr("(" + (this->m_expr) + "<=" + (rhs.m_expr) + ")", DicePMF::scalar_distr(m_pmf<=rhs.m_pmf)); }
DiceDistr DiceDistr::operator==(const DiceDistr& rhs) const { return DiceDistr("(" + (this->m_expr) + "==" + (rhs.m_expr) + ")", DicePMF::scalar_distr(m_pmf==rhs.m_pmf)); }
DiceDistr DiceDistr::operator!=(const DiceDistr& rhs) const { return DiceDistr("(" + (this->m_expr) + "!=" + (rhs.m_expr) + ")", DicePMF::scalar_distr(m_pmf!=rhs.m_pmf)); }
double DiceDistr::operator>(double scalar) const { return (m_pmf>scalar); }
double DiceDistr::operator<(double scalar) const { return (m_pmf<scalar); }
double DiceDistr::operator>=(double scalar) const { return (m_pmf>=scalar); }
double DiceDistr::operator<=(double scalar) const { return (m_pmf<=scalar); }
double DiceDistr::operator==(double scalar) const { return (m_pmf==scalar); }
double DiceDistr::operator!=(double scalar) const { return (m_pmf!=scalar); }
double operator>(double scalar, const DiceDistr& rhs) { return (scalar>rhs.m_pmf); }
double operator>=(double scalar, const DiceDistr& rhs) { return (scalar>=rhs.m_pmf); }
double operator<(double scalar, const DiceDistr& rhs) { return (scalar<rhs.m_pmf); }
double operator<=(double scalar, const DiceDistr& rhs) { return (scalar<=rhs.m_pmf); }
double operator==(double scalar, const DiceDistr& rhs) { return (scalar==rhs.m_pmf); }
double operator!=(double scalar, const DiceDistr& rhs) { return (scalar!=rhs.m_pmf); }

// Arithmetic Operator overloads
DiceDistr DiceDistr::operator+() const { return (*this); }
DiceDistr DiceDistr::operator-() const { return DiceDistr("-"+(this->m_expr), -(this->m_pmf)); }
DiceDistr DiceDistr::operator+(const DiceDistr& rhs) const { return DiceDistr(this->m_expr + "+" + rhs.m_expr, this->m_pmf + rhs.m_pmf); }
DiceDistr DiceDistr::operator+(int scalar) const { return DiceDistr(this->m_expr + "+" + std::to_string(scalar), this->m_pmf + scalar); }
DiceDistr DiceDistr::operator+(double scalar) const { return DiceDistr(this->m_expr + "+" + std::to_string(scalar), this->m_pmf + scalar); }
DiceDistr DiceDistr::operator-(const DiceDistr& rhs) const { return DiceDistr(this->m_expr + "-" + rhs.m_expr, this->m_pmf - rhs.m_pmf); }
DiceDistr DiceDistr::operator-(int scalar) const { return DiceDistr(this->m_expr + "-" + std::to_string(scalar), this->m_pmf - scalar); }
DiceDistr DiceDistr::operator-(double scalar) const { return DiceDistr(this->m_expr + "-" + std::to_string(scalar), this->m_pmf - scalar); }
DiceDistr DiceDistr::operator*(int scalar) const { return DiceDistr("(" + this->m_expr + ")*" + std::to_string(scalar), this->m_pmf * scalar); }
DiceDistr DiceDistr::operator*(double scalar) const { return DiceDistr("(" + this->m_expr + ")*" + std::to_string(scalar), this->m_pmf * scalar); }
DiceDistr DiceDistr::operator*(const DiceDistr& rhs) const { return DiceDistr("(" + this->m_expr + ")*" + rhs.m_expr, this->m_pmf * rhs.m_pmf); }
DiceDistr DiceDistr::operator/(int scalar) const { return DiceDistr("(" + this->m_expr + ")/" + std::to_string(scalar), this->m_pmf / scalar); }
DiceDistr DiceDistr::operator/(double scalar) const { return DiceDistr("(" + this->m_expr + ")/" + std::to_string(scalar), this->m_pmf / scalar); }
DiceDistr DiceDistr::operator/(const DiceDistr& rhs) const { return DiceDistr("(" + this->m_expr + ")/" + rhs.m_expr, this->m_pmf / rhs.m_pmf); }
DiceDistr operator+(int scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "+" + rhs.m_expr, scalar + rhs.m_pmf); }
DiceDistr operator-(int scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "-" + rhs.m_expr, scalar - rhs.m_pmf); }
DiceDistr operator*(int scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "*(" + rhs.m_expr + ")", scalar * rhs.m_pmf); }
DiceDistr operator+(double scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "+" + rhs.m_expr, scalar + rhs.m_pmf); }
DiceDistr operator-(double scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "-" + rhs.m_expr, scalar - rhs.m_pmf); }
DiceDistr operator*(double scalar, const DiceDistr& rhs) { return DiceDistr(std::to_string(scalar) + "*(" + rhs.m_expr + ")", scalar * rhs.m_pmf); }

// Builtin functions
DiceDistr DiceDistr::abs() const { return DiceDistr("abs(" + this->m_expr + ")", this->m_pmf.abs());}
DiceDistr DiceDistr::ceil() const { return DiceDistr("ceil(" + this->m_expr + ")", this->m_pmf.ceil());}
DiceDistr DiceDistr::floor() const { return DiceDistr("floor(" + this->m_expr + ")", this->m_pmf.floor());}
DiceDistr DiceDistr::trunc() const { return DiceDistr("trunc(" + this->m_expr + ")", this->m_pmf.trunc());}
DiceDistr DiceDistr::round() const { return DiceDistr("round(" + this->m_expr + ")", this->m_pmf.round());}
DiceDistr DiceDistr::sqrt() const { return DiceDistr("sqrt(" + this->m_expr + ")", this->m_pmf.sqrt());}
DiceDistr DiceDistr::pow(double rhs) const { return DiceDistr("pow(" + this->m_expr + "," + std::to_string(rhs) + ")", this->m_pmf.pow(rhs));}
DiceDistr DiceDistr::pow(const DiceDistr& rhs) const { return DiceDistr("pow(" + this->m_expr + "," + rhs.m_expr + ")", this->m_pmf.pow(rhs.m_pmf));}

// Stats functions
double DiceDistr::minimum() const { return m_pmf.minimum(); }
double DiceDistr::maximum() const { return m_pmf.maximum(); }
double DiceDistr::expected_value() const { return m_pmf.expected_value(); }
double DiceDistr::variance() const { return m_pmf.variance(); }
double DiceDistr::standard_dev() const { return m_pmf.standard_dev(); }

// Member Access
const std::map<double,double>& DiceDistr::get_pmf() const { return m_pmf.get_pmf(); }   
std::string DiceDistr::get_expr() const { return m_expr; }

// Random selection
double DiceDistr::roll() { return m_pmf.roll(); }

// Debug/Utility functions
std::ostream& operator<< (std::ostream& stream, const DiceDistr& distr)
{
    stream << distr.m_expr << "\r\n";
    stream << distr.m_pmf << "\r\n";
    return stream;
}
