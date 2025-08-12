#include <string>
#include <sstream>
#include <vector>

#include "parser.hpp"
#include "scanner.hpp"
#include "parser_result.hpp"

class DiceParser
{
private:
    calc::Scanner scanner;
    calc::Parser parser;

    std::istringstream istream;
    std::ostringstream errstream;
    parse_result_t result;
public:
    DiceParser() : scanner(std::cin, std::cerr), parser(&scanner, result) {}
    parse_result_t parse(const std::string& dice_str)
    {
        errstream.clear();
        istream.clear();
        istream.str(dice_str);
        scanner.switch_streams(istream, errstream);
        parser.parse();
        return result;
    }
};

struct test_case_t
{
    std::string test_str;
    double      ans;
};

int main(int argc, char* argv[]) {

    std::vector<test_case_t> test_cases{
        {"3+10",13},
        {"16/4",4},
        {"12-20",-8},
        {"4*2",8}
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
