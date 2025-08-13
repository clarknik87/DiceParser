#include <string>
#include <sstream>
#include <vector>

#include "dice_parser.hpp"

struct test_case_t
{
    std::string test_str;
    double      ans;
};

int main(int argc, char* argv[])
{
    std::vector<test_case_t> test_cases{
        // Unary Arithmetic
        {"+5",5},
        {"-6",-6},
        // Integer Arithmetic
        {"3+10",13},
        {"16/4",4},
        {"12-20",-8},
        {"4*2",8},
        // Floating Point Arithmetic
        {"1.5+2.2",3.7},
        {"5/2",2.5},
        {"100.0 - 50.5",49.5},
        {"4*2.2",8.8},
        // Parenthesis
        {"(3+5)*2", 16},
        {"((3+5)*(1+1))/2", 8},
        // Basic Dice Rolls
        {"1d1", 1},
    };

    DiceParser parser;

    int err_cnt = 0;
    for(auto test_case : test_cases)
    {
        auto parse_ans = std::get<double>(parser.parse(test_case.test_str));
        if(parse_ans == test_case.ans)
        {
            std::cout << "  [PASSED] " << test_case.test_str << " = " << parse_ans << std::endl;
        }
        else
        {
            std::cout << "  [FAILED] " << test_case.test_str << " = " << parse_ans << std::endl;
            ++err_cnt;
        }
    }
    if(err_cnt == 0)
    {
        std::cout << "[PASSED] " << test_cases.size() << "/" << test_cases.size() << " test cases" << std::endl;
    }
    else
    {
        std::cout << "[FAILED] " << err_cnt << "/" << test_cases.size() << " test cases" << std::endl;
    }
    return 0;
}
