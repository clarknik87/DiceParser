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
    enum class merge_op{
        add,
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
    DicePMF merge(const DicePMF& other, merge_op op) const;
    DicePMF operator+(const DicePMF& rhs) const;
    DicePMF operator*(double scalar) const;

    // Stats functions
    double minimum() const;
    double maximum() const;
    double expected_value() const;
    double variance() const;
    double standard_dev() const;

    // Element Access
    const std::map<double,double>& get_pmf();
    auto rolls_iter() const;
    auto probs_iter() const;

    // Debug/Utility functions
    friend std::ostream& operator<< (std::ostream& stream, const DicePMF& pmf);
};

// Dice Factory Methods
DicePMF scalar_distr(double value);
DicePMF nds_distr(int numdice, int numsides);
DicePMF max_distr(int numdice, int numsides);
DicePMF min_distr(int numdice, int numsides);
DicePMF compound_max_distr(int numdice, int totaldice, int numsides);
DicePMF compound_min_distr(int numdice, int totaldice, int numsides);

#endif//DICE_PMF_HPP_GUARD
