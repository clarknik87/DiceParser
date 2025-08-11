#include <string>
#include <sstream>
#include <vector>

#include "parser.hpp"
#include "scanner.hpp"

class DiceParser
{
private:
    calc::Scanner scanner;
    calc::Parser parser;

    std::istringstream istream;
    std::ostringstream errstream;
public:
    DiceParser() : scanner(std::cin, std::cerr), parser(&scanner) {}
    int parse(const std::string& dice_str)
    {
        errstream.clear();
        istream.clear();
        istream.str(dice_str);
        scanner.switch_streams(istream, errstream);
        return parser.parse();
    }
};

int main(int argc, char* argv[]) {

    std::vector<std::string> test_cases{
        "1+1",
        "7*3"
    };

    DiceParser parser;

    for(auto test_case : test_cases)
    {
        std::cout << test_case << " = " << parser.parse(test_case + "\n") << std::endl;
    }
    return 0;
}
