#include <iomanip>
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
DicePMF DicePMF::merge(const DicePMF& other, merge_op op) const
{
    // Operator look up table
    using two_arg_func_ptr = double (*)(double, double);
    std::map<merge_op, two_arg_func_ptr> func_lut{
        {merge_op::add, [](double a, double b){return a+b;}},
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
    return merge(rhs, merge_op::add);
}

DicePMF DicePMF::operator*(double scalar) const
{
    DicePMF d;
    for(const auto& [roll,prob] : pmf)
        d.pmf[scalar*roll] = prob;
    return d;
}

/**
 * ELEMENT ACCESS
 */
const std::map<double,double>& DicePMF::get_pmf()
{
    return pmf;
}

auto DicePMF::rolls_iter() const
{
    return std::views::keys(pmf);
}

auto DicePMF::probs_iter() const
{
    return std::views::values(pmf);
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

/**
 * DEBUG/UTILITY METHODS
 */
std::ostream& operator<< (std::ostream& stream, const DicePMF& pmf)
{
    stream << std::setprecision(4);
    stream << "[";
    for(auto p : pmf.probs_iter())
        stream << std::setw(8) << p;
    stream << "]\r\n[";
    for(auto r : pmf.rolls_iter())
        stream << std::setw(8) << r;
    stream << "]";
    return stream;
}
